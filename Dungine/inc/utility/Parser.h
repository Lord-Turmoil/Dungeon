/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Parser.h                                  *
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

#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>

#include "../common/Logger.h"
#include "../utility/Auxilliary.h"

int ParseInt(const char* src, int defValue);
unsigned long ParseUnsigned(const char* src, unsigned long defValue);
char* ParseInt(int* val, const char* src);

bool ParseAttribute(int* val, const char* attribute, int defValue);
bool ParseAttribute(bool* val, const char* attribute, bool defValue);
bool ParseAttribute(long* val, const char* attribute, long defValue);
bool ParseAttribute(unsigned long* val, const char* attribute, unsigned long defValue);
bool ParseAttribute(double* val, const char* attribute, double defValue);

bool ParseAttribute(const char** val, const char* attribute);
bool ParseAttribute(const char** val, const char* attribute, const char* defString);
bool ParseAttribute(std::string* val, const char* attribute);
bool ParseAttribute(std::string* val, const char* attribute, const std::string& defString);
bool ParseAttribute(std::wstring* val, const char* attribute);
bool ParseAttribute(std::wstring* val, const char* attribute, const std::wstring& defString);

bool ParsePrivateAttribute(void* val, const char* attribute, bool (*pParser)(void*, const char*));

#define _PARSE(TAG, VAR, LOC, DEF)                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((attr = node->Attribute(TAG)))                                                                             \
        {                                                                                                              \
            if (!ParseAttribute(&VAR, attr, DEF))                                                                      \
                LOG_WARNING(INVALID_ATTRIBUTE, TAG, LOC);                                                              \
        }                                                                                                              \
        else                                                                                                           \
            LOG_EXTRA_WARNING(MISSING_ATTRIBUTE, TAG, LOC);                                                            \
    } while (0)

#define _PARSE_ESSENTIAL(TAG, VAR, LOC, DEF)                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((attr = node->Attribute(TAG)))                                                                             \
        {                                                                                                              \
            if (!ParseAttribute(&VAR, attr, DEF))                                                                      \
                LOG_ERROR(INVALID_ATTRIBUTE, TAG, LOC);                                                                \
        }                                                                                                              \
        else                                                                                                           \
            LOG_ERROR(MISSING_ATTRIBUTE, TAG, LOC);                                                                    \
    } while (0)

#define _PARSE_PRIVATE(TAG, VAR, LOC, PARSER)                                                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((attr = node->Attribute(TAG)))                                                                             \
        {                                                                                                              \
            if (!ParsePrivateAttribute(&VAR, attr, PARSER))                                                            \
                LOG_WARNING(INVALID_ATTRIBUTE, TAG, LOC);                                                              \
        }                                                                                                              \
        else                                                                                                           \
            LOG_EXTRA_WARNING(MISSING_ATTRIBUTE, TAG, LOC);                                                            \
    } while (0)

#define _PARSE_PRIVATE_ESSENTIAL(TAG, VAR, LOC, PARSER)                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((attr = node->Attribute(TAG)))                                                                             \
        {                                                                                                              \
            if (!ParsePrivateAttribute(&VAR, attr, PARSER))                                                            \
                LOG_ERROR(INVALID_ATTRIBUTE, TAG, LOC);                                                                \
        }                                                                                                              \
        else                                                                                                           \
            LOG_ERROR(MISSING_ATTRIBUTE, TAG, LOC);                                                                    \
    } while (0)

#define _CHECK_TAG(TRUE_TAG)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        if (_STR_DIFF(name, TRUE_TAG))                                                                                 \
            LOG_ERROR(TAG_MISMATCH, name, TRUE_TAG);                                                                   \
    } while (0)

#define _CHECK_TYPE(TRUE_TYPE)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(attr = node->Attribute("type")))                                                                         \
            LOG_ERROR(MISSING_ATTRIBUTE, "type", name);                                                                \
        else                                                                                                           \
        {                                                                                                              \
            if (_STR_DIFF(attr, TRUE_TYPE))                                                                            \
                LOG_ERROR(TYPE_MISMATCH, attr, TRUE_TYPE);                                                             \
        }                                                                                                              \
    } while (0)

#define _RETURN_IF_ERROR()                                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if (Logger::Error())                                                                                           \
            return false;                                                                                              \
    } while (0)

#define _RETURN_STATE()                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        return !Logger::Error();                                                                                       \
    } while (0)

#endif
