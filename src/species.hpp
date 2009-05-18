#ifndef SPECIES_HPP
#define SPECIES_HPP

#include <set>
#include <string>
#include <ostream>
#include <boost/range/iterator_range.hpp>

template<typename T_>
struct species
{
    typedef T_ length_type;

    std::string const& name() const
    {
        return name_;
    }

    length_type const& radius() const
    {
        return radius_;
    }

    double const& D() const
    {
        return diffusion_constant_;
    }

    bool operator==(species const& rhs) const
    {
        return name_ == rhs.name() &&
            diffusion_constant_ == rhs.D() && radius_ == rhs.radius();
    }

    bool operator!=(species const& rhs) const
    {
        return !operator==(rhs);
    }

    species(): diffusion_constant_(0), radius_(0.0) {}

    species(std::string const& name, double diffusion_constant, length_type const& radius)
        : name_(name), diffusion_constant_(diffusion_constant), radius_(radius) {}

private:
    const std::string name_;
    const double diffusion_constant_;
    const length_type radius_;
};

template<typename Tstrm_, typename T_>
inline std::basic_ostream<Tstrm_>& operator<<(std::basic_ostream<Tstrm_>& strm,
        const species<T_>& v)
{
    strm << "species(D=" << v.D() << ", radius=" << v.radius() << ")";
    return strm;
}

#endif /* SPECIES_HPP */
