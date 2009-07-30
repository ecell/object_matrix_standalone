#ifndef PARTICLE_CONTAINER_HPP
#define PARTICLE_CONTAINER_HPP

#include <utility>
#include "generator.hpp"
#include "unassignable_adapter.hpp"
#include "utils.hpp"

template<typename Ttraits_>
class Transaction;

template<typename Ttraits_>
class ParticleContainer
{
public:
    typedef Ttraits_ traits_type;
    typedef typename traits_type::particle_type particle_type;
    typedef typename traits_type::sphere_type sphere_type;
    typedef typename traits_type::species_type species_type;
    typedef typename traits_type::species_id_type species_id_type;
    typedef typename traits_type::position_type position_type;
    typedef typename traits_type::particle_id_type particle_id_type;
    typedef typename traits_type::size_type size_type;
    typedef std::pair<const particle_id_type, particle_type> particle_id_pair;
    typedef Transaction<traits_type> transaction_type;
    typedef abstract_limited_generator<particle_id_pair> particle_id_pair_generator;
    typedef unassignable_adapter<particle_id_pair, get_default_impl::std::vector> particle_id_pair_list;

    virtual ~ParticleContainer() {};

    virtual size_type num_particles() const = 0;

    virtual species_type const& get_species(species_id_type const& id) const = 0;

    virtual particle_id_pair new_particle(species_id_type const& sid,
            position_type const& pos) = 0;

    virtual void update_particle(particle_id_pair const& pi_pair) = 0;

    virtual void remove_particle(particle_id_type const& id) = 0;

    virtual particle_id_pair get_particle(particle_id_type const& id) const = 0;

    virtual particle_id_pair_list* check_overlap(particle_id_pair const& s) const = 0;

    virtual particle_id_pair_list* check_overlap(sphere_type const& s) const = 0;

    virtual particle_id_pair_list* check_overlap(sphere_type const& s, particle_id_type const& ignore) const = 0;

    virtual particle_id_pair_generator* get_particles() const = 0;

    virtual transaction_type* create_transaction() = 0;
};

#endif /* PARTICLE_CONTAINER_HPP */
