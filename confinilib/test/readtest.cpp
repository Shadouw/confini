#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>

#include <confini.h>
#include <confinitestfiles.h>

using namespace std;
using namespace confini;

BOOST_AUTO_TEST_CASE ( ReadStandard )
{
    ConfIniFile<char> File1(TESTINI);

    BOOST_TEST ( std::string("Value1234") == File1("Section1", "Key1") );
    BOOST_TEST ( std::string("1234") == File1("Section1", "Key2") );
    BOOST_TEST ( std::string("Never change this!") == File1("Section1", "Key13") );

    BOOST_TEST ( std::string("1234Value") == File1("Section 2", "Key1") );
}

BOOST_AUTO_TEST_CASE ( ReadEmpty )
{
    ConfIniFile<char> File1(TESTINI);

    BOOST_TEST ( std::string("") == File1("Section1", "Key3") );
    BOOST_TEST ( std::string("") == File1("Section  2", "Key1") );
}

BOOST_AUTO_TEST_CASE ( ReadInvalidSection )
{
    ConfIniFile<char> File1;
    BOOST_REQUIRE_THROW ( File1.setfile(INVALIDSECTIONINI), runtime_error );

    try
    {
        File1.setfile(INVALIDSECTIONINI);
    }
    catch ( const runtime_error &ex )
    {
        BOOST_TEST ( std::string("Invalid Section Line: [Section 2") == ex.what() );

    }
}

BOOST_AUTO_TEST_CASE ( ReadInvalidLine )
{
    ConfIniFile<char> File1;
    BOOST_REQUIRE_THROW ( File1.setfile(INVALIDLINEINI), runtime_error );

    try
    {
        File1.setfile(INVALIDLINEINI);
    }
    catch ( const runtime_error &ex )
    {
        BOOST_TEST ( std::string("Invalid Line: Key1-1234Value") == ex.what() );

    }
}

BOOST_AUTO_TEST_CASE ( ReadDuplicateKey )
{
    ConfIniFile<char> File1;
    BOOST_REQUIRE_THROW ( File1.setfile(DUPLICATEKEYINI), runtime_error );

    try
    {
        File1.setfile(DUPLICATEKEYINI);
    }
    catch ( const runtime_error &ex )
    {
        BOOST_TEST ( std::string("Duplicate Key: [Section1] Key1") == ex.what() );

    }
}

BOOST_AUTO_TEST_CASE ( ReadDuplicateSection )
{
    ConfIniFile<char> File1;
    BOOST_REQUIRE_THROW ( File1.setfile(DUPLICATESECTIONINI), runtime_error );

    try
    {
        File1.setfile(DUPLICATESECTIONINI);
    }
    catch ( const runtime_error &ex )
    {
        BOOST_TEST ( std::string("Duplicate Section: [Section1]") == ex.what() );

    }
}

