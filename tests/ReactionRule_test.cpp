#define BOOST_TEST_MODULE "ReactionRule"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <boost/config.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include "SerialIDGenerator.hpp"
#include "ReactionRule.hpp"

BOOST_AUTO_TEST_CASE(ReactionRule_Reactants_iterator)
{
    SerialIDGenerator<SpeciesTypeID> gen;
    ReactionRule::Reactants r(gen(), gen());
    ReactionRule::Reactants::iterator b(r.begin());
    ReactionRule::Reactants::iterator e(r.end());
    BOOST_CHECK_EQUAL(e - b, 2);
    BOOST_CHECK(b < e);
}
