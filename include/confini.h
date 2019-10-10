#pragma once

#include <string>
#include <map>
#include <utility>

/************************************************************************//**
 * \brief     A class to read and write configuration/ini files
 * \author    Shadouw
 ****************************************************************************/
namespace confini
{
    char const * version();
    enum ConfIniFileLineType { WHITESPACE, COMMENT, SECTION, KEYVALUE };

    template<class T> class ConfIniFile
    {

    typedef std::basic_string<T>             STRINGTYPE;       /**< STRINGTYPE based on character type T */
    typedef std::pair<STRINGTYPE,STRINGTYPE> VALUETYPE;        /**< VALUETYPE holds a pair of "current value" and "original value" or "key" and "value" while parsing */
    typedef std::map<STRINGTYPE,VALUETYPE>   KEYMAPTYPE;       /**< KEYMAPTYPE maps from key to VALUETYPE (inside a section) */
    typedef std::map<STRINGTYPE,KEYMAPTYPE>  SECTIONMAPTYPE;   /**< SECTIONMAPTYPE maps from section to KEYMAPTYPE */

    public:
        ConfIniFile ();
        ConfIniFile ( STRINGTYPE filename );

        void setfile ( STRINGTYPE filename );
        void savefile ();

        STRINGTYPE &operator() ( STRINGTYPE strSection, STRINGTYPE strKey )
        {
            return m_mapConfig[strSection][strKey].first;
        }

    private:
        ConfIniFileLineType checkline ( STRINGTYPE );
        VALUETYPE splitkeyvalue ( STRINGTYPE );

        SECTIONMAPTYPE m_mapConfig;
        STRINGTYPE     m_strFilename;
    };
}
