#ifndef REACTION_RULE_HPP
#define REACTION_RULE_HPP

#include <set>
#include <ostream>
#include <vector>
#include <algorithm>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/range/size.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_categories.hpp>

#include "Defs.hpp"
#include "utils.hpp"
#include "SpeciesTypeID.hpp"
#include "twofold_container.hpp"
#include "utils/memberwise_compare.hpp"

class ReactionRule
{
private:
    typedef std::vector<SpeciesTypeID> species_type_id_vector;

public:
    typedef species_type_id_vector::const_iterator species_type_id_iterator;
    typedef boost::iterator_range<species_type_id_iterator> species_type_id_range;
    typedef twofold_container<SpeciesTypeID> Reactants;

public:
    Reactants const& get_reactants() const
    {
        return reactants_;
    }

    void add_product(SpeciesTypeID const& s)
    {
        products_.insert(
            std::lower_bound(products_.begin(), products_.end(), s),
            s);
    }

    species_type_id_range get_products() const
    {
        return products_;
    }

    Real k() const
    {
        return k_;
    }

    Real& k()
    {
        return k_;
    }

    ReactionRule()
        : reactants_(), k_(.0) {}

    explicit ReactionRule(Reactants const& _reactants, Real k = .0)
        : reactants_(_reactants), k_(k) {}

    template<typename Trange_>
    ReactionRule(Reactants const& _reactants, Trange_ const& products, Real k = .0)
        : reactants_(_reactants), k_(k)
    {
        std::for_each(boost::begin(products), boost::end(products),
                boost::bind(&ReactionRule::add_product, this, _1));
        std::stable_sort(products_.begin(), products_.end());
    }

private:
    Reactants reactants_;
    species_type_id_vector products_;
    Real k_;
};

inline bool operator<(ReactionRule const& lhs, ReactionRule const& rhs)
{
    int tmp = memberwise_compare(lhs.get_reactants(), rhs.get_reactants());
    if (tmp > 0)
    {
        return false;
    }
    else if (tmp < 0)
    {
        return true;
    }
    return memberwise_compare(lhs.get_products(), rhs.get_products()) < 0;
}

inline bool operator==(ReactionRule const& lhs, ReactionRule const& rhs)
{
    return lhs.get_reactants() == rhs.get_reactants() &&
            memberwise_compare(lhs.get_products(), rhs.get_products()) == 0;
}

inline bool operator!=(ReactionRule const& lhs, ReactionRule const& rhs)
{
    return !(lhs == rhs);
}

template<typename T2_>
inline ReactionRule new_reaction_rule(SpeciesTypeID const& r1, T2_ const& products, Real k)
{
    ReactionRule retval((ReactionRule::Reactants(r1)));
    retval.k() = k;
    std::for_each(boost::begin(products), boost::end(products),
            boost::bind(&ReactionRule::add_product, &retval, _1));
    return retval;
}

template<typename T2_>
inline ReactionRule new_reaction_rule(SpeciesTypeID const& r1, SpeciesTypeID const& r2, T2_ const& products, Real k)
{
    ReactionRule retval(ReactionRule::Reactants(r1, r2));
    retval.k() = k;
    std::for_each(boost::begin(products), boost::end(products),
            boost::bind(&ReactionRule::add_product, &retval, _1));
    return retval;
}

inline bool valid(ReactionRule const& r)
{
    return r.get_reactants().size() != 0;
}

template<typename Tchar_, typename Ttraits_>
inline std::basic_ostream<Tchar_, Ttraits_>&
operator<<(std::basic_ostream<Tchar_, Ttraits_>& out, ReactionRule const& r)
{
    bool first;
    out << "ReactionRule(reactants={";
    first = true;
    BOOST_FOREACH (SpeciesTypeID const& s, r.get_reactants())
    {
        if (!first)
        {
            out << ", ";
        }
        out << s;
        first = false;
    }
    out << "}, products={";
    first = true;
    BOOST_FOREACH (SpeciesTypeID const& s, r.get_products())
    {
        if (!first)
        {
            out << ", ";
        }
        out << s;
        first = false;
    }
    out << "})";
    return out;
}

#endif /* REACTION_RULE_HPP */
