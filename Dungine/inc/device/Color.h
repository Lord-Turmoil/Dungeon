/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Color.h                                   *
 *                                                                            *
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

#ifndef _COLOR_H_
#define _COLOR_H_

#include <easyx.h>

// Parse R, G, B in a color.
#define PA(color) (((color) & 0xFF000000) >> 24)
#define PB(color) (((color) & 0x00FF0000) >> 16)
#define PG(color) (((color) & 0x0000FF00) >> 8)
#define PR(color) (((color) & 0x000000FF))

// Convert BGR to RGB
#define BGRtoRGB(color) ((PR(color) << 16) | (PG(color) << 8) | PB(color))

/********************************************************************
** Get the "alpha-ed" color.
**   s: Color of the source image.
**   d: Color of the destination image.
**   a: Alpha value.
*/
#define ARGB(s, d, a)                                                                                                  \
    RGB((PR(s) * a + PR(d) * (255 - a)) >> 8, (PG(s) * a + PG(d) * (255 - a)) >> 8,                                    \
        (PB(s) * a + PB(d) * (255 - a)) >> 8)

/********************************************************************
** Parse color value, used as a callback function.
*/
bool ParseColor(void* val, const char* attribute);

#endif
