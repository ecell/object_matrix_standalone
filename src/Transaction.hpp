#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <vector>
#include <map>
#include <boost/bind.hpp>
#include "utils.hpp"
#include "ParticleContainer.hpp"
#include "sorted_list.hpp"
#include "generator.hpp"

template<typename Ttraits_>
class Transaction: public ParticleContainer<Ttraits_>
{
public:
    typedef Ttraits_ traits_type;
    typedef typename traits_type::particle_type particle_type;
    typedef typename traits_type::species_type species_type;
    typedef typename traits_type::species_id_type species_id_type;
    typedef typename traits_type::position_type position_type;
    typedef typename traits_type::particle_id_type particle_id_type;
    typedef typename traits_type::size_type size_type;
    typedef std::pair<const particle_id_type, particle_type> particle_id_pair;
    typedef abstract_limited_generator<particle_id_pair> particle_id_pair_generator;

    virtual ~Transaction() {}

    virtual particle_id_pair_generator* get_added_particles() const = 0;

    virtual particle_id_pair_generator* get_removed_particles() const = 0;

    virtual particle_id_pair_generator* get_modified_particles() const = 0;

    virtual void rollback() = 0;
};

template<typename Tpc_>
class TransactionImpl: public Transaction<typename Tpc_::traits_type>
{
public:
    typedef Tpc_ particle_container_type;
    typedef typename particle_container_type::traits_type traits_type;
    typedef typename traits_type::particle_type particle_type;
    typedef typename traits_type::species_type species_type;
    typedef typename traits_type::species_id_type species_id_type;
    typedef typename traits_type::position_type position_type;
    typedef typename traits_type::particle_id_type particle_id_type;
    typedef typename traits_type::size_type size_type;
    typedef std::pair<const particle_id_type, particle_type> particle_id_pair;
    typedef abstract_limited_generator<particle_id_pair> particle_id_pair_generator;

private:
    typedef std::map<typename particle_id_pair::first_type,
            typename particle_id_pair::second_type> particle_id_pair_set_type;
    typedef sorted_list<std::vector<particle_id_type> > particle_id_list_type;

public:
    virtual particle_id_pair new_particle(species_id_type const& sid,
            position_type const& pos)
    {
        particle_id_pair retval(pc_.new_particle(sid, pos));
        const bool result(added_particles_.push_no_duplicate(retval.first));
        BOOST_ASSERT(result);
        return retval;
    }

    virtual void update_particle(particle_id_pair const& pi_pair)
    {
        BOOST_ASSERT(removed_particles_.end() ==
                removed_particles_.find(pi_pair.first));
        std::pair<typename particle_id_pair_set_type::iterator, bool> r(
                orig_particles_.insert(particle_id_pair(
                    pi_pair.first, particle_type())));
        if (r.second &&
            added_particles_.end() == added_particles_.find(pi_pair.first))
        {
            modified_particles_.push_no_duplicate(pi_pair.first);
            particle_type _v(pc_.get_particle(pi_pair.first).second);
            std::swap((*r.first).second, _v);
        }
        pc_.update_particle(pi_pair);
    }

    virtual void remove_particle(particle_id_type const& id)
    {
        std::pair<typename particle_id_pair_set_type::iterator, bool> r(
                orig_particles_.insert(particle_id_pair(
                    id, particle_type())));
        if (r.second)
        {
            particle_type _v(pc_.get_particle(id).second);
            std::swap((*r.first).second, _v);
        }

        if (added_particles_.erase(id) == 0)
        {
            const bool result(removed_particles_.push_no_duplicate(id));
            BOOST_ASSERT(result);
        }
        else
        {
            orig_particles_.erase(id);
        }
        pc_.remove_particle(id);
    }

    virtual particle_id_pair get_particle(particle_id_type const& id) const
    {
        return pc_.get_particle(id);
    }

    virtual bool check_overlap(particle_id_pair const& s) const
    {
        return pc_.check_overlap(s);
    }

    virtual Transaction<traits_type>* create_transaction()
    {
        return new TransactionImpl<particle_container_type>(*this);
    }

    virtual species_type const& get_species(species_id_type const& id) const
    {
        return pc_.get_species(id);
    }

    virtual size_type num_particles() const
    {
        return pc_.num_particles();
    }

    virtual particle_id_pair_generator* get_particles() const
    {
        return pc_.get_particles();
    }

    virtual particle_id_pair_generator* get_added_particles() const
    {
        return make_range_generator<true>(
            make_transform_iterator_range(added_particles_,
                boost::bind(&TransactionImpl::get_particle, this, _1)));
            
    }

    virtual particle_id_pair_generator* get_removed_particles() const
    {
        return make_range_generator<true>(
            make_transform_iterator_range(removed_particles_,
                boost::bind(&TransactionImpl::get_original_particle, this, _1)));
    }

    virtual particle_id_pair_generator* get_modified_particles() const
    {
        return make_range_generator<true>(
            make_transform_iterator_range(modified_particles_,
                boost::bind(&TransactionImpl::get_original_particle, this, _1)));
    }

    virtual void rollback()
    {
        for (typename particle_id_pair_set_type::iterator
                i(orig_particles_.begin()), e(orig_particles_.end());
                i != e; ++i)
        {
            pc_.update_particle(*i);
        }

        for (typename particle_id_list_type::iterator
                i(added_particles_.begin()), e(added_particles_.end());
                i != e; ++i)
        {
            pc_.remove_particle(*i);
        }
        added_particles_.clear();
        modified_particles_.clear();
        removed_particles_.clear();
        orig_particles_.clear();
    }

    virtual ~TransactionImpl() {}

    TransactionImpl(particle_container_type& world): pc_(world) {}

private:
    particle_id_pair get_original_particle(particle_id_type const& id) const
    {
        typename particle_id_pair_set_type::const_iterator i(orig_particles_.find(id));
        if (orig_particles_.end() == i)
        {
            throw not_found(std::string("No such particle: id=")
                    + boost::lexical_cast<std::string>(id));
        }
        return *i;
    }

private:
    particle_container_type& pc_;
    particle_id_list_type added_particles_;
    particle_id_list_type modified_particles_;
    particle_id_pair_set_type orig_particles_;
    particle_id_list_type removed_particles_;
};

#endif /* TRANSACTION_HPP */
