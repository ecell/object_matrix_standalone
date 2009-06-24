#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Sphere.hpp"

template<typename T_, typename Tsid_>
struct Particle
{
    typedef Sphere<T_> sphere_type;
    typedef Tsid_ species_id_type;
    typedef typename sphere_type::position_type position_type;
    typedef typename sphere_type::length_type length_type;

    Particle(): sphere_(), species_id_() {}

    Particle(species_id_type const& species_id, sphere_type const& sphere)
        : sphere_(sphere), species_id_(species_id) {}

    position_type& position()
    {
        return sphere_.position();
    }

    position_type const& position() const
    {
        return sphere_.position();
    }

    length_type& radius()
    {
        return sphere_.radius();
    }

    length_type const& radius() const
    {
        return sphere_.radius();
    }

    species_id_type const& sid() const
    {
        return species_id_;
    }

    species_id_type& sid()
    {
        return species_id_;
    }

private:
    sphere_type sphere_;
    species_id_type species_id_;
};

#endif /* PARTICLE_HPP */
