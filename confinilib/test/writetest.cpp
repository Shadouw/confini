#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include <confini.h>
#include <confinitestfiles.h>

using namespace std;
using namespace confini;

BOOST_AUTO_TEST_CASE ( MakeChanges )
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

BOOST_AUTO_TEST_CASE ( TestChanges )
{
    ConfIniFile<char> File1(TEST2INI);

    BOOST_TEST ( std::string("Value") == File1("Section1", "Key2") );
    BOOST_TEST ( std::string("NewValue") == File1("Section1", "Key3") );
    BOOST_TEST ( std::string("Never change this!") == File1("Section1", "Key13") );
    BOOST_TEST ( std::string("New Section & Value") == File1("Section-3", "Key1") );
    BOOST_TEST ( std::string("New Section & Value") == File1("Section-4", "Key1") );
    BOOST_TEST ( std::string("New Section & Value") == File1("Section-4", "Key2") );
    BOOST_TEST ( std::string("New Section & Value") == File1("Section-4", "Key3") );
    BOOST_TEST ( std::string("New Section & Value") == File1("Section-5", "Key1") );
    BOOST_TEST ( std::string("New Section & Value") == File1("Section-5", "Key2") );
    BOOST_TEST ( std::string("New Section & Value") == File1("Section-5", "Key3") );
    BOOST_TEST ( std::string("") == File1("Section1", "Key1") );

    boost::filesystem::remove(TEST2INI);
}
