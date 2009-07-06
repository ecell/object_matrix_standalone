#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <ostream>
#include "Vector3.hpp"

template<typename T_>
class Sphere
{
public:
    typedef T_ value_type;
    typedef Vector3<T_> position_type;
    typedef T_ length_type;

public:
    Sphere()
        : position_(), radius_(0) {}

    Sphere(const position_type& position, const length_type& radius)
        : position_(position), radius_(radius) {}

    position_type const& position() const
    {
        return position_;
    }

    position_type& position()
    {
        return position_;
    }

    length_type const& radius() const
    {
        return radius_;
    }

    length_type& radius()
    {
        return radius_;
    }

private:
    position_type position_;
    length_type radius_;
};

template<typename T_, typename Trhs_>
inline bool operator==(Sphere<T_> const& lhs, Trhs_ const& rhs)
{
    return lhs.position() == rhs.position() &&
            lhs.radius() == rhs.radius();
}

template<typename T_, typename Trhs_>
inline bool operator!=(Sphere<T_> const& lhs, Trhs_ const& rhs)
{
    return !operator==(lhs, rhs);
}

template<typename Tstrm_, typename T_>
inline std::basic_ostream<Tstrm_>& operator<<(std::basic_ostream<Tstrm_>& strm,
        const Sphere<T_>& v)
{
    strm << "{" << v.position() <<  ", " << v.radius() << "}";
    return strm;
}

#endif /* SPHERE_HPP */
