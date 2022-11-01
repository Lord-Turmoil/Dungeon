/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : DMath.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 25, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
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

#ifndef _DMATH_H_
#define _DMATH_H_

#include "../common/DDefines.h"

#ifndef abs
#define abs(a) (((a) > (0)) ? (a) : (-(a)))
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef sign
#define sign(x) (((x) < (0)) ? (-1) : (1))
#endif

template<typename T>
inline T dabs(T a)
{
	return (a > (T)0) ? a : (-a);
}

template<typename T>
inline T dmax(T a, T b)
{
	return (a > b) ? a : b;
}

template<typename T>
inline T dmin(T a, T b)
{
	return (a < b) ? a : b;
}

template<typename T, typename... Values>
inline T dmax(T arg, Values... args)
{
	return dmax(arg, dmax(args...));
}

template<typename T, typename... Values>
inline T dmin(T arg, Values... args)
{
	return dmin(arg, dmin(args...));
}

inline int to_even(int a)
{
	return a & (~1);
}

inline int to_odd(int a)
{
	return a | 1;
}


// distance
double Distance(const Coordinate& p1, const Coordinate& p2);
int ManhattanDist(const Coordinate& p1, const Coordinate& p2);
int ManhattanDist(int x1, int y1, int x2, int y2);
template<typename T>
double Distance(T x1, T y1, T x2, T y2)
{
	T dx = x1 - x2;
	T dy = y1 - y2;

	return sqrt(dx * dx + dy * dy);
}
template<typename T>
double Distance(T x, T y)
{
	return sqrt(x * x + y * y);
}

// vector operations
Vector& Normalize(Vector& vec);
Vector Normal(const Vector& vec);
Vector& Unitize(Vector& vec);
Vector Unit(const Vector& vec);
double Module(const Vector& vec);
Vector Rotate(const Vector& vec, double radian);
Coordinate Rotate(const Coordinate& vec, double radian);

inline bool IsZero(const Vector& vec)
{
	return Module(vec) < EPSILON;
}

inline bool IsZero(double x)
{
	return dabs(x) < EPSILON;
}

inline bool IsTrivia(const Vector& vec)
{
	return Module(vec) < 1.0;
}

inline bool IsTrivia(double x)
{
	return dabs(x) < 1.0;
}

inline bool IsOpposite(double x, double y)
{
	return ((x < 0.0) != (y < 0.0));
}

// Get direction from v1 to v2.
Vector GetDirection(const Vector& v1, const Vector& v2);
Vector GetDirection(const Coordinate& v1, const Coordinate& v2);
Vector GetDisplacement(const Vector& v1, const Vector& v2);
Vector GetDisplacement(const Coordinate& v1, const Coordinate& v2);

double GetAngleCosine(const Vector& v1, const Vector& v2);
double GetAngle(const Vector& v1, const Vector& v2);

#endif