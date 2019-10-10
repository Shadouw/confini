#include <confini.h>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE ( check_version )
{
    BOOST_REQUIRE_EQUAL ( "0.0.1", confini::version() );
}


