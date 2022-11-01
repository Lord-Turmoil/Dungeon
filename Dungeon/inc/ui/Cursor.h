/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Cursor.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 18, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Manage cursor style.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _CURSOR_H_
#define _CURSOR_H_

enum CursorStyle
{
	CUR_ARROW,
	CUR_ATTACK,
	CUR_NUM
};

const char CURSOR_DIR[] = "res\\images\\cursor\\";
const char* const CURSOR_NAME[] = {
	"arrow.cur",
	"attack.ani" };

void LoadCursorStyle();	// Load on start.
void SetCursorStyle(CursorStyle style);

#endif