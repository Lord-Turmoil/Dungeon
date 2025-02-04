/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Logger.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 3, 2022                               *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Error and warning info in game should be reported and logged.            *
 * -------------------------------------------------------------------------- *
 * Reference:                                                                 *
 *   Easy2D: https://gitee.com/Easy2D/                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdarg.h>
#include <stdio.h>

#include "Macros.h"

#ifdef DUNGINE_DEBUG
#include <cassert>
#endif

#ifdef DUNGINE_DEBUG
#define DUNGINE_ASSERT(expression) assert(expression)
#else
#define DUNGINE_ASSERT(expression)
#endif

#define LOG_MESSAGE(FORMAT, ...) Logger::LogMessage(FORMAT, __VA_ARGS__)
#define LOG_ERROR(FORMAT, ...)   Logger::LogError(FORMAT, __VA_ARGS__)

#ifdef DUNGINE_NO_LOG
#define LOG_WARNING(FORMAT, ...)
#define LOG_EXTRA_WARNING(FORMAT, ...)
#else
#define LOG_WARNING(FORMAT, ...) Logger::LogWarning(FORMAT, __VA_ARGS__)
#ifdef DUNGINE_EXTRA_WARNING
#define LOG_EXTRA_WARNING(FORMAT, ...) Logger::LogWarning(FORMAT, __VA_ARGS__)
#else
#define LOG_EXTRA_WARNING(FORMAT, ...)
#endif
#endif

const char LOG_FILE[] = "log.txt";

enum class GlobalState
{
    GS_GOOD = 0,
    GS_WARNING,
    GS_ERROR
};

class Logger
{
public:
    static void LogError(const char* format, ...);
    static void LogWarning(const char* format, ...);
    static void LogMessage(const char* format, ...);

    static GlobalState GetGlobalState()
    {
        return m_globalState;
    }

    static void ClearGlobalState()
    {
        m_globalState = GlobalState::GS_GOOD;
    }

    static bool Good()
    {
        return m_globalState == GlobalState::GS_GOOD;
    }

    static bool NotBad()
    {
        return m_globalState != GlobalState::GS_ERROR;
    }

    static bool Error()
    {
        return m_globalState == GlobalState::GS_ERROR;
    }

private:
    static void _Output(FILE* fp, const char* prompt, const char* format, va_list args);
    static void _PrintTimestamp(FILE* fp);

    static GlobalState m_globalState;
};

#endif
