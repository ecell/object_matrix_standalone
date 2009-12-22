#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <set>
#include <iostream>
#include <gsl/gsl_errno.h>

#include <boost/tuple/tuple.hpp>
#include <boost/python.hpp>
#include <boost/python/return_by_value.hpp>
#include <boost/python/manage_new_object.hpp>
#include <boost/python/return_internal_reference.hpp>

#include "Defs.hpp"
#include "Vector3.hpp"
#include "Particle.hpp"
#include "Sphere.hpp"
#include "SpeciesType.hpp"
#include "ParticleID.hpp"
#include "ShellID.hpp"
#include "SerialIDGenerator.hpp"
#include "MatrixSpace.hpp"
#include "SerialIDGenerator.hpp"
#include "Model.hpp"
#include "World.hpp"

#include "peer/Exception.hpp"
#include "peer/GeneratorIteratorWrapper.hpp"
#include "peer/Identifier.hpp"
#include "peer/MatrixSpace.hpp"
#include "peer/numpy/type_mappings.hpp"
#include "peer/ReactionRule.hpp"
#include "peer/set_indexing_suite.hpp"
#include "peer/SpeciesType.hpp"
#include "peer/Particle.hpp"
#include "peer/tuple_converters.hpp"
#include "peer/utils.hpp"
#include "peer/py_hash_support.hpp"

#include "get_mapper_mf.hpp"

typedef Real length_type;
typedef Vector3<length_type> position_type;
typedef SpeciesTypeID species_id_type;
typedef ParticleID particle_id_type;
typedef Particle<length_type, species_id_type> particle_type;
typedef Sphere<length_type> sphere_type;
typedef ShellID shell_id_type;

struct position_to_ndarray_converter
{
    typedef position_type native_type;
    
    static PyObject* convert(const native_type& p)
    {
        static const npy_intp dims[1] = { native_type::size() };
        void* data( PyDataMem_NEW( native_type::size() * sizeof(length_type) ) );
        memcpy(data, static_cast<const void*>( p.data() ),
               native_type::size() * sizeof(length_type));
        PyObject* array(
            PyArray_New(&PyArray_Type, 1, 
                        const_cast<npy_intp*>( dims ),
                        peer::util::get_numpy_typecode<length_type>::value,
                        NULL, data, 0, NPY_CARRAY, NULL ));
        reinterpret_cast<PyArrayObject*>( array )->flags |= NPY_OWNDATA;
        return array;
    }
};

struct ndarray_to_position_converter
{
    typedef position_type native_type;
    
    static void* convertible(PyObject* ptr)
    {
        if (!PyArray_Check(ptr))
        {
            return NULL;
        }
        
        PyObject* retval(
            PyArray_CastToType(
                reinterpret_cast<PyArrayObject*>(ptr),
                     PyArray_DescrFromType(
                         peer::util::get_numpy_typecode<double>::value), 0));
        if (!retval)
        {
            return NULL;
        }
        
        if (PyObject_Size(retval) != 3)
        {
            boost::python::decref(retval);
            return NULL;
        }
        
        return retval;
    }
    
    static void construct(PyObject* ptr,
            boost::python::converter::rvalue_from_python_storage<native_type>* data)
    {
        PyArrayObject* array_obj = static_cast<PyArrayObject*>(
            data->stage1.convertible);
        data->stage1.convertible = new(data->storage.bytes) native_type(
            reinterpret_cast<double*>(PyArray_DATA(array_obj)));
        boost::python::decref(reinterpret_cast<PyObject*>(array_obj));
    }
};

struct seq_to_position_converter
{
    typedef position_type native_type;
    
    static void* convertible(PyObject* ptr)
    {
        if (!PySequence_Check(ptr))
        {
            return NULL;
        }
        
        if (PySequence_Size(ptr) != 3)
        {
            return NULL;
        }
        
        return ptr;
    }
    
    static void construct(PyObject* ptr,
      boost::python::converter::rvalue_from_python_storage<native_type>* data)
    {
        data->stage1.convertible = new(data->storage.bytes) native_type(
            PyFloat_AsDouble(boost::python::handle<>(PySequence_GetItem(ptr, 0)).get()),
            PyFloat_AsDouble(boost::python::handle<>(PySequence_GetItem(ptr, 1)).get()),
            PyFloat_AsDouble(boost::python::handle<>(PySequence_GetItem(ptr, 2)).get()));
    }
};

struct sphere_to_python_converter
{
    typedef sphere_type native_type;

    static PyObject* convert(native_type const& v)
    {
        return boost::python::incref(
            boost::python::object(boost::make_tuple(
                v.position(), v.radius())).ptr());
    }
};

struct python_to_sphere_converter
{
    typedef sphere_type native_type;

    static void* convertible(PyObject* pyo)
    {
        if (!PyTuple_Check(pyo))
        {
            return 0;
        }
        if (PyTuple_Size(pyo) != 2)
        {
            return 0;
        }
        return pyo;
    }

    static void construct(PyObject* pyo, 
                          boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        PyObject* items[] = { PyTuple_GetItem(pyo, 0), PyTuple_GetItem(pyo, 1) };
        void* storage(reinterpret_cast<
            boost::python::converter::rvalue_from_python_storage<
                native_type >*
            >(data)->storage.bytes);
        new (storage) native_type (
            boost::python::extract<
                native_type::position_type>(items[0]),
            PyFloat_AsDouble(items[1]));
        data->convertible = storage;
    }
};

template<typename T_>
void register_id_generator(char const* class_name)
{
    using namespace boost::python;

    class_<SerialIDGenerator<T_> >(class_name, init<int>())
        .def("__call__", &SerialIDGenerator<T_>::operator())
        ;
}
/*
struct particle_to_python_converter
{
    typedef particle_type native_type;
    
    static PyObject* convert(const native_type& p)
    {
        return boost::python::incref(
            boost::python::object(
                boost::make_tuple(p.position(), p.radius(), p.sid())).ptr());
    }
};
*/
struct python_to_particle_converter 
{
    static void* convertible(PyObject* pyo)
    {
        if (!PySequence_Check(pyo))
        {
            return 0;
        }
        if (PySequence_Size(pyo) != 3)
        {
            return 0;
        }
        return pyo;
    }

    static void construct(PyObject* pyo, 
                          boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        PyObject* items[] = {
            PySequence_GetItem(pyo, 0),
            PySequence_GetItem(pyo, 1),
            PySequence_GetItem(pyo, 2)
        };
        void* storage(reinterpret_cast<
            boost::python::converter::rvalue_from_python_storage<particle_type>*>(data)->storage.bytes);
        new (storage) particle_type(
            boost::python::extract<species_id_type>(items[2]),

            particle_type::sphere_type(
                boost::python::extract<sphere_type::position_type>(items[0]),
                PyFloat_AsDouble(items[1])));
        data->convertible = storage;
    }
};

static boost::python::object species_type_class;

struct species_type_to_species_type_id_converter
{
    typedef ::SpeciesTypeID native_type;

    static void* convertible(PyObject* pyo)
    {
        if (!PyObject_TypeCheck(pyo, reinterpret_cast<PyTypeObject*>(
                species_type_class.ptr())))
        {
            return 0;
        }
        return pyo;
    }

    static void construct(PyObject* pyo, 
                          boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        using namespace boost::python;
        void* storage(reinterpret_cast<
            converter::rvalue_from_python_storage<native_type>* >(
                data)->storage.bytes);
        new (storage) native_type(static_cast<SpeciesType*>(extract<SpeciesType*>(object(borrowed(pyo))))->id());
        data->convertible = storage;
    }
};

template<typename T_>
struct world_get_species
{
    typedef typename T_::species_iterator iterator;

    static iterator begin(T_& impl)
    {
        return impl.get_species().begin();
    }

    static iterator end(T_& impl)
    {
        return impl.get_species().end();
    }
};

template<typename T_>
static void register_world_class(char const* name)
{
    using namespace boost::python;
    typedef T_ world_type;

    class_<world_type>(name, init<typename world_type::length_type,
                                  typename world_type::size_type>())
        .add_property("num_particles", &world_type::num_particles)
        .add_property("world_size", &world_type::world_size)
        .add_property("cell_size", &world_type::cell_size)
        .add_property("matrix_size", &world_type::matrix_size)
        .add_property("species",
            range<return_value_policy<return_by_value>, world_type const&>(
                &world_get_species<world_type const>::begin,
                &world_get_species<world_type const>::end))
        .def("add_species", &world_type::add_species)
        .def("get_species",
            (typename world_type::species_type const&(world_type::*)(typename world_type::species_id_type const&) const)&world_type::get_species,
            return_internal_reference<>())
        .def("distance", &world_type::distance)
        .def("distance_sq", &world_type::distance_sq)
        .def("apply_boundary", (typename world_type::position_type(world_type::*)(typename world_type::position_type const&) const)&world_type::apply_boundary)
        .def("apply_boundary", (typename world_type::length_type(world_type::*)(typename world_type::length_type const&) const)&world_type::apply_boundary)
        .def("cyclic_transpose", (typename world_type::position_type(world_type::*)(typename world_type::position_type const&, typename world_type::position_type const&) const)&world_type::cyclic_transpose)
        .def("cyclic_transpose", (typename world_type::length_type(world_type::*)(typename world_type::length_type const&, typename world_type::length_type const&) const)&world_type::cyclic_transpose)
        .def("new_particle", &world_type::new_particle)
        .def("check_overlap", (typename world_type::particle_id_pair_list*(world_type::*)(typename world_type::particle_id_pair const&) const)&world_type::check_overlap, return_value_policy<manage_new_object>())
        .def("check_overlap", (typename world_type::particle_id_pair_list*(world_type::*)(typename world_type::sphere_type const&, typename world_type::particle_id_type const&) const)&world_type::check_overlap, return_value_policy<manage_new_object>())
        .def("check_overlap", (typename world_type::particle_id_pair_list*(world_type::*)(typename world_type::sphere_type const&) const)&world_type::check_overlap, return_value_policy<manage_new_object>())
        .def("update_particle", &world_type::update_particle)
        .def("remove_particle", &world_type::remove_particle)
        .def("get_particle", &world_type::get_particle)
        .def("create_transaction", &world_type::create_transaction,
                return_value_policy<manage_new_object>())
        .def("__iter__", &world_type::get_particles,
                return_value_policy<return_by_value>())
        ;
}

BOOST_PYTHON_MODULE(object_matrix)
{
    using namespace boost::python;

    import_array();

    // GSL error handler: is this the best place for this?
    gsl_set_error_handler(&gsl_error_handler);
  
    peer::util::register_std_exception_translator();

    peer::util::register_tuple_converter<
            boost::tuple<position_type, length_type> >();

    to_python_converter<
            sphere_to_python_converter::native_type,
            sphere_to_python_converter>();
    peer::util::to_native_converter<
            python_to_sphere_converter::native_type,
            python_to_sphere_converter>();

    to_python_converter<position_type,
            position_to_ndarray_converter>();
    peer::util::to_native_converter<position_type,
            ndarray_to_position_converter>();
    peer::util::to_native_converter<position_type,
            seq_to_position_converter>();

    peer::util::register_tuple_converter<
        boost::tuple<sphere_type::position_type,
                length_type, species_id_type> >();

    /*
    to_python_converter<particle_type, particle_to_python_converter>();
    */
    peer::util::to_native_converter<particle_type, python_to_particle_converter>();

    peer::ParticleWrapper<particle_type>::__register_class("Particle");

    peer::util::register_tuple_converter<
        boost::tuple<sphere_type::position_type, length_type> >();

    peer::IdentifierWrapper<particle_id_type>::__register_class("ParticleID");
    register_id_generator<particle_id_type>("ParticleIDGenerator");
    peer::IdentifierWrapper<species_id_type>::__register_class("SpeciesTypeID");
    register_id_generator<species_id_type>("SpeciesTypeIDGenerator");
    peer::IdentifierWrapper<shell_id_type>::__register_class("ShellID");
    register_id_generator<shell_id_type>("ShellIDGenerator");

    peer::MatrixSpace< MatrixSpace<sphere_type, object, get_mapper_mf> >::__register_class("ObjectContainer");
    peer::MatrixSpace< MatrixSpace<sphere_type, shell_id_type> >::__register_class("ShellContainer");
    peer::MatrixSpace< MatrixSpace<particle_type, particle_id_type> >::__register_class("ParticleContainer");

    class_<std::set<particle_id_type> >("ParticleIDSet")
        .def(peer::util::set_indexing_suite<std::set<particle_id_type> >())
        ;

    species_type_class = peer::SpeciesType::__register_class();

    class_<Model, boost::noncopyable>("Model")
        .add_property("network_rules",
            make_function(&Model::network_rules,
                return_value_policy<reference_existing_object>()))
        .def("new_species_type", &Model::new_species_type,
                return_value_policy<reference_existing_object>())
        .def("get_species_type", &Model::get_species_type,
                return_value_policy<reference_existing_object>())
        ;

    peer::ReactionRule::__register_class();

    peer::util::to_native_converter<SpeciesTypeID, species_type_to_species_type_id_converter>();

    peer::util::GeneratorIteratorWrapper<ptr_generator<NetworkRules::reaction_rule_generator> >::__register_class("ReactionRuleGenerator");

    peer::util::ExceptionWrapper<not_found, peer::util::PyExcTraits<&PyExc_LookupError> >::__register_class("NotFound");
    peer::util::ExceptionWrapper<already_exists, peer::util::PyExcTraits<&PyExc_StandardError> >::__register_class("AlreadyExists");

    class_<NetworkRules, boost::noncopyable>("NetworkRules", no_init)
        .def("add_reaction_rule", &NetworkRules::add_reaction_rule)
        .def("query_reaction_rule", static_cast<NetworkRules::reaction_rule_generator*(NetworkRules::*)(SpeciesTypeID const&) const>(&NetworkRules::query_reaction_rule), return_value_policy<return_by_value>())
        .def("query_reaction_rule", static_cast<NetworkRules::reaction_rule_generator*(NetworkRules::*)(SpeciesTypeID const&, SpeciesTypeID const&) const>(&NetworkRules::query_reaction_rule), return_value_policy<return_by_value>())
        ;

    typedef SpeciesInfo<species_id_type, length_type> species_info_type;
    class_<species_info_type>("SpeciesInfo", init<species_id_type>())
        .def(init<species_id_type, length_type, Real>())
        .add_property("id",
            make_function(&species_info_type::id,
                return_value_policy<return_by_value>()))
        .add_property("radius",
            make_function(
                &peer::util::reference_accessor_wrapper<
                    species_info_type, length_type,
                    &species_info_type::radius,
                    &species_info_type::radius>::get,
                return_value_policy<return_by_value>()),
            &peer::util::reference_accessor_wrapper<
                species_info_type, Real,
                &species_info_type::radius,
                &species_info_type::radius>::set)
        ;

    typedef CyclicWorldTraits<length_type> world_traits_type;
    typedef World<world_traits_type> CyclicWorld;
    typedef CyclicWorld::transaction_type transaction_type;

    peer::util::register_tuple_converter<CyclicWorld::particle_id_pair>();

    peer::util::GeneratorIteratorWrapper<ptr_generator<CyclicWorld::particle_id_pair_generator> >::__register_class("ParticleIDPairGenerator");

    class_<Transaction<world_traits_type>, boost::noncopyable>("Transaction", no_init)
        .add_property("num_particles", &transaction_type::num_particles)
        .add_property("added_particles",
            make_function(&transaction_type::get_added_particles,
                return_value_policy<return_by_value>()))
        .add_property("removed_particles",
            make_function(&transaction_type::get_removed_particles,
                return_value_policy<return_by_value>()))
        .add_property("modified_particles",
            make_function(&transaction_type::get_modified_particles,
                return_value_policy<return_by_value>()))
        .def("new_particle", &transaction_type::new_particle)
        .def("update_particle", &transaction_type::update_particle)
        .def("remove_particle", &transaction_type::remove_particle)
        .def("get_particle", &transaction_type::get_particle)
        .def("check_overlap", (transaction_type::particle_id_pair_list*(transaction_type::*)(transaction_type::particle_id_pair const&) const)&transaction_type::check_overlap, return_value_policy<manage_new_object>())
        .def("check_overlap", (transaction_type::particle_id_pair_list*(transaction_type::*)(transaction_type::sphere_type const&, transaction_type::particle_id_type const&) const)&transaction_type::check_overlap, return_value_policy<manage_new_object>())
        .def("check_overlap", (transaction_type::particle_id_pair_list*(transaction_type::*)(transaction_type::sphere_type const&) const)&transaction_type::check_overlap, return_value_policy<manage_new_object>())
        .def("create_transaction", &transaction_type::create_transaction,
                return_value_policy<manage_new_object>())
        .def("rollback", &transaction_type::rollback)
        .def("__iter__", &transaction_type::get_particles,
                return_value_policy<return_by_value>())
        ;

    register_world_class<CyclicWorld>("CyclicWorld");
}
