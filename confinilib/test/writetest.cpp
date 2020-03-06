#include <iostream>
#include <string>

#include <catch2/catch.hpp>
#include <boost/filesystem.hpp>

#include <confini.h>
#include <confinitestfiles.h>

using namespace std;
using namespace confini;

TEST_CASE ( "Make Changes", "[MakeChanges]" )
{
    boost::filesystem::copy_file(TESTINI, TEST2INI, boost::filesystem::copy_option::overwrite_if_exists );
    ConfIniFile<char> File1(TEST2INI);

    // Change a value:
    File1("Section1", "Key2") = "Value";
    // Add a value
    File1("Section1", "Key3") = "NewValue";
    // Add a section/value
    File1("Section-3", "Key1") = "New Section & Value";
    // Add two section with multiple values
    File1("Section-4", "Key1") = "New Section & Value";
    File1("Section-4", "Key2") = "New Section & Value";
    File1("Section-4", "Key3") = "New Section & Value";
    File1("Section-5", "Key1") = "New Section & Value";
    File1("Section-5", "Key2") = "New Section & Value";
    File1("Section-5", "Key3") = "New Section & Value";
    // Remove a value
    File1("Section1", "Key1") = "";

    File1.savefile();
}

TEST_CASE ( "Test Changes", "[TestChanges]" )
{
    ConfIniFile<char> File1(TEST2INI);

    REQUIRE ( std::string("Value") == File1("Section1", "Key2") );
    REQUIRE ( std::string("NewValue") == File1("Section1", "Key3") );
    REQUIRE ( std::string("Never change this!") == File1("Section1", "Key13") );
    REQUIRE ( std::string("New Section & Value") == File1("Section-3", "Key1") );
    REQUIRE ( std::string("New Section & Value") == File1("Section-4", "Key1") );
    REQUIRE ( std::string("New Section & Value") == File1("Section-4", "Key2") );
    REQUIRE ( std::string("New Section & Value") == File1("Section-4", "Key3") );
    REQUIRE ( std::string("New Section & Value") == File1("Section-5", "Key1") );
    REQUIRE ( std::string("New Section & Value") == File1("Section-5", "Key2") );
    REQUIRE ( std::string("New Section & Value") == File1("Section-5", "Key3") );
    REQUIRE ( std::string("") == File1("Section1", "Key1") );

    boost::filesystem::remove(TEST2INI);
}
