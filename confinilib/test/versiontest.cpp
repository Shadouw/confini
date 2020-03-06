#include <confini.h>
#include <catch2/catch.hpp>

TEST_CASE ( "Check Version", "[CheckVersion]" )
{
    REQUIRE ( std::string("0.0.1") == std::string(confini::version()) );
}


