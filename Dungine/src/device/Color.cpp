/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Color.cpp                                 *
 *												                              *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 2, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Provide some color definitions and operations.                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <cstdio>

#include "../../inc/device/color.h"

/******************************************************************************
 * ParseColor -- Parse color from a string.                                   *
 *                                                                            *
 *    The format of the string can be "rgb(r, g, b)" or "#rrggbb".            *
 *                                                                            *
 * INPUT:   val       -- Address of the value.                                *
 *          attribute -- The source attribute string.                         *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
bool ParseColor(void* val, const char* attribute)
{
    if (attribute == nullptr)
    {
        return false;
    }

    COLORREF color;
    if (*attribute == '#')
    {
        if (sscanf_s(attribute, "#%x", &color) != 1)
        {
            return false;
        }
        color = BGRtoRGB(color);
    }
    else
    {
        int r, g, b;
        if (sscanf_s(attribute, "rgb(%d, %d, %d)", &r, &g, &b) != 3)
        {
            return false;
        }
        color = RGB(r, g, b);
    }

    *static_cast<COLORREF*>(val) = color;

    return true;
}
