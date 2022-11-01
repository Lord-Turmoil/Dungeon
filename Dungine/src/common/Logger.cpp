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
 *                    Last Update : May 12, 2022                              *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
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

#include <cstdlib>
#include <Windows.h>

#include "../../inc/common/Logger.h"
#include "../../inc/common/Macros.h"


GlobalState Logger::m_globalState;

// How many logs.
static int logCount;


/******************************************************************************
 * Logger::LogError -- Log an error info.                                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   format -- Format in vfprintf.                                     *
 *          ...    -- Argument list.                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Created.                                               *
 *============================================================================*/
void Logger::LogError(const char* format, ...)
{
	FILE* fp;
	va_list args;

	va_start(args, format);

#ifdef DGE_ENABLE_CONSOLE_LOG
	_Output(stdout, "Error: ", format, args);
#endif

	if (fopen_s(&fp, LOG_FILE, "a") != 0)
	{
		_Output(stderr, "ERROR: ", "LogError() failed!", NULL);
		return;
	}
	_Output(fp, "Error: ", format, args);
	fclose(fp);

	va_end(args);

	m_globalState = GlobalState::GS_ERROR;

	logCount++;
}


/******************************************************************************
 * Logger::LogWarning -- Log a warning info.                                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   format -- Format in vfprintf.                                     *
 *          ...    -- Argument list.                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Created.                                               *
 *============================================================================*/
void Logger::LogWarning(const char* format, ...)
{
	FILE* fp;
	va_list args;
	
	va_start(args, format);

#ifdef DGE_ENABLE_CONSOLE_LOG
	_Output(stdout, "Error: ", format, args);
#endif

	if (fopen_s(&fp, LOG_FILE, "a") != 0)
	{
		_Output(stderr, "ERROR: ", "LogWarning() failed!", NULL);
		return;
	}
	_Output(fp, "Warning: ", format, args);
	fclose(fp);

	va_end(args);

	m_globalState = GlobalState::GS_WARNING;

	logCount++;
}


/******************************************************************************
 * Logger::LogMessage -- Log a message.                                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   format -- Format in vfprintf.                                     *
 *          ...    -- Argument list.                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Created.                                               *
 *============================================================================*/
void Logger::LogMessage(const char* format, ...)
{
	FILE* fp;
	va_list args;

	va_start(args, format);

#ifdef DGE_ENABLE_CONSOLE_LOG
	_Output(stdout, "Message: ", format, args);
#endif

	if (fopen_s(&fp, LOG_FILE, "a") != 0)
	{
		_Output(stderr, "ERROR: ", "LogMessage() failed!", NULL);
		return;
	}
	_Output(fp, "Message: ", format, args);
	fclose(fp);

	va_end(args);

	logCount++;
}


/******************************************************************************
 * Logger::_Output -- Output a message.                                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   fp     -- Target file.                                            *
 *          prompt -- Prompt info.                                            *
 *          format -- ...                                                     *
 *          args   -- Argument list.                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Created.                                               *
 *============================================================================*/
void Logger::_Output(FILE* fp, const char* prompt, const char* format, va_list args)
{
	_PrintTimestamp(fp);

	fprintf_s(fp, "%s", prompt);
	vfprintf_s(fp, format, args);
	fprintf_s(fp, "\n");
}


/******************************************************************************
 * Logger::_PrintTimestamp -- Print timestamp to error file.                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   fp -- Target file.                                                *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Created.                                               *
 *============================================================================*/
void Logger::_PrintTimestamp(FILE* fp)
{
	if (logCount > 0)
		return;

	SYSTEMTIME sysTime;

	GetLocalTime(&sysTime);

	fprintf_s(fp, "\n------------------------------\n");	// *30
	fprintf_s(fp, "%02hu/%02hu/%02hu ", sysTime.wMonth, sysTime.wDay, sysTime.wYear);
	fprintf_s(fp, "%02hu:%02hu:%02hu\n", sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
}