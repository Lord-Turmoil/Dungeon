/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Cursor.cpp                                *
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
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/ui/Cursor.h"

#include <Windows.h>
#include <dungine.h>

static HCURSOR hCursor[CUR_NUM];

void LoadCursorStyle()
{
	const char* filename;

	for (int i = 0; i < CUR_NUM; i++)
	{
		filename = splice(CURSOR_DIR, CURSOR_NAME[i]);
		if (!IsValidDirectory(filename))
			hCursor[i] = LoadCursor(NULL, IDC_ARROW);
		else
			hCursor[i] = (HCURSOR)LoadImage(NULL, widen(filename), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
	}
}

void SetCursorStyle(CursorStyle style)
{
	SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, (LONG_PTR)hCursor[style]);
}