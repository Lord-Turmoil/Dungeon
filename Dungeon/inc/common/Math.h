/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Math.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 25, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide basic math functions.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _MATH_H_
#define _MATH_H_

#include <dungine.h>

/********************************************************************
** Get the radian to rotate from positive x to the given direction.
*/
double GetRotationRadian(const Coordinate& dir, bool mirror = false);
double GetRotationRadian(const Vector& dir, bool mirror = false);
double GetRotationRadian(const Coordinate& dir, int radius, bool mirror = false);
double GetRotationRadian(const Vector& dir, double radius, bool mirror = false);

#endif
