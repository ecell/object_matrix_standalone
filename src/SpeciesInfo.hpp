#ifndef SPECIES_HPP
#define SPECIES_HPP

#include <set>
#include <string>
#include <ostream>
#include "Defs.hpp"

template<typename Tid_, typename T_>
struct SpeciesInfo
{
    typedef Tid_ identifier_type;
    typedef T_ length_type;

    identifier_type const& id() const
    {
        return id_;
    }

    length_type const& radius() const
    {
        return radius_;
    }

    length_type& radius()
    {
        return radius_;
    }

    Real const& D() const
    {
        return diffusion_coef_;
    }

    Real& D()
    {
        return diffusion_coef_;
    }

    bool operator==(SpeciesInfo const& rhs) const
    {
        return id == rhs.id() && diffusion_coef_ == rhs.D() &&
                radius_ == rhs.radius();
    }

    bool operator!=(SpeciesInfo const& rhs) const
    {
        return !operator==(rhs);
    }

    SpeciesInfo(identifier_type const& id, Real const& D = 0.,
            length_type const& r = 0.) 
        : id_(id), diffusion_coef_(D), radius_(r) {}

    SpeciesInfo() {}

private:
    identifier_type id_;
    Real diffusion_coef_;
    length_type radius_;
};

template<typename Tchar_, typename Ttraits_, typename Tid_, typename T_>
inline std::basic_ostream<Tchar_, Ttraits_>&
operator<<(std::basic_ostream<Tchar_, Ttraits_>& strm, const SpeciesInfo<Tid_, T_>& v)
{
    strm << "SpeciesInfo(id=" << v.id() << ", D=" << v.D() << ", radius=" << v.radius() << ")";
    return strm;
}

#endif /* SPECIES_HPP */
