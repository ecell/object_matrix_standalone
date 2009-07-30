#ifndef WORLD_HPP
#define WORLD_HPP

#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/array.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include "exceptions.hpp"
#include "utils.hpp"
#include "generator.hpp"
#include "filters.hpp"
#include "abstract_set.hpp"
#include "Particle.hpp"
#include "ParticleID.hpp"
#include "SpeciesTypeID.hpp"
#include "SpeciesInfo.hpp"
#include "SerialIDGenerator.hpp"
#include "MatrixSpace.hpp"
#include "Transaction.hpp"

template<typename Tlength_type_>
struct WorldTraitsBase
{
    typedef std::size_t size_type;
    typedef Tlength_type_ length_type;
    typedef ParticleID particle_id_type;
    typedef SerialIDGenerator<particle_id_type> particle_id_generator;
    typedef SpeciesTypeID species_id_type;
    typedef Particle<length_type, species_id_type> particle_type;
    typedef SpeciesInfo<species_id_type, length_type> species_type;
    typedef typename particle_type::sphere_type sphere_type;
    typedef typename sphere_type::position_type position_type;

    template<typename Tval_>
    static Tval_ apply_boundary(Tval_ const& v, length_type const& world_size)
    {
        return v;
    }

    template<typename Tval_>
    static Tval_ cyclic_transpose(Tval_ const& p0, Tval_ const& p1, length_type const& world_size)
    {
        return p0;
    }

    template<typename Toc_, typename Tfun_, typename Tsphere_>
    static void take_neighbor(Toc_& oc, Tfun_& fun, const Tsphere_& cmp)
    {
        take_neighbor(oc, fun, cmp);
    }

    template<typename Toc_, typename Tfun_, typename Tsphere_>
    static void take_neighbor(Toc_ const& oc, Tfun_& fun, const Tsphere_& cmp)
    {
        take_neighbor(oc, fun, cmp);
    }
};

template<typename Tlength_type_>
struct CyclicWorldTraits: public WorldTraitsBase<Tlength_type_>
{
protected:
    typedef WorldTraitsBase<Tlength_type_> base_type;
public:
    typedef typename base_type::length_type length_type;
    typedef typename base_type::position_type position_type;

    template<typename Tval_>
    static Tval_ apply_boundary(Tval_ const& v, length_type const& world_size)
    {
        return modulo(v, world_size);
    }

    static length_type cyclic_transpose(length_type const& p0, length_type const& p1, length_type const& world_size)
    {
        const length_type diff(p1 - p0), half(world_size / 2);
        if (diff > half)
        {
            return p0 + half;
        }
        else if (diff < -half)
        {
            return p0 - half;
        }
        return p0;
    }

    static position_type cyclic_transpose(position_type const& p0, position_type const& p1, length_type const& world_size)
    {
        return position_type(
            cyclic_transpose(p0[0], p1[0], world_size),
            cyclic_transpose(p0[1], p1[1], world_size),
            cyclic_transpose(p0[2], p1[2], world_size)
        );
    }

    template<typename Toc_, typename Tfun_, typename Tsphere_>
    static void take_neighbor(Toc_& oc, Tfun_& fun, const Tsphere_& cmp)
    {
        take_neighbor_cyclic(oc, fun, cmp);
    }

    template<typename Toc_, typename Tfun_, typename Tsphere_>
    static void take_neighbor(Toc_ const& oc, Tfun_& fun, const Tsphere_& cmp)
    {
        take_neighbor_cyclic(oc, fun, cmp);
    }
};

template<typename Ttraits_>
class World: public ParticleContainer<Ttraits_>
{
public:
    typedef Ttraits_ traits_type;
    typedef typename traits_type::length_type length_type;
    typedef typename traits_type::species_type species_type;
    typedef typename traits_type::position_type position_type;
    typedef typename traits_type::particle_type particle_type;
    typedef typename traits_type::particle_id_type particle_id_type;
    typedef typename traits_type::particle_id_generator particle_id_generator;
    typedef typename traits_type::species_id_type species_id_type;
    typedef typename traits_type::sphere_type sphere_type;
    typedef typename traits_type::size_type size_type;
    typedef MatrixSpace<particle_type, particle_id_type> particle_matrix_type;
    typedef std::pair<const particle_id_type, particle_type> particle_id_pair;
    typedef Transaction<traits_type> transaction_type;
    typedef boost::iterator_range<typename particle_matrix_type::const_iterator> particle_id_pair_range;
    typedef abstract_limited_generator<particle_id_pair> particle_id_pair_generator;
    typedef unassignable_adapter<particle_id_pair, get_default_impl::std::vector> particle_id_pair_list;

protected:
    typedef std::map<species_id_type, species_type> species_map;
    typedef select_second<typename species_map::value_type> second_selector_type;

    template<typename Tset_>
    struct overlap_checker
    {
        overlap_checker(Tset_ const& ignore = Tset_()): ignore_(ignore), result_(0) {}

        template<typename Titer_>
        void operator()(Titer_ const& i, length_type const&)
        {
            if (!contains(ignore_, (*i).first))
            {
                if (!result_)
                {
                    result_ = new particle_id_pair_list();
                }
                result_->push_back(*i);
            }
        }

        particle_id_pair_list* result() const
        {
            return result_;
        }

    private:
        Tset_ const& ignore_;
        particle_id_pair_list* result_;
    };

    template<typename Tset_>
    struct particle_overlap_checker
    {
        typedef typename collection_value<Tset_>::type set_value_type;
        particle_overlap_checker(set_value_type const& id,
            Tset_ const& ignore = Tset_())
            : id_(id), ignore_(ignore), result_(0) {}

        template<typename Titer_>
        void operator()(Titer_ const& i, length_type const&)
        {
            if ((*i).first != id_ && !contains(ignore_, (*i).first))
            {
                if (!result_)
                {
                    result_ = new particle_id_pair_list();
                }
                result_->push_back(*i);
            }
        }

        particle_id_pair_list* result() const
        {
            return result_;
        }

    private:
        set_value_type const& id_;
        Tset_ const& ignore_;
        particle_id_pair_list* result_;
    };


public:
    typedef boost::transform_iterator<second_selector_type,
            typename species_map::const_iterator> species_iterator;
    typedef boost::iterator_range<species_iterator> species_range;

public:
    World(length_type world_size = 1., size_type size = 1)
        : pmat_(world_size, size)
    {
    }

    virtual size_type num_particles() const
    {
        return pmat_.size();
    }

    length_type world_size() const
    {
        return pmat_.world_size();
    }

    length_type cell_size() const
    {
        return pmat_.cell_size();
    }

    size_type matrix_size() const
    {
        return pmat_.matrix_size();
    }

    void add_species(species_type const& species)
    {
        species_map_[species.id()] = species;
    }

    virtual species_type const& get_species(species_id_type const& id) const
    {
        typename species_map::const_iterator i(species_map_.find(id));
        if (species_map_.end() == i)
        {
            throw not_found(std::string("Unknown species (id=") + boost::lexical_cast<std::string>(id) + ")");
        }
        return (*i).second;
    }

    species_range get_species() const
    {
        return species_range(
            species_iterator(species_map_.begin(), second_selector_type()),
            species_iterator(species_map_.end(), second_selector_type()));
    }

    length_type distance(position_type const& lhs, position_type const& rhs)
    {
        return distance(cyclic_transpose(lhs, rhs), rhs);
    }

    length_type distance_sq(position_type const& lhs, position_type const& rhs)
    {
        return distance_sq(cyclic_transpose(lhs, rhs), rhs);
    }

    position_type apply_boundary(position_type const& v) const
    {
        return traits_type::apply_boundary(v, world_size());
    }

    length_type apply_boundary(length_type const& v) const
    {
        return traits_type::apply_boundary(v, world_size());
    }

    position_type cyclic_transpose(position_type const& p0, position_type const& p1) const
    {
        return traits_type::cyclic_transpose(p0, p1, world_size());
    }

    length_type cyclic_transpose(length_type const& p0, length_type const& p1) const
    {
        return traits_type::cyclic_transpose(p0, p1, world_size());
    }

    particle_id_pair new_particle(species_id_type const& sid,
            position_type const& pos)
    {
        particle_id_pair retval(pidgen_(),
            particle_type(sid, sphere_type(pos, get_species(sid).radius())));
        pmat_.update(retval);
        return retval;
    }

    template<typename Tset_>
    particle_id_pair_list* check_overlap(particle_id_pair const& s, Tset_ const& ignore) const
    {
        particle_overlap_checker<Tset_> oc(s.first, ignore);
        traits_type::take_neighbor(pmat_, oc, s.second.as_sphere());
        return oc.result();
    }

    virtual particle_id_pair_list* check_overlap(particle_id_pair const& s) const
    {
        particle_overlap_checker<boost::array<particle_id_type, 0> > oc(s.first);
        traits_type::take_neighbor(pmat_, oc, s.second.as_sphere());
        return oc.result();
    }

    virtual particle_id_pair_list* check_overlap(sphere_type const& s, particle_id_type const& ignore) const
    {
        return check_overlap(s, array_gen(ignore));
    }

    virtual particle_id_pair_list* check_overlap(sphere_type const& s) const
    {
        return check_overlap<sphere_type>(s);
    }

    template<typename Tsph_, typename Tset_>
    particle_id_pair_list* check_overlap(Tsph_ const& s, Tset_ const& ignore,
        typename boost::disable_if<boost::is_same<Tsph_, particle_id_pair> >::type* =0) const
    {
        overlap_checker<Tset_> oc(ignore);
        traits_type::take_neighbor(pmat_, oc, s);
        return oc.result();
    }

    template<typename Tsph_>
    particle_id_pair_list* check_overlap(Tsph_ const& s,
        typename boost::disable_if<boost::is_same<Tsph_, particle_id_pair> >::type* =0) const
    {
        overlap_checker<boost::array<particle_id_type, 0> > oc;
        traits_type::take_neighbor(pmat_, oc, s);
        return oc.result();
    }

    void update_particle(particle_id_pair const& pi_pair)
    {
        pmat_.update(pi_pair);
    }

    void remove_particle(particle_id_type const& id)
    {
        pmat_.erase(id);
    }

    particle_id_pair get_particle(particle_id_type const& id) const
    {
        typename particle_matrix_type::const_iterator i(pmat_.find(id));
        if (pmat_.end() == i) {
            throw not_found(std::string("No such particle: id=")
                    + boost::lexical_cast<std::string>(id));
        }
        return *i;
    }

    transaction_type* create_transaction()
    {
        return new TransactionImpl<World>(*this);
    }

    particle_id_pair_generator* get_particles() const
    {
        return make_range_generator<particle_id_pair>(pmat_);
    }

    particle_id_pair_range get_particles_range() const
    {
        return particle_id_pair_range(pmat_.begin(), pmat_.end());
    }

private:
    particle_matrix_type pmat_;
    particle_id_generator pidgen_;
    species_map species_map_;
};

#endif /* WORLD_HPP */
