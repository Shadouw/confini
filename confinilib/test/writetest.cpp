#include <iostream>
#include <fstream>
#include <string>

#include <catch2/catch.hpp>

#include <confini.h>
#include <confinitestfiles.h>

#include <stdio.h>

using namespace std;
using namespace confini;

TEST_CASE ( "Make Changes", "[MakeChanges]" )
{
    std::ifstream src ( TESTINI,  std::ios::binary );
    std::ofstream out ( TEST2INI, std::ios::binary );
    out << src.rdbuf();
    out.close();
    src.close();

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

    remove(TEST2INI);
}
