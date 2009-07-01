#define BOOST_TEST_MODULE "SerialIDGenerator"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include "SerialIDGenerator.hpp"
#include "ParticleID.hpp"

BOOST_AUTO_TEST_CASE(with_int)
{
    SerialIDGenerator<int> s;
    BOOST_CHECK_EQUAL(s(), serial_add(int(0), 1));
    BOOST_CHECK_EQUAL(s(), serial_add(serial_add(int(0), 1), 1));
    BOOST_CHECK_EQUAL(s(), serial_add(serial_add(serial_add(int(0), 1), 1), 1));
}

BOOST_AUTO_TEST_CASE(with_ParticleID)
{
    SerialIDGenerator<ParticleID> s;
    BOOST_CHECK_EQUAL(s(), serial_add(ParticleID(), 1));
    BOOST_CHECK_EQUAL(s(), serial_add(serial_add(ParticleID(), 1), 1));
    BOOST_CHECK_EQUAL(s(), serial_add(serial_add(serial_add(ParticleID(), 1), 1), 1));
}
