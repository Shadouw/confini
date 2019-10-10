#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>

#include <confini.h>
#include <confinitestfiles.h>

using namespace std;
using namespace confini;

BOOST_AUTO_TEST_CASE ( ChangeExistingValue )
{
    ConfIniFile<char> File1(TESTINI);

    // First read it
    BOOST_TEST ( std::string("1234") == File1("Section1", "Key2") );

    // Now Change it
    File1("Section1", "Key2") = "Value";
    // Test again
    BOOST_TEST ( std::string("Value") == File1("Section1", "Key2") );
}

BOOST_AUTO_TEST_CASE ( AddNewValue )
{
    ConfIniFile<char> File1(TESTINI);

    // First read it
    BOOST_TEST ( std::string("") == File1("Section1", "Key3") );

    // Now Change it
    File1("Section1", "Key3") = "NewValue";
    // Test again
    BOOST_TEST ( std::string("NewValue") == File1("Section1", "Key3") );
}

BOOST_AUTO_TEST_CASE ( AddNewSection )
{
    ConfIniFile<char> File1(TESTINI);

    // First read it
    BOOST_TEST ( std::string("") == File1("Section-3", "Key1") );

    // Now Change it
    File1("Section-3", "Key1") = "New Section & Value";
    // Test again
    BOOST_TEST ( std::string("New Section & Value") == File1("Section-3", "Key1") );
}

BOOST_AUTO_TEST_CASE ( RemoveValue )
{
    ConfIniFile<char> File1(TESTINI);

    // First read it
    BOOST_TEST ( std::string("1234") == File1("Section1", "Key2") );

    // Now Change it
    File1("Section1", "Key2") = "";
    // Test again
    BOOST_TEST ( std::string("") == File1("Section1", "Key2") );
}
