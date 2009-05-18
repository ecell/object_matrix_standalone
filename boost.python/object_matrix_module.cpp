#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <set>
#include <iostream>
#include <boost/tuple/tuple.hpp>
#include <boost/python.hpp>
#include <boost/python/return_by_value.hpp>
#include "position.hpp"
#include "particle.hpp"
#include "sphere.hpp"
#include "species.hpp"
#include "particle_id.hpp"
#include "shell_id.hpp"
#include "serial_id_generator.hpp"
#include "object_container.hpp"
#include "serial_id_generator.hpp"
#include "peer/utils.hpp"
#include "peer/tuple_converters.hpp"
#include "peer/numpy/type_mappings.hpp"
#include "peer/ObjectContainer.hpp"
#include "peer/Identifier.hpp"
#include "peer/set_indexing_suite.hpp"

typedef double length_type;
typedef position<length_type> position_type;
typedef species_id species_id_type;
typedef particle_id particle_id_type;
typedef particle<length_type> particle_type;
typedef species<length_type> species_type;
typedef sphere<length_type> sphere_type;
typedef shell_id shell_id_type;

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
            PyFloat_AsDouble(PySequence_GetItem(ptr, 0)),
            PyFloat_AsDouble(PySequence_GetItem(ptr, 1)),
            PyFloat_AsDouble(PySequence_GetItem(ptr, 2)));
    }
};

struct sphere_to_python_converter
{
    typedef sphere<length_type> native_type;

    static PyObject* convert(native_type const& v)
    {
        return boost::python::incref(
            boost::python::object(boost::make_tuple(
                v.position(), v.radius())).ptr());
    }
};

struct python_to_sphere_converter
{
    typedef sphere<length_type> native_type;

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

    class_<serial_id_generator<T_> >(class_name, init<int>())
        .def("__call__", &serial_id_generator<T_>::operator())
        ;
}

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

struct python_to_particle_converter 
{
    static void* convertible(PyObject* pyo)
    {
        if (!PyTuple_Check(pyo))
        {
            return 0;
        }
        if (PyTuple_Size(pyo) != 3)
        {
            return 0;
        }
        return pyo;
    }

    static void construct(PyObject* pyo, 
                          boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        PyObject* items[] = {
            PyTuple_GetItem(pyo, 0),
            PyTuple_GetItem(pyo, 1),
            PyTuple_GetItem(pyo, 2)
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

BOOST_PYTHON_MODULE(object_matrix)
{
    using namespace boost::python;

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

    to_python_converter<particle_type, particle_to_python_converter>();
    peer::util::to_native_converter<particle_type, python_to_particle_converter>();

    peer::util::register_tuple_converter<
        boost::tuple<sphere_type::position_type, length_type> >();

    peer::IdentifierWrapper<particle_id_type>::__register_class("ParticleID");
    register_id_generator<particle_id_type>("ParticleIDGenerator");
    peer::IdentifierWrapper<species_id_type>::__register_class("SpeciesID");
    register_id_generator<species_id_type>("SpeciesIDGenerator");
    peer::IdentifierWrapper<shell_id_type>::__register_class("ShellID");
    register_id_generator<shell_id_type>("ShellIDGenerator");

    import_array();
#if OBJECTMATRIX_USE_ITERATOR
    peer::util::register_stop_iteration_exc_translator();
#endif
    //peer::ObjectContainer< object_container<sphere_type, int> >::__register_class("SphericalObjectContainer");
    peer::ObjectContainer< object_container<sphere_type, shell_id> >::__register_class("ShellContainer");
    peer::ObjectContainer< object_container<particle_type, particle_id_type> >::__register_class("ParticleContainer");

    class_<species_type>("Species",
        init<std::string, double, length_type>())
        .add_property("name",
            make_function(&species_type::name,
                return_value_policy<return_by_value>()))
        .add_property("radius",
            make_function(&species_type::radius,
                return_value_policy<return_by_value>()))
        .add_property("D",
            make_function(&species_type::D,
                return_value_policy<return_by_value>()))
        ;

    class_<std::set<particle_id_type> >("ParticleIDSet")
        .def(peer::util::set_indexing_suite<std::set<particle_id_type> >())
        ;
}
