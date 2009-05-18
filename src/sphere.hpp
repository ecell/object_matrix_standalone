#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <ostream>
#include "position.hpp"

template<typename T_>
struct sphere
{
    typedef T_ value_type;
    typedef ::position<T_> position_type;
    typedef T_ length_type;

    sphere()
        : position_(), radius_(0) {}

    sphere(const position_type& position, const length_type& radius)
        : position_(position), radius_(radius) {}

    position_type& position()
    {
        return position_;
    }

    position_type const& position() const 
    {
        return position_;
    }

    length_type& radius()
    {
        return radius_;
    }

    length_type const& radius() const 
    {
        return radius_;
    }

    bool operator==(const sphere& rhs) const
    {
        return position_ == rhs.position() && radius_ == rhs.radius();
    }

    bool operator!=(const sphere& rhs) const
    {
        return !operator==(rhs);
    }

private:
    position_type position_;
    length_type radius_;
};

template<typename Tstrm_, typename T_>
inline std::basic_ostream<Tstrm_>& operator<<(std::basic_ostream<Tstrm_>& strm,
        const sphere<T_>& v)
{
    strm << "{" << v.position() <<  ", " << v.radius() << "}";
    return strm;
}

#endif /* SPHERE_HPP */
