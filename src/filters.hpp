#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <functional>
#include <cmath>

template<typename Toc_, typename Tfun_, typename Tsphere_>
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
    typedef Tsphere_ sphere_type;

public:
    inline neighbor_filter(Tfun_& next, const sphere_type& cmp)
        : next_(next), cmp_(cmp) {}

    inline result_type operator()(argument_type i) const
    {
        typename argument_type::reference item(*i);

/* This is considered harmful
        if (cmp_ == item.second)
        {
            return;
        }
*/
        const typename sphere_type::length_type dist(
            distance(cmp_.position(), item.second.position())
            - item.second.radius());
        if (dist < cmp_.radius())
        {
            next_(i, dist);
        }
    }

private:
    Tfun_& next_;
    const sphere_type cmp_;
};

template<typename Toc_, typename Tfun_, typename Tsphere_>
inline void take_neighbor(Toc_& oc, Tfun_& fun,
        const Tsphere_& cmp)
{
    oc.each_neighbor(oc.index(cmp.position()),
                     neighbor_filter<Toc_, Tfun_, Tsphere_>(fun, cmp));
}

template<typename Toc_, typename Tfun_, typename Tsphere_>
inline void take_neighbor(Toc_ const& oc, Tfun_& fun,
        const Tsphere_& cmp)
{
    oc.each_neighbor(oc.index(cmp.position()),
                     neighbor_filter<Toc_ const, Tfun_, Tsphere_>(fun, cmp));
}

template<typename Toc_, typename Tfun_, typename Tsphere_>
class cyclic_neighbor_filter
        : public std::binary_function<
            typename boost::mpl::if_<
                boost::is_const<Toc_>,
                typename Toc_::const_iterator,
                typename Toc_::iterator>::type,
            const typename Toc_::position_type&,
            void>
{
    typedef typename boost::mpl::if_<
            boost::is_const<Toc_>,
            typename Toc_::const_iterator,
            typename Toc_::iterator>::type first_argument_type;
    typedef const typename Toc_::position_type& second_argument_type;
    typedef void result_type;
    typedef Tsphere_ sphere_type;

public:
    inline cyclic_neighbor_filter(Tfun_& next,
            const sphere_type& cmp)
        : next_(next), cmp_(cmp) {}

    inline result_type operator()(first_argument_type i,
            second_argument_type p) const
    {
        typename first_argument_type::reference item(*i);

/* XXX: This is considered harmful
        if (cmp_ == item.second)
        {
            return;
        }
*/
        const typename sphere_type::length_type dist(
            distance(cmp_.position(), item.second.position() + p)
            - item.second.radius());
        if (dist < cmp_.radius())
        {
            next_(i, dist);
        }
    }

private:
    Tfun_& next_;
    const sphere_type cmp_;
};

template<typename Toc_, typename Tfun_, typename Tsphere_>
inline void take_neighbor_cyclic(Toc_& oc, Tfun_& fun,
         const Tsphere_& cmp)
{
    oc.each_neighbor_cyclic(oc.index(cmp.position()),
            cyclic_neighbor_filter<Toc_, Tfun_, Tsphere_>(fun, cmp));
}

template<typename Toc_, typename Tfun_, typename Tsphere_>
inline void take_neighbor_cyclic(Toc_ const& oc, Tfun_& fun,
         const Tsphere_& cmp)
{
    oc.each_neighbor_cyclic(oc.index(cmp.position()),
            cyclic_neighbor_filter<Toc_ const, Tfun_, Tsphere_>(fun, cmp));
}

#endif /* ALGORITHM_HPP */
