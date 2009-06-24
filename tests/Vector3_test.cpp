#define BOOST_TEST_MODULE "Vector3"

#include <boost/config.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/mpl/list.hpp>

#include "Vector3.hpp"

typedef boost::mpl::list3<long double, double, float> scalar_types;

template<typename T_>
struct get_tolerance
{
};

template<>
struct get_tolerance<float>
{
    BOOST_STATIC_CONSTANT(float, value = 1e-8);
};

template<>
struct get_tolerance<double>
{
    BOOST_STATIC_CONSTANT(double, value = 1e-17);
};

template<>
struct get_tolerance<long double>
{
    BOOST_STATIC_CONSTANT(long double, value = 1e-26);
};

BOOST_AUTO_TEST_CASE_TEMPLATE(distance_sq, T_, scalar_types)
{
    typedef ::Vector3<T_> Vector3;

    BOOST_CHECK_CLOSE((T_)3.0f,
            Vector3(1, 1, 1).distance_sq(Vector3(0, 0, 0)),
            get_tolerance<T_>::value);
    BOOST_CHECK_CLOSE((T_)3.0f,
            Vector3(1, 2, 1).distance_sq(Vector3(0, 1, 0)),
            get_tolerance<T_>::value);
    BOOST_CHECK_CLOSE((T_)0.75f,
            Vector3(0.5, 0.5, 1).distance_sq(Vector3(0, 1, 0.5)),
            get_tolerance<T_>::value);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(add_op, T_, scalar_types)
{
    typedef ::Vector3<T_> Vector3;

    BOOST_CHECK_EQUAL(Vector3(1, 1, 1),
            Vector3(1, 1, 1) + Vector3(0, 0, 0));
    BOOST_CHECK_EQUAL(Vector3(1, 3, 1),
            Vector3(1, 2, 1) + Vector3(0, 1, 0));
    BOOST_CHECK_EQUAL(Vector3(0.5, 1.5, 1.5),
            Vector3(0.5, 0.5, 1) + Vector3(0, 1, 0.5));
}

