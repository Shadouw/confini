#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>

#include <confini.h>

using namespace std;
using namespace confini;

BOOST_AUTO_TEST_CASE ( ReadStandard )
{
    ConfIniFile<char> File1("../../confinilib/test/test.ini");

    BOOST_TEST ( std::string("Value1234") == File1("Section1", "Key1") );
    BOOST_TEST ( std::string("1234") == File1("Section1", "Key2") );
    BOOST_TEST ( std::string("Never change this!") == File1("Section1", "Key13") );

    BOOST_TEST ( std::string("1234Value") == File1("Section 2", "Key1") );
}

BOOST_AUTO_TEST_CASE ( ReadEmpty )
{
    ConfIniFile<char> File1("../../confinilib/test/test.ini");

    BOOST_TEST ( std::string("") == File1("Section1", "Key3") );
    BOOST_TEST ( std::string("") == File1("Section  2", "Key1") );
}

BOOST_AUTO_TEST_CASE ( ReadInvalidSection )
{
    ConfIniFile<char> File1;
    BOOST_REQUIRE_THROW ( File1.setfile("../../confinilib/test/InvalidSection.ini"), runtime_error );

    try
    {
        File1.setfile("../../confinilib/test/InvalidSection.ini");
    }
    catch ( const runtime_error &ex )
    {
        BOOST_TEST ( std::string("Invalid Section Line: [Section 2") == ex.what() );

    }
}

BOOST_AUTO_TEST_CASE ( ReadInvalidLine )
{
    ConfIniFile<char> File1;
    BOOST_REQUIRE_THROW ( File1.setfile("../../confinilib/test/InvalidLine.ini"), runtime_error );

    try
    {
        File1.setfile("../../confinilib/test/InvalidLine.ini");
    }
    catch ( const runtime_error &ex )
    {
        BOOST_TEST ( std::string("Invalid Line: Key1-1234Value") == ex.what() );

    }
}

BOOST_AUTO_TEST_CASE ( ReadDuplicateKey )
{
    ConfIniFile<char> File1;
    BOOST_REQUIRE_THROW ( File1.setfile("../../confinilib/test/DuplicateKey.ini"), runtime_error );

    try
    {
        File1.setfile("../../confinilib/test/DuplicateKey.ini");
    }
    catch ( const runtime_error &ex )
    {
        BOOST_TEST ( std::string("Duplicate Key: [Section1] Key1") == ex.what() );

    }
}

BOOST_AUTO_TEST_CASE ( ReadDuplicateSection )
{
    ConfIniFile<char> File1;
    BOOST_REQUIRE_THROW ( File1.setfile("../../confinilib/test/DuplicateSection.ini"), runtime_error );

    try
    {
        File1.setfile("../../confinilib/test/DuplicateSection.ini");
    }
    catch ( const runtime_error &ex )
    {
        BOOST_TEST ( std::string("Duplicate Section: [Section1]") == ex.what() );

    }
}

