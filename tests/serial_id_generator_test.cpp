#define BOOST_TEST_MODULE "serial_id_generator"
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include "serial_id_generator.hpp"
#include "particle_id.hpp"

BOOST_AUTO_TEST_CASE(with_int)
{
    serial_id_generator<int> s;
    BOOST_CHECK_EQUAL(s(), serial_add(int(0), 1));
    BOOST_CHECK_EQUAL(s(), serial_add(serial_add(int(0), 1), 1));
    BOOST_CHECK_EQUAL(s(), serial_add(serial_add(serial_add(int(0), 1), 1), 1));
}

BOOST_AUTO_TEST_CASE(with_particle_id)
{
    serial_id_generator<particle_id> s;
    BOOST_CHECK_EQUAL(s(), serial_add(particle_id(), 1));
    BOOST_CHECK_EQUAL(s(), serial_add(serial_add(particle_id(), 1), 1));
    BOOST_CHECK_EQUAL(s(), serial_add(serial_add(serial_add(particle_id(), 1), 1), 1));
}
