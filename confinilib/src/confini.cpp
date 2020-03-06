#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <LittleCPPHelper/strings.h>

#include <confini.h>
#include <confiniversion.h>

using namespace std;

/************************************************************************//**
 * \brief     Returns library version
 * \return    const char *: Library version
 * \exception -
 * \author    Shadouw
 ****************************************************************************/
char const * confini::version()
{
    return CONFINIVERSION;
}

/************************************************************************//**
 * \brief     Default Constructor
 * \exception -
 * \author    Shadouw
 ****************************************************************************/
template<class T> confini::ConfIniFile<T>::ConfIniFile ()
{

}

/************************************************************************//**
 * \brief     Constructor to read a file
 * \param     [in]  filename The name of the file to open
 * \exception runtime_error
 * \author    Shadouw
 ****************************************************************************/
template<class T> confini::ConfIniFile<T>::ConfIniFile ( STRINGTYPE filename ) :
    m_strFilename ( filename )
{
    setfile ( m_strFilename );
}

/************************************************************************//**
 * \brief     Checks a single line of the ini file
 * \param     [in]  strLine
 * \return    enum #ConfIniFileLineType
 * \exception runtime_error
 * \author    Shadouw
 ****************************************************************************/
template<class T> confini::ConfIniFileLineType confini::ConfIniFile<T>::checkline ( STRINGTYPE strLine )
{
    LittleCPPHelper::trim(strLine);

    if ( 0 == strLine.size() )                           // 1) Whitespace only
    {
        return WHITESPACE;
    }
    else if ( '#' == strLine[0] || ';' == strLine[0] )   // 2) # or ; Comment
    {
        return COMMENT;
    }
    else if ( '[' == strLine[0] )                        // 3) [Section]
    {
        if ( ']' != strLine.at(strLine.size()-1) )
        {
            throw runtime_error( "Invalid Section Line: " + strLine );
        }
        return SECTION;
    }
    else                                                 // 4) Key=Value
    {
        int nPos = strLine.find('=');

        if ( -1 == nPos )
        {
            throw runtime_error( "Invalid Line: " + strLine );
        }

        return KEYVALUE;
    }
}

/************************************************************************//**
 * \brief     Splits a "Key = Value" pair into both parts.
 * \param     [in]  strLine
 * \return    pair<key,value>
 * \exception runtime_error
 * \author    Shadouw
 ****************************************************************************/
template<class T> typename confini::ConfIniFile<T>::VALUETYPE confini::ConfIniFile<T>::splitkeyvalue ( STRINGTYPE strLine )
{
    int nPos = strLine.find('=');

    if ( -1 == nPos )
    {
        throw runtime_error( "Invalid Line: " + strLine );
    }
    STRINGTYPE strKey   = LittleCPPHelper::trim_copy(strLine.substr(0, nPos));
    STRINGTYPE strValue = LittleCPPHelper::trim_copy(strLine.substr(nPos+1));

    return make_pair(strKey, strValue);
}

/************************************************************************//**
 * \brief     Load filename into memory
 * \param     [in]  filename
 * \return    void
 * \exception runtime_error
 * \author    Shadouw
 ****************************************************************************/
template<class T> void confini::ConfIniFile<T>::setfile ( STRINGTYPE filename )
{
    m_strFilename = filename;
    m_mapConfig.clear();

    basic_ifstream<T> inifile ( m_strFilename );

    STRINGTYPE strLine;
    STRINGTYPE strSection;
    while ( getline(inifile, strLine) )
    {
        basic_istringstream<T> iss(strLine);

        switch ( checkline(strLine) )
        {
            case WHITESPACE:
            case COMMENT:
                break;
            case SECTION:
                LittleCPPHelper::trim(strLine);
                strSection = strLine.substr(1, strLine.size()-2);

                if ( m_mapConfig.end() != m_mapConfig.find(strSection) )
                    throw runtime_error( "Duplicate Section: [" + strSection + "]" );


                break;
            case KEYVALUE:
                VALUETYPE keyvalue = splitkeyvalue ( strLine );

                if ( "" != m_mapConfig[strSection][keyvalue.first].first )
                    throw runtime_error( "Duplicate Key: [" + strSection + "] " + keyvalue.first );

                m_mapConfig[strSection][keyvalue.first].first  = keyvalue.second;
                m_mapConfig[strSection][keyvalue.first].second = keyvalue.second;
                break;
        }
    }
}

/************************************************************************//**
 * \brief     Saves changes back to file
 * \return    void
 * \exception -
 * \author    Shadouw
 ****************************************************************************/
template<class T> void confini::ConfIniFile<T>::savefile ()
{
    // First read entire file
    std::vector<STRINGTYPE> vstrFile;
    basic_ifstream<T> inifile;
    STRINGTYPE strLine1;

    inifile.open( m_strFilename );
    while ( getline(inifile, strLine1) )
        vstrFile.push_back(strLine1);
    inifile.close();

    // Next write the vector back into the file
    basic_ofstream<T> outfile;
    outfile.open ( m_strFilename );
    STRINGTYPE strSection;

    for ( STRINGTYPE strLine2 : vstrFile )
    {
        switch ( checkline(strLine2) )
        {
            case WHITESPACE:
            case COMMENT:
                outfile << strLine2 << endl;
                break;
            case SECTION:
                // Any new value in "old" Section?
                {
                    bool bWritten = false;
                    for ( auto &keyvalue : m_mapConfig[strSection] )
                    {
                        if ( keyvalue.second.second != keyvalue.second.first )
                        {
                            bWritten = true;
                            outfile << keyvalue.first << " = " << keyvalue.second.first  << endl;
                            keyvalue.second.second = keyvalue.second.first;
                        }
                    }

                    // Now write Section into file
                    if ( bWritten )
                        outfile << endl;
                }

                outfile << strLine2 << endl;
                LittleCPPHelper::trim(strLine2);
                strSection = strLine2.substr(1, strLine2.size()-2);
                break;
            case KEYVALUE:
                VALUETYPE keyvalue = splitkeyvalue ( strLine2 );

                if ( "" == m_mapConfig[strSection][keyvalue.first].first ) // Value got deleted
                {
                    // Don't write, but remember that it's empty now
                    m_mapConfig[strSection][keyvalue.first].second = "";
                }
                else if ( m_mapConfig[strSection][keyvalue.first].first !=  // Value got changed
                          m_mapConfig[strSection][keyvalue.first].second )
                {
                    // Write the changed value and remember it
                    outfile << keyvalue.first << " = " << m_mapConfig[strSection][keyvalue.first].first  << endl;
                    m_mapConfig[strSection][keyvalue.first].second = m_mapConfig[strSection][keyvalue.first].first;
                }
                else // Obviously unchanged
                {
                    outfile << strLine2 << endl;
                }
                break;
        }
    }

    // Write all new sections now
    string strLastWrittenSection;
    for ( auto keymap : m_mapConfig )
    {
        for ( auto &keyvalue : keymap.second )
        {
            if ( keyvalue.second.second != keyvalue.second.first )
            {
                if ( strLastWrittenSection != keymap.first )
                {
                    outfile << endl;
                    outfile << "[" << keymap.first << "]" << endl;
                    strLastWrittenSection = keymap.first;
                }

                outfile << keyvalue.first << " = " << keyvalue.second.first  << endl;
                keyvalue.second.second = keyvalue.second.first;
            }
        }
    }

    outfile.close();
}

// Force compiler to generate specialized classes
template class confini::ConfIniFile<char>;
