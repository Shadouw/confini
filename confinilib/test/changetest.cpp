#include <iostream>
#include <string>

#include <catch2/catch.hpp>

#include <confini.h>
#include <confinitestfiles.h>

using namespace std;
using namespace confini;

TEST_CASE ( "Change Existing Value", "[ChangeExistingValue]" )
{
    ConfIniFile<char> File1(TESTINI);

    // First read it
    REQUIRE ( std::string("1234") == File1("Section1", "Key2") );

    // Now Change it
    File1("Section1", "Key2") = "Value";
    // Test again
    REQUIRE ( std::string("Value") == File1("Section1", "Key2") );
}

TEST_CASE ( "Add New Value", "[AddNewValue]" )
{
    ConfIniFile<char> File1(TESTINI);

    // First read it
    REQUIRE ( std::string("") == File1("Section1", "Key3") );

    // Now Change it
    File1("Section1", "Key3") = "NewValue";
    // Test again
    REQUIRE ( std::string("NewValue") == File1("Section1", "Key3") );
}

TEST_CASE ( "Add New Section", "[AddNewSection]" )
{
    ConfIniFile<char> File1(TESTINI);

    // First read it
    REQUIRE ( std::string("") == File1("Section-3", "Key1") );

    // Now Change it
    File1("Section-3", "Key1") = "New Section & Value";
    // Test again
    REQUIRE ( std::string("New Section & Value") == File1("Section-3", "Key1") );
}

TEST_CASE ( "Remove Value", "[RemoveValue]" )
{
    ConfIniFile<char> File1(TESTINI);

    // First read it
    REQUIRE ( std::string("1234") == File1("Section1", "Key2") );

    // Now Change it
    File1("Section1", "Key2") = "";
    // Test again
    REQUIRE ( std::string("") == File1("Section1", "Key2") );
}
