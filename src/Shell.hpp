#ifndef SHELL_HPP
#define SHELL_HPP

#include "sphere.hpp"

template<typename T_>
struct shell
{
    typedef sphere<T_> sphere_type;
    typedef typename sphere_type::position_type position_type;
    typedef typename sphere_type::length_type length_type;

    shell(): sphere_() {}

    shell(sphere_type const& sphere)
        : sphere_(sphere) {}

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

private:
    sphere_type sphere_;
};

#endif /* SHELL_HPP */
