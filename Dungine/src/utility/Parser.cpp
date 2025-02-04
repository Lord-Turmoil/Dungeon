/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Parser.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Parser is used to parse XML attribute to values.                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <cstdlib> // atof()
#include <ctype.h>
#include <string.h>

#include "../../inc/utility/Auxilliary.h"
#include "../../inc/utility/Parser.h"

/******************************************************************************
 * ParseInt -- Parse an int from a string.                                    *
 *                                                                            *
 *    This function assumes that the string only contains one int.            *
 *                                                                            *
 * INPUT:   src      -- Source string.                                        *
 *          defValue -- Default value.                                        *
 *                                                                            *
 * OUTPUT:  Return the int in it.                                             *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
int ParseInt(const char* src, int defValue)
{
    if (!src)
    {
        return defValue;
    }

    char* p = (char*)src;
    bool negative = false;

    while ((*p != '\0') && !isdigit(*p))
    {
        if (*p == '-')
        {
            negative = !negative;
        }
        p++;
    }
    if (*p == '\0')
    {
        return defValue;
    }

    int rv = 0;
    while ((*p != '\0') && isdigit(*p))
    {
        rv = (rv << 1) + (rv << 3) + (*p ^ '0');
        p++;
    }

    return negative ? (-rv) : rv;
}

/******************************************************************************
 * ParseUnsigned --- Parse an unsigned long.                                  *
 *                                                                            *
 *    This function assumes that the string only contains one value.          *
 *                                                                            *
 * INPUT:   src      -- Source string.                                        *
 *          defValue -- Default value.                                        *
 *                                                                            *
 * OUTPUT:  Return the unsinged long in it.                                   *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Created.                                               *
 *============================================================================*/
unsigned long ParseUnsigned(const char* src, unsigned long defValue)
{
    if (!src)
    {
        return defValue;
    }

    char* p = (char*)src;

    while ((*p != '\0') && !isdigit(*p))
    {
        p++;
    }
    if (*p == '\0')
    {
        return defValue;
    }

    unsigned long rv = 0;
    while ((*p != '\0') && isdigit(*p))
    {
        rv = (rv << 1) + (rv << 3) + (*p ^ '0');
        p++;
    }

    return rv;
}

/******************************************************************************
 * ParseInt -- Parse an int from a string.                                    *
 *                                                                            *
 *    This function assumes that there are multiple ints in a string, so it   *
 *    returns the end position of one reading to get ready for next reading.  *
 *                                                                            *
 * INPUT:   val -- Address of the value.                                      *
 *          src -- Source string.                                             *
 *                                                                            *
 * OUTPUT:  Return the end position of reading. Return nullptr if no number   *
 *          is available.                                                     *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
char* ParseInt(int* val, const char* src)
{
    if (!src)
    {
        return nullptr;
    }

    char* p = (char*)src;
    bool negative = false;

    while ((*p != '\0') && !isdigit(*p))
    {
        if (*p == '-')
        {
            negative = !negative;
        }
        p++;
    }
    if (*p == '\0')
    {
        return nullptr;
    }

    *val = 0;
    while ((*p != '\0') && isdigit(*p))
    {
        *val = (*val << 1) + (*val << 3) + (*p - '0');
        p++;
    }
    if (negative)
    {
        *val = -(*val);
    }

    return p;
}

/******************************************************************************
 * ParseAttribute -- Parse string attribute to specific value.                *
 *                                                                            *
 *    This is used to parse XML attribute to ordinary values.                 *
 *                                                                            *
 * INPUT:   val       -- Address of the value.                                *
 *          attribute -- The attribute string to parse.                       *
 *          defValue  -- Default value.                                       *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
bool ParseAttribute(int* val, const char* attribute, int defValue)
{
    if (!attribute)
    {
        *val = defValue;
        return false;
    }

    *val = ParseInt(attribute, defValue);

    return true;
}

bool ParseAttribute(bool* val, const char* attribute, bool defValue)
{
    if (!attribute)
    {
        *val = defValue;
        return false;
    }

    if (strcmp(attribute, "true") == 0)
    {
        *val = true;
    }
    else if (strcmp(attribute, "false") == 0)
    {
        *val = false;
    }
    else
    {
        *val = defValue;
        return false;
    }

    return true;
}

bool ParseAttribute(long* val, const char* attribute, long defValue)
{
    if (!attribute)
    {
        *val = defValue;
        return false;
    }

    *val = ParseInt(attribute, defValue);

    return true;
}

bool ParseAttribute(unsigned long* val, const char* attribute, unsigned long defValue)
{
    if (!attribute)
    {
        *val = defValue;
        return false;
    }

    *val = ParseUnsigned(attribute, defValue);

    return true;
}

bool ParseAttribute(double* val, const char* attribute, double defValue)
{
    if (!attribute)
    {
        *val = defValue;
        return false;
    }

    *val = atof(attribute);

    return true;
}

/******************************************************************************
 * ParseAttribute -- Parse C-style string to string.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   val       -- Address of the value.                                *
 *          attribute -- The attribute string to parse.                       *
 *          defString -- Default string if parse failed.                      *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
bool ParseAttribute(const char** val, const char* attribute)
{
    if (!attribute)
    {
        return false;
    }

    *val = attribute;

    return true;
}

bool ParseAttribute(const char** val, const char* attribute, const char* defString)
{
    if (!attribute)
    {
        *val = defString;
        return false;
    }

    *val = attribute;

    return true;
}

bool ParseAttribute(std::string* val, const char* attribute)
{
    if (!attribute)
    {
        return false;
    }

    *val = attribute;

    return true;
}

bool ParseAttribute(std::string* val, const char* attribute, const std::string& defString)
{
    if (!attribute)
    {
        *val = defString;
        return false;
    }

    *val = attribute;

    return true;
}

bool ParseAttribute(std::wstring* val, const char* attribute)
{
    if (attribute == nullptr)
    {
        return false;
    }

    static wchar_t w_attr[WIDECHAR_BUFFER_SIZE];

    if (widen(w_attr, attribute))
    {
        *val = w_attr;
    }
    else
    {
        return false;
    }

    return true;
}

bool ParseAttribute(std::wstring* val, const char* attribute, const std::wstring& defString)
{
    if (attribute == nullptr)
    {
        *val = defString;
        return false;
    }

    static wchar_t w_attr[WIDECHAR_BUFFER_SIZE];

    if (widen(w_attr, attribute))
    {
        *val = w_attr;
    }
    else
    {
        *val = defString;
        return false;
    }

    return true;
}

/******************************************************************************
 * ParsePrivateAttribute -- Parse private attribute to private value.         *
 *                                                                            *
 *    This is for some custom types, and require their own parser to parse.   *
 *                                                                            *
 * INPUT:   val       -- Address of the value.                                *
 *          attribute -- Source attribute string.                             *
 *          pParser   -- The specific parser for the custom type.             *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
bool ParsePrivateAttribute(void* val, const char* attribute, bool (*pParser)(void*, const char*))
{
    if ((!attribute) || (!pParser))
    {
        return false;
    }

    return pParser(val, attribute);
}
