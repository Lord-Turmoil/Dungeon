/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Auxilliary.cpp                            *
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

#include <cstdlib>
#include <ctime>
#include <direct.h> // for _mkdir
#include <io.h>     // for _access

#include "../../inc/utility/Auxilliary.h"
#include "../../inc/utility/DMath.h"

#include "../../inc/device/Sound.h"

/******************************************************************************
 * ResizeRect -- Resize rectangle.                                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   rect                                                              *
 *          left                                                              *
 *          top                                                               *
 *          right                                                             *
 *          bottom                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
void ResizeRect(RECT* rect, int left, int top, int right, int bottom)
{
    rect->left = left;
    rect->top = top;
    rect->right = right;
    rect->bottom = bottom;
}

/******************************************************************************
 * InRectangle -- Check if a target is in rectangle.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   rect   -- The range rectangle.                                    *
 *          target -- The target coordinate.                                  *
 *                                                                            *
 * OUTPUT:  Return if in range.                                               *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
bool InRectangle(const RECT& rect, const Coordinate& target)
{
    return ((rect.left <= target.x) && (target.x <= rect.right) && (rect.top <= target.y) && (target.y <= rect.bottom));
}

bool InRectangleStrict(const RECT& rect, const Coordinate& target)
{
    return ((rect.left < target.x) && (target.x < rect.right) && (rect.top < target.y) && (target.y < rect.bottom));
}

bool InRectangle(const Rect& rect, const Coordinate& target)
{
    return ((rect.pos.x <= target.x) && (target.x < rect.pos.x + rect.width) && (rect.pos.y <= target.y) &&
            (target.y < rect.pos.y + rect.height));
}

bool InRectangleStrict(const Rect& rect, const Coordinate& target)
{
    return ((rect.pos.x < target.x) && (target.x < rect.pos.x + rect.width - 1) && (rect.pos.y < target.y) &&
            (target.y < rect.pos.y + rect.height - 1));
}

/******************************************************************************
 * InRoundRect -- Check if a target is in roundrectangle.                     *
 *                                                                            *
 *    It will first check if the target is in rectangle or not. Then it will  *
 *    check if the target is in the fissure around the corner.                *
 *                                                                            *
 * INPUT:   rect   -- The outter rectangle of roundrectangle.                 *
 *          radius -- The border radius of round rectangle.                   *
 *          target -- The target coordinate.                                  *
 *                                                                            *
 * OUTPUT:  Return if in range.                                               *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
bool InRoundRect(const RECT& rect, int radius, const Coordinate& target)
{
    if (!InRectangle(rect, target))
    {
        return false;
    }

    int dx = target.x - rect.left;
    int dy = target.y - rect.top;
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int cx, cy;

    if (dx < radius)
    {
        cx = rect.left + radius;
        if (dy < radius)
        {
            cy = rect.top + radius;
        }
        else if (dy > width - radius)
        {
            cy = rect.bottom - radius;
        }
        else
        {
            return true;
        }
    }
    else if (dx > width - radius)
    {
        cx = rect.right - radius;
        if (dy < radius)
        {
            cy = rect.top + radius;
        }
        else if (dy > width - radius)
        {
            cy = rect.bottom - radius;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }

    return InCircle(cx, cy, radius, target);
}

bool InRoundRect(const Rect& rect, int radius, const Coordinate& target)
{
    if (!InRectangle(rect, target))
    {
        return false;
    }

    int dx = target.x - rect.x;
    int dy = target.y - rect.y;
    int width = rect.width;
    int height = rect.height;
    int cx, cy;

    if (dx < radius)
    {
        cx = rect.x + radius;
        if (dy < radius)
        {
            cy = rect.y + radius;
        }
        else if (dy > width - radius)
        {
            cy = rect.y + height - radius;
        }
        else
        {
            return true;
        }
    }
    else if (dx > width - radius)
    {
        cx = rect.x + width - radius;
        if (dy < radius)
        {
            cy = rect.y + radius;
        }
        else if (dy > width - radius)
        {
            cy = rect.y + height - radius;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }

    return InCircle(cx, cy, radius, target);
}

/******************************************************************************
 * InCircle -- Check if a target is in circle.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   center -- The center of circle.                                   *
 *          radius -- The radius of circle.                                   *
 *          target -- The target coordinate.                                  *
 *                                                                            *
 * OUTPUT:  Return if in range.                                               *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
bool InCircle(const Coordinate& center, int radius, const Coordinate& target)
{
    return Distance(center, target) < radius;
}

/******************************************************************************
 * InCircle -- Check if a target is in circle.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   x      -- The x coordinate of circle.                             *
 *          y      -- The y coordinate of circle.                             *
 *          radius -- The radius of circle.                                   *
 *          target -- The target coordinate.                                  *
 *                                                                            *
 * OUTPUT:  Return if in range.                                               *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
bool InCircle(int x, int y, int radius, const Coordinate& target)
{
    return Distance(x, y, target.x, target.y);
}

/******************************************************************************
 * HasIntersect -- Check if two rectangles are intersect with each other.     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   rect1                                                             *
 *          rect2                                                             *
 *                                                                            *
 * OUTPUT:  Whether intersect or not.                                         *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
bool HasIntersect(const RECT& rect1, const RECT& rect2)
{
    return !((rect1.right < rect2.left) || (rect1.left > rect2.right) || (rect1.bottom < rect2.top) ||
             (rect1.top > rect2.bottom));
}

bool HasIntersect(const Rect& rect1, const Rect& rect2)
{
    return !((rect1.pos.x + rect1.width < rect2.pos.x) || (rect1.pos.y + rect1.height < rect2.pos.y) ||
             (rect2.pos.x + rect2.width < rect1.pos.x) || (rect2.pos.y + rect2.height < rect1.pos.y));
}

Coordinate Intersect(const Rect& rectA, const Rect& rectB)
{
    Coordinate overlap;

    if (rectA.x < rectB.x)
    {
        overlap.x = rectA.x + rectA.width - rectB.x;
    }
    else
    {
        overlap.x = rectB.x + rectB.width - rectA.x;
    }

    if (rectA.y < rectB.y)
    {
        overlap.y = rectA.y + rectA.height - rectB.y;
    }
    else
    {
        overlap.y = rectB.y + rectB.height - rectA.y;
    }

    return overlap;
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** String conversion.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/******************************************************************************
 * widen -- Widen char to wchar_t.                                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   dest -- The destination wchar_t buffer.                           *
 *          src  -- The source char buffer.                                   *
 *                                                                            *
 * OUTPUT:  Return whether successful or not.                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/02/13 Tony : Created.                                               *
 *============================================================================*/
bool widen(wchar_t* dest, const char* src)
{
    size_t ret;
    errno_t err = mbstowcs_s(&ret, dest, strlen(src) + 1, src, WIDECHAR_BUFFER_SIZE);

    return err ? false : true;
}

/******************************************************************************
 * widen -- Widen char to wchar_t.                                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   src  -- The source char buffer.                                   *
 *                                                                            *
 * OUTPUT:  Return wchar_t string. nullptr if convertion failed.              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/02/13 Tony : Created.                                               *
 *============================================================================*/
wchar_t* widen(const char* src)
{
    static wchar_t dest[WIDECHAR_BUFFER_SIZE];

    size_t ret;
    errno_t err = mbstowcs_s(&ret, dest, strlen(src) + 1, src, WIDECHAR_BUFFER_SIZE);

    return err ? nullptr : dest;
}

/******************************************************************************
 * narrow -- Narrow wchar_t to char.                                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   dest -- The destination char buffer.                              *
 *          src  -- The source char buffer.                                   *
 *                                                                            *
 * OUTPUT:  Return whether successful or not.                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/12 Tony : Created.                                               *
 *============================================================================*/
bool narrow(char* dest, const wchar_t* src)
{
    size_t ret;
    errno_t err = wcstombs_s(&ret, dest, wcslen(src) + 1, src, CHAR_BUFFER_SIZE);

    return err ? false : true;
}

/******************************************************************************
 * narrow -- Narrow wchar_t to char.                                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   src  -- The source wchar_t buffer.                                *
 *                                                                            *
 * OUTPUT:  Return char string. nullptr if convertion failed.                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/12 Tony : Created.                                               *
 *============================================================================*/
char* narrow(const wchar_t* src)
{
    static char dest[WIDECHAR_BUFFER_SIZE];

    size_t ret;
    errno_t err = wcstombs_s(&ret, dest, wcslen(src) + 1, src, CHAR_BUFFER_SIZE);

    return err ? nullptr : dest;
}

/******************************************************************************
 * strip -- Strip the white character at both end of the string.              *
 *                                                                            *
 *    Clear the white space at both end of the string, mainly used when load  *
 *    the text in xml tags.                                                   *
 *                                                                            *
 * INPUT:   str -- The string.                                                *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
void strip(std::string* str)
{
    while ((!str->empty()) && isspace(str->back()))
    {
        str->pop_back();
    }
    while ((!str->empty()) && isspace(str->front()))
    {
        str->erase(str->begin());
    }
}

void strip(char* str)
{
    int back = static_cast<int>(strlen(str));
    int front = 0;

    while ((back > 0) && isspace(str[back - 1]))
    {
        back--;
    }
    while ((front < back) && isspace(str[front]))
    {
        front++;
    }
    for (int i = 0, j = front; j < back; i++, j++)
    {
        str[i] = str[j];
    }
    str[back - front] = '\0';
}

/******************************************************************************
 * splice -- Splice two C-style string temporarily.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   front                                                             *
 *          end                                                               *
 *                                                                            *
 * OUTPUT:  Return spliced string.                                            *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/30 Tony : Created.                                               *
 *============================================================================*/
static char spliceResult[CHAR_BUFFER_SIZE];

const char* splice(const char* front, const char* end)
{
    int k = 0;
    int len;

    len = static_cast<int>(strlen(front));
    for (int i = 0; i < len; i++)
    {
        spliceResult[k++] = front[i];
    }
    len = static_cast<int>(strlen(end));
    for (int i = 0; i < len; i++)
    {
        spliceResult[k++] = end[i];
    }
    spliceResult[k] = '\0';

    return spliceResult;
}

const char* splice(const char* front, const char end)
{
    int k = 0;
    int len = static_cast<int>(strlen(front));

    for (int i = 0; i < len; i++)
    {
        spliceResult[k++] = front[i];
    }
    spliceResult[k++] = end;
    spliceResult[k] = '\0';

    return spliceResult;
}

/******************************************************************************
 * ResetRandomSeed -- Reset random seed.                                      *
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
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void SetRandomSeed()
{
    srand(static_cast<unsigned>(time(nullptr)));
}

/******************************************************************************
 * Random -- Get a random number.                                             *
 *                                                                            *
 *    Get a random nuber in [lower, upper).                                   *
 *                                                                            *
 * INPUT:   lower -- If not assigned, the lower will be zero.                 *
 *          upper -- The upper bound of the result. upper not included.       *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
int Random(int upper)
{
    return (upper == 0) ? (0) : (rand() % upper);
}

int Random(int lower, int upper)
{
    if (upper <= lower)
    {
        return lower;
    }

    return lower + rand() % (upper - lower);
}

double Random(double upper)
{
    return _Random() * upper;
}

double Random(double lower, double upper)
{
    return lower + _Random() * (upper - lower);
}

/******************************************************************************
 * IsValidDirectory -- Check if the directory is valid or not.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   dir                                                               *
 *                                                                            *
 * OUTPUT:  Return valid or not.                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/11 Tony : Created.                                               *
 *============================================================================*/
bool IsValidDirectory(const char* dir)
{
    return (_access(dir, 0) == 0);
}

bool IsValidDirectory(const wchar_t* dir)
{
    return (_waccess(dir, 0) == 0);
}

/******************************************************************************
 * CreateNewDirectory -- Create a new directory.                              *
 *                                                                            *
 *    CreateDirectory() is occupied. :(                                       *
 *                                                                            *
 * INPUT:   dir -- The new directory.                                         *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/11 Tony : Created.                                               *
 *============================================================================*/
bool CreateNewDirectory(const char* dir)
{
    if (_mkdir(dir) == 0)
    {
        return true;
    }

    LOG_ERROR(CANNOT_CREATE_DIR, dir);

    return false;
}

bool CreateNewDirectory(const wchar_t* dir)
{
    if (_wmkdir(dir) == 0)
    {
        return true;
    }

    LOG_ERROR(CANNOT_CREATE_DIR, narrow(dir));

    return false;
}
