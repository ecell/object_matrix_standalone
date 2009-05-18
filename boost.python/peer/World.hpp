#ifndef OBJECTMATRIX_PEER_WORLD_HPP
#define OBJECTMATRIX_PEER_WORLD_HPP

#include <memory>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/return_by_value.hpp>
#include <boost/python/return_internal_reference.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

#include "peer/ObjectContainer.hpp"
#include "peer/Species.hpp"
#include "peer/Identifier.hpp"
#include "world.hpp"
#include "object_container.hpp"

namespace peer {

template<typename T_>
class World
{
public:
    typedef world<T_> impl_type;
    typedef typename impl_type::particle_id_type particle_id_type;
    typedef typename impl_type::particle_type particle_type;
    typedef typename impl_type::species_type species_type;
    typedef typename impl_type::species_id_type species_id_type;
    typedef peer::Species<typename impl_type::species_type> species_peer;
    typedef peer::ObjectContainer<typename impl_type::particle_container*>
            particle_container_peer;
    
    typedef boost::tuple<
        typename impl_type::species_map::key_type const&,
        typename impl_type::species_map::mapped_type*> new_species_retval_type;

    typedef boost::tuple<
        typename impl_type::particle_container::key_type const&,
        typename impl_type::particle_container::mapped_type*> new_particle_retval_type;

protected:
    struct my_map_indexing_suite
        : public boost::python::map_indexing_suite<
            typename impl_type::species_map,
            true,
            my_map_indexing_suite >
    {
        template<class T>
        static void extension_def(T& klass)
        {
            util::register_tuple_converter<typename impl_type::species_map::value_type>();
        }
    };

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
            PyObject* items[] = { PyTuple_GetItem(pyo, 0), PyTuple_GetItem(pyo, 1) };
            void* storage(reinterpret_cast<
                boost::python::converter::rvalue_from_python_storage<particle_type>*>(data)->storage.bytes);
            new (storage) particle_type(
                boost::python::extract<species_id_type>(items[2]),

                typename particle_type::sphere_type(
                    boost::python::extract<typename impl_type::sphere_type::position_type>(items[0]),
                    PyFloat_AsDouble(items[1])));
            data->convertible = storage;
        }
    };

public:
    World(typename impl_type::length_type world_size,
            typename impl_type::particle_container::size_type size)
        : impl_(world_size, size) {}

    new_species_retval_type
    newSpecies(double diffusion_constant,
            typename impl_type::length_type const& radius)
    {
        typename impl_type::species_map::value_type& retval =
            impl_.new_species(diffusion_constant, radius);
        return new_species_retval_type(retval.first, &retval.second);
    }

    new_particle_retval_type
    newParticle(species_id_type const& sid,
            typename impl_type::sphere_type::position_type const& pos)
    {
        typename impl_type::particle_container::value_type& retval =
                impl_.new_particle(sid, pos);
        return new_particle_retval_type(retval.first, &retval.second);
    }

    bool removeParticle(particle_id_type const& id)
    {
        return impl_.remove_particle(id);
    }

    species_type const& getSpecies(species_id_type const& id)
    {
        return impl_.get_species(id);
    }

    particle_type const& getParticle(particle_id_type const& id)
    {
        return impl_.get_particle( id );
    }

    typename impl_type::species_map& getSpeciesMap()
    {
        return impl_.species_();
    }

    particle_container_peer get_particles()
    {
        return &impl_.particles();
    }

    static void __register_class()
    {
        using namespace boost::python;

        util::register_tuple_converter<new_species_retval_type>();
        util::register_tuple_converter<new_particle_retval_type>();
        util::register_tuple_converter<typename impl_type::species_map::value_type>();

        register_exception_translator< not_found >( &translateNotFound );

        util::register_tuple_converter<
            boost::tuple<typename impl_type::sphere_type::position_type,
                    typename impl_type::length_type, species_id_type> >();

        to_python_converter<particle_type, particle_to_python_converter>();
        util::to_native_converter<particle_type, python_to_particle_converter>();

        util::register_tuple_converter<
            boost::tuple<typename impl_type::sphere_type::position_type,
                    typename impl_type::length_type> >();

        IdentifierWrapper<particle_id_type>::__register_class("ParticleID");
        IdentifierWrapper<species_id_type>::__register_class("SpeciesID");
  
        particle_container_peer::__register_class("ParticleContainer");
        species_peer::__register_class();

        class_<typename impl_type::species_map>("SpeciesMap", no_init)
            .def(my_map_indexing_suite());

        class_<World>("World",  init<typename impl_type::length_type,
                    typename impl_type::particle_container::size_type>())
            .def("getParticle", &World::getParticle,
                    return_value_policy<copy_const_reference>())
            .def("getSpecies", &World::getSpecies,
                    return_value_policy<return_by_value>())
            .def("newSpecies", &World::newSpecies)
            .def("newParticle", &World::newParticle)
            .def("removeParticle", &World::removeParticle)
            .add_property("speciesMap",
                make_function(&World::getSpeciesMap,
                    return_internal_reference<>()))
            .add_property("particles",
                make_function(&World::get_particles,
                    return_value_policy<return_by_value>()))
            ;
    }

private:
    static void translateNotFound( const not_found& anException )
    {
        PyErr_SetString( PyExc_KeyError, anException.what() );
    }

private:
    impl_type impl_;
};

} // namespace peer

#endif /* OBJECTMATRIX_PEER_WORLD_HPP */
