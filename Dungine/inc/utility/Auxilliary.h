/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Auxilliary.h                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 31, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Integrate some auxilliary functions.                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _AUXILLIARY_H_
#define _AUXILLIARY_H_

#include <cmath>
#include <string>
#include <vector>

#include "../common/Common.h"

#define _STR_SAME(STR1, STR2) (strcmp(STR1, STR2) == 0)
#define _STR_DIFF(STR1, STR2) (strcmp(STR1, STR2) != 0)

#define _DELETE(POINTER)                                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        if (POINTER)                                                                                                   \
        {                                                                                                              \
            delete POINTER;                                                                                            \
            POINTER = nullptr;                                                                                         \
        }                                                                                                              \
    } while (0)

// Shape related.
void ResizeRect(RECT* rect, int left, int top, int right, int bottom);

bool InRectangle(const RECT& rect, const Coordinate& target);
bool InRectangleStrict(const RECT& rect, const Coordinate& target);
bool InRectangle(const Rect& rect, const Coordinate& target);
bool InRectangleStrict(const Rect& rect, const Coordinate& target);
bool InRoundRect(const RECT& rect, int radius, const Coordinate& target);
bool InRoundRect(const Rect& rect, int radius, const Coordinate& target);
bool InCircle(const Coordinate& center, int radius, const Coordinate& target);
bool InCircle(int x, int y, int radius, const Coordinate& target);

bool HasIntersect(const RECT& rect1, const RECT& rect2);
bool HasIntersect(const Rect& rect1, const Rect& rect2);
Coordinate Intersect(const Rect& rectA, const Rect& rectB);

// char and wchar_t
const int WIDECHAR_BUFFER_SIZE = 128;
const int CHAR_BUFFER_SIZE = 128;

bool widen(wchar_t* dest, const char* src);
wchar_t* widen(const char* src);
bool narrow(char* dest, const wchar_t* src);
char* narrow(const wchar_t* src);

// string operation
void strip(std::string* str);
void strip(char* str);

const char* splice(const char* front, const char* end);
const char* splice(const char* front, char end);

// Random value.
void SetRandomSeed();
int Random(int upper);
int Random(int lower, int upper);

inline double _Random()
{
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

double Random(double upper);
double Random(double lower, double upper);

template <typename T> T Random(std::vector<T> vec)
{
    return vec[Random(static_cast<int>(vec.size()))];
}

/********************************************************************
** Emm... what to say...?
*/
template <typename T> T BlendValue(T start, T end, double ratio)
{
    return static_cast<T>(start * (1.0 - ratio) + end * ratio);
}

template <typename T> T BlendValue(T start, T end, double ratio, T (*blender)(T, T, double))
{
    return start + blender(start, end, ratio);
}

// Some default blender.
template <typename T> T LinearBlender(T start, T end, double ratio)
{
    return static_cast<T>((end - start) * ratio);
}

template <typename T> T EaseInBlender(T start, T end, double ratio)
{
    return static_cast<T>((end - start) * (1.0 - log10(10.0 - 9.0 * ratio)));
}

template <typename T> T EaseOutBlender(T start, T end, double ratio)
{
    return static_cast<T>((end - start) * log10(9.0 * ratio + 1.0));
}

#if 0 // Experimental
template<typename T>
T _EaseOutBlender(T start, T end, double ratio)
{
	// (a - b)(1 - q)^n + b
	return (start - end) * pow(0.90, ratio * 100.0) + end;
}
#endif

/********************************************************************
** Directory.
*/
bool IsValidDirectory(const char* dir);
bool IsValidDirectory(const wchar_t* dir);

// Emm.. CreateDirectory is defined in fileapi.h. :(
bool CreateNewDirectory(const char* dir);
bool CreateNewDirectory(const wchar_t* dir);

#endif
