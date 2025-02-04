/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Startup.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 30, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                  *
 *   Load all resources and make all preperations.                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _STARTUP_H_
#define _STARTUP_H_

#include <dungine.h>

/********************************************************************
** All in all, the game is to run the application. If there are any
** errors, just replace the application, and run will not be affected.
*/

bool Initialize();
bool _InitDevice();
bool _InitApplication();
void _UnInitApplication();
bool _InitErrorApplication();

void Run();
void _RunError();

void ClearUp();

// Used to generate internal background.
const COLORREF SRC_COLOR = RGB(41, 182, 246);
const COLORREF DEST_COLOR = RGB(128, 222, 234);
void GradiantFill(IMAGE* pImage, COLORREF srcColor, COLORREF destColor);

#endif
