/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : DMath.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 25, 2022                             *
 *                                                                            *
 *                    Last Update : December 9, 2022                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide all math operations.                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <cmath>

#include "../../inc/utility/DMath.h"


/******************************************************************************
 * Distance -- Get distance between two coordinate.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   p1                                                                *
 *          p2                                                                *
 *                                                                            *
 * OUTPUT:  Return the distance.                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
double Distance(const Coordinate& p1, const Coordinate& p2)
{
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;

	return sqrt(dx * dx + dy * dy);
}


/******************************************************************************
 * ManhattanDist -- Get Manhatta distance between two coordinate.             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   p1                                                                *
 *          p2                                                                *
 *                                                                            *
 * OUTPUT:  Return the distance.                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
int ManhattanDist(const Coordinate& p1, const Coordinate& p2)
{
	return dabs(p1.x - p2.x) + dabs(p1.y - p2.y);
}

int ManhattanDist(int x1, int y1, int x2, int y2)
{
	return dabs(x1 - x2) + dabs(y1 - y2);
}



/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Vector Operations
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/******************************************************************************
 * Normalize -- Get the normal of the vector.                                 *
 *                                                                            *
 *    This only get one normal vector.                                        *
 *                                                                            *
 * INPUT:   vec                                                               *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
Vector& Normalize(Vector& vec)
{
	vec.Init(vec.y, -vec.x);
	return vec;
}

Vector Normal(const Vector& vec)
{
	return Vector(vec.y, -vec.x);
}


/******************************************************************************
 * Unitize -- Unitize a vector.                                               *
 *                                                                            *
 *    This will not change the original vector, but will create a new vector. *
 *                                                                            *
 * INPUT:   vec                                                               *
 *                                                                            *
 * OUTPUT:  Return the normal vector.                                         *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
Vector& Unitize(Vector& vec)
{
	double mod = Module(vec);

	if (mod < EPSILON)
		vec.Init(0.0, 0.0);
	else
		vec /= mod;

	return vec;
}

Vector Unit(const Vector& vec)
{
	double mod = Module(vec);

	if (mod < EPSILON)
		return VECTOR_ZERO;
	else
		return vec / mod;
}


/******************************************************************************
 * Module -- Get the module of the vector.                                    *
 *                                                                            *
 *    This will not change the original vector, just get its module.          *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
double Module(const Vector& vec)
{
	return Distance(vec.x, vec.y);
}


/******************************************************************************
 * Rotate -- Rotate a vector.                                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   vec    -- The original vector.                                    *
 *          radian -- Rotate counter-clockwise.                               *
 *                                                                            *
 * OUTPUT:  Return the rotated vector.                                        *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
Vector Rotate(const Vector& vec, double radian)
{
	double cosx = cos(radian);
	double sinx = sin(radian);

	return { vec.x * cosx - vec.y * sinx, vec.x * sinx + vec.y * cosx };
}


/******************************************************************************
 * Rotate -- Rotate... Coord?                                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   vec                                                               *
 *          radian                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
Coordinate Rotate(const Coordinate& vec, double radian)
{
	double cosx = cos(radian);
	double sinx = sin(radian);

	return { (int)((double)vec.x * cosx - (double)vec.y * sinx), -(int)((double)vec.x * sinx + (double)vec.y * cosx) };
}


/******************************************************************************
 * VectorProjection -- Get the vector projection of vec to base.              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   vec                                                               *
 *          base                                                              *
 *                                                                            *
 * OUTPUT:  Vector projection.                                                *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/12/09 Tony : Created.                                               *
 *============================================================================*/
Vector VectorProjection(Vector& vec, Vector& base)
{
	Vector unit = Unit(base);
	return vec * unit * unit;
}

/******************************************************************************
 * ScalarProjection -- Get the scalar projection of vec to base.              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   vec                                                               *
 *          base                                                              *
 *                                                                            *
 * OUTPUT:  Scalar projection.                                                *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/12/09 Tony : Created.                                               *
 *============================================================================*/
double ScalarProjection(Vector& vec, Vector& base)
{
	return vec * Unit(base);
}


/******************************************************************************
 * GetDirection -- Get direction from one point to another.                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   v1, v2                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/23 Tony : Created.                                               *
 *============================================================================*/
Vector GetDirection(const Vector& v1, const Vector& v2)
{
	return Unit(v2 - v1);
}

Vector GetDirection(const Coordinate& v1, const Coordinate& v2)
{
	if (v1 == v2)
		return VECTOR_PX;
	return Unit({ (double)(v2.x - v1.x), (double)(v2.y - v1.y) });
}


/******************************************************************************
 * GetDisplacement -- Get displacement from v1 to v2.                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   v1, v2                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/29 Tony : Created.                                               *
 *============================================================================*/
Vector GetDisplacement(const Vector& v1, const Vector& v2)
{
	return v2 - v1;
}

Vector GetDisplacement(const Coordinate& v1, const Coordinate& v2)
{
	return { (double)(v2.x - v1.x), (double)(v2.y - v1.y) };
}


/******************************************************************************
 * GetAngle -- Get angle between two vectors.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/25 Tony : Created.                                               *
 *============================================================================*/
double GetAngleCosine(const Vector& v1, const Vector& v2)
{
	double m = Module(v1) * Module(v2);
	
	if (IsZero(m))
		return 1.0;
	else
		return (v1 * v2) / m;
}

double GetAngle(const Vector& v1, const Vector& v2)
{
	return acos(GetAngleCosine(v1, v2));
}