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

    bool operator==(const Sphere& rhs) const
    {
        return position_ == rhs.position_ && radius_ == rhs.radius_;
    }

    bool operator!=(const Sphere& rhs) const
    {
        return !operator==(rhs);
    }

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

template<typename Tstrm_, typename T_>
inline std::basic_ostream<Tstrm_>& operator<<(std::basic_ostream<Tstrm_>& strm,
        const Sphere<T_>& v)
{
    strm << "{" << v.position() <<  ", " << v.radius() << "}";
    return strm;
}

#endif /* SPHERE_HPP */
