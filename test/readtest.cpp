#include <iostream>
#include <string>

#include <catch2/catch.hpp>

#include <confini.h>
#include <confinitestfiles.h>

using namespace std;
using namespace confini;

TEST_CASE ( "Read Standard", "[ReadStandard]" )
{
    ConfIniFile<char> File1(TESTINI);

    REQUIRE ( std::string("Value1234") == File1("Section1", "Key1") );
    REQUIRE ( std::string("1234") == File1("Section1", "Key2") );
    REQUIRE ( std::string("Never change this!") == File1("Section1", "Key13") );

    REQUIRE ( std::string("1234Value") == File1("Section 2", "Key1") );
}

TEST_CASE ( "Read Empty", "[ReadEmpty]" )
{
    ConfIniFile<char> File1(TESTINI);

    REQUIRE ( std::string("") == File1("Section1", "Key3") );
    REQUIRE ( std::string("") == File1("Section  2", "Key1") );
}

TEST_CASE ( "Read Invalid Section", "[ReadInvalidSection]" )
{
    ConfIniFile<char> File1;
    REQUIRE_THROWS_MATCHES ( File1.setfile(INVALIDSECTIONINI), runtime_error,  Catch::Message("Invalid Section Line: [Section 2"));
}

TEST_CASE ( "Read Invalid Line", "[ReadInvalidLine]" )
{
    ConfIniFile<char> File1;
    REQUIRE_THROWS_MATCHES ( File1.setfile(INVALIDLINEINI), runtime_error,  Catch::Message("Invalid Line: Key1-1234Value"));
}

TEST_CASE ( "Read Duplicate Key", "[ReadDuplicateKey]" )
{
    ConfIniFile<char> File1;
    REQUIRE_THROWS_MATCHES ( File1.setfile(DUPLICATEKEYINI), runtime_error,  Catch::Message("Duplicate Key: [Section1] Key1"));
}

TEST_CASE ( "Read Duplicate Section", "[ReadDuplicateSection]" )
{
    ConfIniFile<char> File1;
    REQUIRE_THROWS_MATCHES ( File1.setfile(DUPLICATESECTIONINI), runtime_error,  Catch::Message("Duplicate Section: [Section1]"));
}

