#define BOOST_TEST_MODULE "species"
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include "species.hpp"

BOOST_AUTO_TEST_CASE(basic)
{
    std::cout << species<double>("name", 0.4, 0.2) << std::endl;
    BOOST_CHECK_EQUAL(species<double>("name", 0.4, 0.2), species<double>("name", 0.4, 0.2));
    BOOST_CHECK(species<double>("name", 0.4, 0.2) != species<double>("eman", 0.4, 0.2));
}
