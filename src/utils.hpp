#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <vector>
#include <boost/range/size.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <algorithm>

#if defined(HAVE_UNORDERED_MAP)
#include <unordered_map>
#elif defined(HAVE_TR1_UNORDERED_MAP)
#include <tr1/unordered_map>
#elif defined(HAVE_BOOST_UNORDERED_MAP_HPP)
#include <boost/unordered_map.hpp>
#endif /* HAVE_UNORDERED_MAP */

namespace get_default_impl
{
    namespace std
    {
        template<typename Tkey_, typename Tval_>
        struct map
        {
            typedef ::std::map<Tkey_, Tval_> type;
        };

        template<typename Tkey_, typename Tval_>
        struct unordered_map
        {
#if defined(HAVE_UNORDERED_MAP)
            typedef ::std::unordered_map<Tkey_, Tval_> type;
#elif defined(HAVE_TR1_UNORDERED_MAP)
            typedef ::std::tr1::unordered_map<Tkey_, Tval_> type;
#elif defined(HAVE_BOOST_UNORDERED_MAP_HPP)
            typedef ::boost::unordered_map<Tkey_, Tval_> type;
#endif
        };

        template<typename Tval_>
        struct vector
        {
            typedef ::std::vector<Tval_> type; 
        };
    } // std
} // namespace get_default_impl

template < class T >
struct pointee_greater
{
    bool operator()( T x, T y ) const { return *y < *x; }
};


template < class T >
struct pointee_less
{
    bool operator()( T x, T y ) const { return *y > *x; }
};

template < typename T_ >
struct select_first
{
    typedef T_ argument_type;
    typedef typename T_::first_type result_type;

    typename T_::first_type& operator()( T_& pair ) const
    {
        return pair.first;
    }

    typename T_::first_type const& operator()( T_ const& pair ) const
    {
        return pair.first;
    }
};

template < typename T_ >
struct select_second
{
    typedef T_ argument_type;
    typedef typename T_::second_type result_type;

    typename T_::second_type& operator()( T_& pair ) const
    {
        return pair.second;
    }

    typename T_::second_type const& operator()( T_ const& pair ) const
    {
        return pair.second;
    }
};

namespace detail
{
    template < typename Tderived_, typename Tfun1_, typename Tfun2_,
               typename Tretval_ = typename Tfun1_::result_type >
    struct unary_compose_impl
    {
        typedef typename Tfun2_::argument_type argument_type;
        typedef typename Tfun1_::result_type result_type;

        unary_compose_impl( Tfun1_ const& f1, Tfun2_ const& f2 )
            : f1_( f1 ), f2_( f2 ) {}

        result_type operator()( argument_type const& val ) const
        {
            return f1_( f2_( val ) );
        }

        result_type operator()( argument_type const& val )
        {
            return f1_( f2_( val ) );
        }

        result_type operator()( argument_type& val ) const
        {
            return f1_( f2_( val ) );
        }

        result_type operator()( argument_type& val )
        {
            return f1_( f2_( val ) );
        }

    private:
        Tfun1_ f1_;
        Tfun2_ f2_;
    };

    template < typename Tderived_, typename Tfun1_, typename Tfun2_ >
    struct unary_compose_impl< Tderived_, Tfun1_, Tfun2_, void >
    {
        typedef typename Tfun2_::argument_type argument_type;
        typedef void result_type;

        unary_compose_impl( Tfun1_ const& f1, Tfun2_ const& f2 )
            : f1_( f1 ), f2_( f2 ) {}

        void operator()( argument_type const& val ) const
        {
            f1_( f2_( val ) );
        }

        void operator()( argument_type const& val )
        {
            f1_( f2_( val ) );
        }

        void operator()( argument_type& val ) const
        {
            f1_( f2_( val ) );
        }

        void operator()( argument_type& val )
        {
            f1_( f2_( val ) );
        }

    private:
        Tfun1_ f1_;
        Tfun2_ f2_;
    };

    template < typename Tderived_, typename Tfun1_, typename Tfun2_,
               typename Tretval_ = typename Tfun1_::result_type >
    struct binary_compose_impl
    {
        typedef typename Tfun2_::first_argument_type first_argument_type;
        typedef typename Tfun2_::second_argument_type second_argument_type;
        typedef typename Tfun1_::result_type result_type;

        binary_compose_impl( Tfun1_ const& f1, Tfun2_ const& f2 )
            : f1_( f1 ), f2_( f2 ) {}

        result_type operator()( first_argument_type const& v1,
                                second_argument_type const& v2 ) const
        {
            return f1_( f2_( v1, v2 ) );
        }

        result_type operator()( first_argument_type const& v1,
                                second_argument_type const& v2 )
        {
            return f1_( f2_( v1, v2 ) );
        }

        result_type operator()( first_argument_type& v1,
                                second_argument_type& v2 ) const
        {
            return f1_( f2_( v1, v2 ) );
        }

        result_type operator()( first_argument_type& v1,
                                second_argument_type& v2 )
        {
            return f1_( f2_( v1, v2 ) );
        }

    private:
        Tfun1_ f1_;
        Tfun2_ f2_;
    };

    template < typename Tderived_, typename Tfun1_, typename Tfun2_ >
    struct binary_compose_impl< Tderived_, Tfun1_, Tfun2_, void >
    {
        typedef typename Tfun2_::first_argument_type first_argument_type;
        typedef typename Tfun2_::second_argument_type second_argument_type;
        typedef void result_type;

        binary_compose_impl( Tfun1_ const& f1, Tfun2_ const& f2 )
            : f1_( f1 ), f2_( f2 ) {}

        void operator()( first_argument_type const& v1,
                                second_argument_type const& v2 ) const
        {
            f1_( f2_( v1, v2 ) );
        }

        void operator()( first_argument_type const& v1,
                                second_argument_type const& v2 )
        {
            f1_( f2_( v1, v2 ) );
        }

        void operator()( first_argument_type& v1,
                                second_argument_type& v2 ) const
        {
            f1_( f2_( v1, v2 ) );
        }

        void operator()( first_argument_type& v1,
                                second_argument_type& v2 )
        {
            f1_( f2_( v1, v2 ) );
        }

    private:
        Tfun1_ f1_;
        Tfun2_ f2_;
    };
} // namespace detail

template < typename Tfun1_, typename Tfun2_ >
struct unary_compose
    : public detail::unary_compose_impl<unary_compose< Tfun1_, Tfun2_ >,
                                              Tfun1_, Tfun2_ >
{
public:
    unary_compose( Tfun1_ const& f1, Tfun2_ const& f2 )
        : detail::unary_compose_impl< unary_compose, Tfun1_, Tfun2_ >( f1, f2 ) {}
};

template < typename Tfun1_, typename Tfun2_ >
inline unary_compose< Tfun1_, Tfun2_ >
compose_unary( Tfun1_ const& f1, Tfun2_ const& f2 )
{
    return unary_compose< Tfun1_, Tfun2_ >( f1, f2 );
}

template < typename Tfun1_, typename Tfun2_ >
struct binary_compose
    : public detail::binary_compose_impl<binary_compose< Tfun1_, Tfun2_ >,
                                              Tfun1_, Tfun2_ >
{
public:
    binary_compose( Tfun1_ const& f1, Tfun2_ const& f2 )
        : detail::binary_compose_impl< binary_compose, Tfun1_, Tfun2_ >( f1, f2 ) {}
};

template < typename Tfun1_, typename Tfun2_ >
inline binary_compose< Tfun1_, Tfun2_ >
compose_binary( Tfun1_ const& f1, Tfun2_ const& f2 )
{
    return binary_compose< Tfun1_, Tfun2_ >( f1, f2 );
}

template < typename T_ >
struct delete_ptr
{
    typedef void result_type;
    typedef T_* argument_type;

    void operator()( T_* ptr )
    {
        delete ptr;
    }
};

template<typename T_, typename Targ_>
struct reinterpret_caster
{
    T_ operator()(Targ_ const& v)
    {
        return reinterpret_cast<T_>(v);
    }
};

template<typename T_, typename Targ_>
struct reinterpret_caster<T_&, Targ_&>
{
    T_& operator()(Targ_& v)
    {
        return reinterpret_cast<T_&>(v);
    }
};

template<typename T_, typename Targ_>
inline T_ reinterpret_cast_wrapper(Targ_ const& v, typename boost::disable_if<boost::is_reference<T_> >::type* = 0)
{
    return reinterpret_caster<T_, Targ_>()(v);
}

template<typename T_, typename Targ_>
inline T_& reinterpret_cast_wrapper(Targ_& v)
{
    return reinterpret_caster<T_&, Targ_&>()(v);
}

template<typename T_, typename Targ_>
struct dynamic_caster
{
    T_ operator()(Targ_ const& v)
    {
        return dynamic_cast<T_>(v);
    }
};

template<typename T_, typename Targ_>
struct dynamic_caster<T_&, Targ_&>
{
    T_& operator()(Targ_& v)
    {
        return dynamic_cast<T_&>(v);
    }
};

template<typename T_, typename Targ_>
inline T_ dynamic_cast_wrapper(Targ_ const& v, typename boost::disable_if<boost::is_reference<T_> >::type* = 0)
{
    return dynamic_caster<T_, Targ_>()(v);
}

template<typename T_, typename Targ_>
inline T_& dynamic_cast_wrapper(Targ_& v)
{
    return dynamic_caster<T_&, Targ_&>()(v);
}

template<typename Tlhs_, typename Trhs_>
inline int memberwise_compare(Tlhs_ const& lhs, Trhs_ const& rhs)
{
    if (boost::size(lhs) <= boost::size(rhs))
    {
        std::pair<typename Tlhs_::iterator, typename Trhs_::iterator> pair(
            std::mismatch(lhs.begin(), lhs.end(), rhs.begin()));
        if (pair.first == lhs.end())
            return boost::size(lhs) - boost::size(rhs);
        return *pair.first < *pair.second ?  -1:
                *pair.first > *pair.second ? 1: 0;
    }
    else if (boost::size(lhs) > boost::size(rhs))
    {
        std::pair<typename Trhs_::iterator, typename Tlhs_::iterator> pair(
            std::mismatch(rhs.begin(), rhs.end(), lhs.begin()));
        if (pair.first == rhs.end())
            return 1;
        return *pair.first < *pair.second ? 1:
                *pair.first > *pair.second ? -1: 0;
    }
    return 0;
}

template<typename Trange_, typename Tfun_>
struct get_transformed_range
{
    typedef typename boost::range_iterator<Trange_>::type iterator_type;
    typedef boost::transform_iterator<Tfun_, iterator_type> transform_iterator_type;
    typedef boost::iterator_range<transform_iterator_type> type;
};

template<typename Trange_, typename Tfun_>
struct get_transformed_range<const Trange_, Tfun_>
{
    typedef typename boost::range_const_iterator<Trange_>::type iterator_type;
    typedef boost::transform_iterator<Tfun_, iterator_type> transform_iterator_type;
    typedef boost::iterator_range<transform_iterator_type> type;
};

template<typename T_>
struct get_select_first_iterator
{
    typedef boost::transform_iterator<
        select_first<typename T_::value_type>, T_> type;
};

template<typename T_>
struct get_select_second_iterator
{
    typedef boost::transform_iterator<
        select_second<typename T_::value_type>, T_> type;
};

template<typename T_>
inline typename get_select_first_iterator<T_>::type
make_select_first_iterator(T_ const& iter)
{
    return typename get_select_first_iterator<T_>::type(iter,
        select_first<typename T_::value_type>());
        
}

template<typename T_>
inline typename get_select_second_iterator<T_>::type
make_select_second_iterator(T_ const& iter)
{
    return typename get_select_second_iterator<T_>::type(iter,
        select_second<typename T_::value_type>());
}

namespace detail {

template<typename Trange_>
struct select_first_range_impl
{
    typedef boost::iterator_range<typename get_select_first_iterator<
        typename boost::range_iterator<Trange_>::type >::type> type;
};

template<typename Trange_>
struct select_second_range_impl
{
    typedef boost::iterator_range<typename get_select_second_iterator<
        typename boost::range_iterator<Trange_>::type >::type> type;
};

template<typename Trange_>
struct select_first_range_impl<const Trange_>
{
    typedef boost::iterator_range<typename get_select_first_iterator<
        typename boost::range_const_iterator<Trange_>::type >::type> type;
};

template<typename Trange_>
struct select_second_range_impl<const Trange_>
{
    typedef boost::iterator_range<typename get_select_second_iterator<
        typename boost::range_const_iterator<Trange_>::type >::type> type;
};

} // namespace detail

template<typename Trange_>
struct select_first_range: public detail::select_first_range_impl<Trange_>::type
{
    select_first_range(Trange_ const& range)
        : detail::select_first_range_impl<Trange_>::type(
                boost::begin(range), boost::end(range)) {}
};

template<typename Trange_>
struct select_second_range: public detail::select_second_range_impl<Trange_>::type
{
    select_second_range(Trange_ const& range)
        : detail::select_second_range_impl<Trange_>::type(
                boost::begin(range), boost::end(range)) {}
};

template<typename Trange_>
inline select_first_range<Trange_>
make_select_first_range(Trange_ const& range)
{
    return select_first_range<Trange_>(range);
}

template<typename Trange_>
inline select_second_range<Trange_>
make_select_second_range(Trange_ const& range)
{
    return select_second_range<Trange_>(range);
}

template<typename Trange_>
struct range_iterator_category
    : public boost::iterator_category<typename boost::range_iterator<Trange_>::type>
{
};

template<typename Trange_, typename Ticat_>
struct check_range_iterator_category
    : public boost::is_same<
        typename range_iterator_category<Trange_>::type, Ticat_> {};

template<typename T_>
class reference_or_instance
{
public:
    reference_or_instance(T_& r): type_(REF)
    {
        ref_ = &r;
    }

    reference_or_instance(T_ const& i, int)
        : type_(INSTANCE), ref_(new (instance_) T_(i)) {}

    reference_or_instance(): type_(INSTANCE), ref_(new (instance_) T_()) {}

    reference_or_instance(reference_or_instance const& that): type_(that.type_)
    {
        switch (type_)
        {
        case REF:
            ref_ = that.ref_;
            break;
        case INSTANCE:
            ref_ = new (instance_) T_(static_cast<T_ const&>(that));
            break;
        }
    }

    ~reference_or_instance()
    {
        if (type_ == INSTANCE)
        {
            ref_->~T_();
        }
    }

    operator T_&()
    {
        return *ref_;
    }

    operator T_ const&() const
    {
        return *ref_;
    }

private:
    enum {
        REF,
        INSTANCE
    } type_;

    char instance_[sizeof(T_)];
    T_* ref_;
};

template<typename Trange_, typename Tfun_>
inline boost::iterator_range<
    boost::transform_iterator<
        Tfun_, typename boost::range_const_iterator<Trange_>::type> >
make_transform_iterator_range(Trange_ const& range, Tfun_ const& fun)
{
    typedef boost::transform_iterator<Tfun_,
        typename boost::range_const_iterator<Trange_>::type> transform_iterator;

    return boost::iterator_range<transform_iterator>(
        transform_iterator(boost::begin(range), fun),
        transform_iterator(boost::end(range), fun));
}

template<typename Trange_, typename Tfun_>
inline boost::iterator_range<
    boost::transform_iterator<
        Tfun_, typename boost::range_iterator<Trange_>::type> >
make_transform_iterator_range(Trange_& range, Tfun_ const& fun)
{
    typedef boost::transform_iterator<Tfun_,
        typename boost::range_iterator<Trange_>::type> transform_iterator;

    return boost::iterator_range<transform_iterator>(
        transform_iterator(boost::begin(range), fun),
        transform_iterator(boost::end(range), fun));
}

void gsl_error_handler( char const* reason, char const* file, int line, int gsl_errno );

#endif /* UTILS_HPP */
