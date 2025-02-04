/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Math.cpp                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 25, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Provide basic math functions.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Math.h"

/******************************************************************************
 * GetRotationRadian -- Get the radian to rotate.                             *
 *                                                                            *
 *    This is mainly for weapon and bullet. If the object has two directions, *
 *    such as weapon, mirror should be set true to deal with the radian in a  *
 *    proper way. And since weapon's muzzle should aim at the target, radius  *
 *    is needed. Direction of positive x-axis is the base.                    *
 *                                                                            *
 * INPUT:   dir    -- Direction to rotate to.                                 *
 *          radius                                                            *
 *          mirror                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/25 Tony : Created.                                               *
 *============================================================================*/
double GetRotationRadian(const Coordinate& dir, bool mirror)
{
    double radian;

    if (dir.x == 0)
    {
        radian = (dir.y > 0) ? (-HALF_PI) : HALF_PI;
    }
    else if (dir.x > 0)
    {
        radian = atan(-static_cast<double>(dir.y) / static_cast<double>(dir.x));
    }
    else
    {
        radian = atan(-static_cast<double>(dir.y) / static_cast<double>(dir.x)) + (mirror ? 0.0 : PI);
    }

    return radian;
}

double GetRotationRadian(const Vector& dir, bool mirror)
{
    double radian;

    if (dabs(dir.x) < EPSILON)
    {
        radian = (dir.y > 0.0) ? (-HALF_PI) : HALF_PI;
    }
    else if (dir.x > 0.0)
    {
        radian = atan(-dir.y / dir.x);
    }
    else
    {
        radian = atan(-dir.y / dir.x) + (mirror ? 0.0 : PI);
    }

    return radian;
}

double GetRotationRadian(const Coordinate& dir, int radius, bool mirror)
{
    double r1 = GetRotationRadian(dir, mirror);
    if (radius == 0)
    {
        return r1;
    }

    double dist = Distance(COORD_ZERO, dir);
    if ((dist < static_cast<double>(radius)) || IsZero(dist))
    {
        return r1;
    }

    double r2 = asin(static_cast<double>(radius) / dist);
    if (mirror && (dir.x < 0))
    {
        return r1 + r2;
    }
    return r1 - r2;
}

double GetRotationRadian(const Vector& dir, double radius, bool mirror)
{
    double r1 = GetRotationRadian(dir, mirror);
    if (radius == 0)
    {
        return r1;
    }

    double dist = Module(dir);
    if ((dist < radius) || IsZero(dist))
    {
        return r1;
    }

    double r2 = asin(radius / dist);
    if (mirror && (dir.x < 0))
    {
        return r1 + r2;
    }
    return r1 - r2;
}
