#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <functional>
#include <cmath>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>
#include "Vector3.hpp"
#include "Sphere.hpp"

template<typename Toc_, typename Tfun_>
class neighbor_filter
        : public std::unary_function<
            typename boost::mpl::if_<
                boost::is_const<Toc_>,
                typename Toc_::const_iterator,
                typename Toc_::iterator>::type, void>
{
    typedef typename boost::mpl::if_<
            boost::is_const<Toc_>,
            typename Toc_::const_iterator,
            typename Toc_::iterator>::type argument_type;
    typedef void result_type;

    typedef const ::Sphere< typename Toc_::mapped_type::length_type > sphere_type;

public:
    inline neighbor_filter(Tfun_& next, const sphere_type& cmp)
        : next_(next), cmp_(cmp) {}

    inline void operator()(argument_type const& i) const
    {
        typename argument_type::reference item(*i);

        if (cmp_.position() == item.second.position() &&
            cmp_.radius() == item.second.radius())
        {
            return;
        }

        const double dist(
            distance(cmp_.position(), item.second.position())
            - item.second.radius());
        if (dist < cmp_.radius())
        {
            next_(i, dist);
        }
    }

private:
    Tfun_& next_;
    sphere_type cmp_;
};

template<typename Toc_, typename Tfun_>
inline void take_neighbor(Toc_& oc, Tfun_& fun,
        const ::Sphere< typename Toc_::mapped_type::length_type >& cmp)
{
    oc.each_neighbor(oc.index(cmp.position()),
                     neighbor_filter<Toc_, Tfun_>(fun, cmp));
}

template<typename Toc_, typename Tfun_>
class cyclic_neighbor_filter
        : public std::binary_function<
            typename Toc_::reference,
            const typename Toc_::position_type&,
            void>
{
    typedef typename Toc_::iterator first_argument_type;
    typedef const typename Toc_::position_type& second_argument_type;
    typedef void result_type;
    typedef const ::Sphere< typename Toc_::mapped_type::length_type > sphere_type;

public:
    inline cyclic_neighbor_filter(Tfun_& next,
            const sphere_type& cmp)
        : next_(next), cmp_(cmp) {}

    inline result_type operator()(first_argument_type i,
            second_argument_type p) const
    {
        typename first_argument_type::reference item(*i);

        if (cmp_.position() == item.second.position() &&
            cmp_.radius() == item.second.radius())
        {
            return;
        }

        const double dist(
            distance(cmp_.position(), item.second.position() + p)
            - item.second.radius());
        if (dist < cmp_.radius())
        {
            next_(i, dist);
        }
    }

private:
    Tfun_& next_;
    sphere_type cmp_;
};

template<typename Toc_, typename Tfun_>
inline void take_neighbor_cyclic(Toc_& oc, Tfun_& fun,
         const ::Sphere< typename Toc_::mapped_type::length_type >& cmp)
{
    oc.each_neighbor_cyclic(oc.index(cmp.position()),
            cyclic_neighbor_filter<Toc_, Tfun_>(fun, cmp));
}

#endif /* ALGORITHM_HPP */
