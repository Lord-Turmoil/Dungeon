/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : DDefines.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 31, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For the defines of the types in the project. All enum types should be    *
 *   defined here.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

// Common.h is a common header file name, so add D for Dungine.
#ifndef _DDEFINES_H_
#define _DDEFINES_H_

#include <Windows.h>
#include <ctime>
#include <easyx.h>

/********************************************************************
** Some global constants.
** Some of them comes from Arduino.h :)
*/
const double EPSILON = 1e-5;

const double EULER = 2.7182818284590452353602874713527;
const double PI = 3.1415926535897932384626433832795;
const double HALF_PI = 1.5707963267948966192313216916398;
const double TWO_PI = 6.283185307179586476925286766559;

const double DEG_TO_RAD = 0.017453292519943295769236907684886;
const double RAD_TO_DEG = 57.295779513082320876798154814105;

const double G = 9.8;

const int INF_INT = 0x3f3f3f3f;
const double INF_DOUBLE = 1e6;

// This is the quality of rotateimage().
const bool HIGH_QUALITY = false;

// The transparent color of images. This is fixed in the program.
const COLORREF TRANSPARENT_COLOR = 0x00ff00ff;

struct Coordinate
{
    int x;
    int y;

    Coordinate() : x(0), y(0)
    {
    }

    explicit Coordinate(int _x) : x(_x), y(_x)
    {
    }

    Coordinate(int _x, int _y) : x(_x), y(_y)
    {
    }

    Coordinate(const Coordinate& obj) : x(obj.x), y(obj.y)
    {
    }

    Coordinate(const Coordinate&& obj) noexcept : x(obj.x), y(obj.y)
    {
    }

    bool IsZero() const
    {
        return ((x == 0) && (y == 0));
    }

    bool Positive() const
    {
        return ((x > 0) && (y > 0));
    }

    void Init(int _x, int _y)
    {
        x = _x;
        y = _y;
    }

    void Init(const Coordinate& obj)
    {
        x = obj.x;
        y = obj.y;
    }

    void Mirror()
    {
        x = -x;
        y = -y;
    }

    void MirrorX()
    {
        x = -x;
    }

    void MirrorY()
    {
        y = -y;
    }

    Coordinate GetMirror() const
    {
        return { -x, -y };
    }

    Coordinate GetMirrorX() const
    {
        return { -x, y };
    }

    Coordinate GetMirrorY() const
    {
        return { x, -y };
    }

    Coordinate& operator=(const Coordinate& obj)
    {
        this->x = obj.x;
        this->y = obj.y;

        return *this;
    };

    Coordinate operator+(const Coordinate& obj) const
    {
        return { this->x + obj.x, this->y + obj.y };
    };

    Coordinate operator-(const Coordinate& obj) const
    {
        return { this->x - obj.x, this->y - obj.y };
    };

    Coordinate operator-() const
    {
        return { -this->x, -this->y };
    };

    Coordinate& operator+=(const Coordinate& obj)
    {
        this->x += obj.x;
        this->y += obj.y;

        return *this;
    };

    Coordinate& operator-=(const Coordinate& obj)
    {
        this->x -= obj.x;
        this->y -= obj.y;

        return *this;
    };

    bool operator==(const Coordinate& obj) const
    {
        return (this->x == obj.x) && (this->y == obj.y);
    }

    bool operator!=(const Coordinate& obj) const
    {
        return (this->x != obj.x) || (this->y != obj.y);
    }

    friend Coordinate operator*(const int k, const Coordinate& coord)
    {
        return { coord.x * k, coord.y * k };
    };

    friend Coordinate operator*(const Coordinate& coord, const int k)
    {
        return { coord.x * k, coord.y * k };
    };

    Coordinate& operator*=(const int k)
    {
        *this = *this * k;
        return *this;
    };

    friend Coordinate operator*(const double k, const Coordinate& coord)
    {
        return { static_cast<int>(coord.x * k), static_cast<int>(coord.y * k) };
    };

    friend Coordinate operator*(const Coordinate& coord, const double k)
    {
        return { static_cast<int>(coord.x * k), static_cast<int>(coord.y * k) };
    };

    Coordinate& operator*=(const double k)
    {
        *this = *this * k;
        return *this;
    };

    friend Coordinate operator*(const float k, const Coordinate& coord)
    {
        return { static_cast<int>(coord.x * k), static_cast<int>(coord.y * k) };
    };

    friend Coordinate operator*(const Coordinate& coord, const float k)
    {
        return { static_cast<int>(coord.x * k), static_cast<int>(coord.y * k) };
    };

    Coordinate& operator*=(const float k)
    {
        *this = *this * k;
        return *this;
    };

    friend Coordinate operator/(const Coordinate& coord, const int k)
    {
        return { coord.x / k, coord.y / k };
    };

    Coordinate& operator/=(const int k)
    {
        *this = *this / k;
        return *this;
    };

    friend Coordinate operator/(const Coordinate& coord, const double k)
    {
        return { static_cast<int>(coord.x / k), static_cast<int>(coord.y / k) };
    };

    Coordinate& operator/=(const double k)
    {
        *this = *this / k;
        return *this;
    };

    friend Coordinate operator/(const Coordinate& coord, const float k)
    {
        return { static_cast<int>(coord.x / k), static_cast<int>(coord.y / k) };
    };

    Coordinate& operator/=(const float k)
    {
        *this = *this / k;
        return *this;
    };
};

struct CompareCoordinate
{
    bool operator()(const Coordinate& c1, const Coordinate& c2) const
    {
        if (c1.x == c2.x)
        {
            return c1.y < c2.y;
        }
        else
        {
            return c1.x < c2.x;
        }
    };
};

/*
** Parse a coordinate(or just a pair) from a string. Used
** as a callback function.
*/
bool ParseCoord(void* val, const char* attribute);

/********************************************************************
** Some unit coordinates. P is for positive, N is for negative.
*/
const Coordinate COORD_PX = { 1, 0 };
const Coordinate COORD_NX = { -1, 0 };
const Coordinate COORD_PY = { 0, 1 };
const Coordinate COORD_NY = { 0, -1 };
const Coordinate COORD_ZERO = { 0, 0 };

struct Vector
{
    double x;
    double y;

    Vector() : x(0.0), y(0.0)
    {
    }

    explicit Vector(double _x) : x(_x), y(_x)
    {
    }

    Vector(double _x, double _y) : x(_x), y(_y)
    {
    }

    Vector(const Vector& obj) : x(obj.x), y(obj.y)
    {
    }

    void Init(double _x, double _y)
    {
        x = _x;
        y = _y;
    }

    void Init(const Vector& vec)
    {
        x = vec.x;
        y = vec.y;
    }

    void Mirror()
    {
        x = -x;
        y = -y;
    }

    void MirrorX()
    {
        x = -x;
    }

    void MirrorY()
    {
        y = -y;
    }

    Vector GetMirror() const
    {
        return { -x, -y };
    }

    Vector GetMirrorX() const
    {
        return { -x, y };
    }

    Vector GetMirrorY() const
    {
        return { x, -y };
    }

    double Module() const;
    Vector Unit() const;

    void Rotate(double radian);

    bool operator==(const Vector& obj) const
    {
        /*
        ** WARNING!!!
        ** Here, I'm too lazy to use a precision to compare two
        ** double numbers. If it doesn't run properly, just
        ** make a little change to it. :)
        */
        return (this->x == obj.x) && (this->y == obj.y);
    };

    Vector& operator=(const Vector& obj)
    {
        this->x = obj.x;
        this->y = obj.y;

        return *this;
    };

    Vector operator+(const Vector& obj) const
    {
        return { this->x + obj.x, this->y + obj.y };
    };

    Vector operator-(const Vector& obj) const
    {
        return { this->x - obj.x, this->y - obj.y };
    };

    Vector operator-() const
    {
        return { -this->x, -this->y };
    };

    Vector& operator+=(const Vector& obj)
    {
        this->x += obj.x;
        this->y += obj.y;
        return *this;
    };

    Vector& operator-=(const Vector& obj)
    {
        this->x -= obj.x;
        this->y -= obj.y;
        return *this;
    }

    Vector& operator*=(const double k)
    {
        this->x *= k;
        this->y *= k;
        return *this;
    }

    Vector& operator/=(const double k)
    {
        this->x /= k;
        this->y /= k;
        return *this;
    }

    /*
    ** 2022/01/20
    ** Here... it is the inner product. :(
    */
    double operator*(const Vector& obj)
    {
        return this->x * obj.x + this->y * obj.y;
    };

    friend Vector operator*(const double k, const Vector& obj)
    {
        return { k * obj.x, k * obj.y };
    };

    friend Vector operator*(const Vector& obj, const double k)
    {
        return { k * obj.x, k * obj.y };
    };

    friend Vector operator/(const Vector& obj, const double k)
    {
        return { obj.x / k, obj.y / k };
    };

    friend double operator*(const Vector& v1, const Vector& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }
};

const Vector VECTOR_PX = { 1.0, 0.0 };
const Vector VECTOR_NX = { -1.0, 0.0 };
const Vector VECTOR_PY = { 0.0, 1.0 };
const Vector VECTOR_NY = { 0.0, -1.0 };
const Vector VECTOR_ZERO = { 0.0, 0.0 };

/********************************************************************
** Rectangle for the size of an area. Rectange() is a function in
** wingdi.h... :(
*/
struct Rect
{
    union
    {
        Coordinate pos;

        struct
        {
            int x, y;
        };
    };

    int width;
    int height;

    static RECT m_rect;

    Rect(int _x = 0, int _y = 0, int _width = 0, int _height = 0) : x(_x), y(_y), width(_width), height(_height)
    {
    }

    Rect(const Rect& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
    {
    }

    bool IsValid() const
    {
        return ((width > 0) && (height > 0));
    }

    void SetPos(const Coordinate& _pos)
    {
        pos = _pos;
    }

    void SetPos(int _x, int _y)
    {
        x = _x;
        y = _y;
    }

    void Reset(const Rect& rect)
    {
        pos = rect.pos;
        width = rect.width;
        height = rect.height;
    }

    void Reset(int _x, int _y, int _width, int _height)
    {
        pos.x = _x;
        pos.y = _y;
        width = _width;
        height = _height;
    }

    void Reset(const Coordinate& _pos, int _width, int _height)
    {
        pos = _pos;
        width = _width;
        height = _height;
    }

    void Resize(int _width, int _height)
    {
        width = _width;
        height = _height;
    }

    void Translate(const Coordinate& offset)
    {
        pos += offset;
    }

    Coordinate GetCenter() const
    {
        return Coordinate(x + (width >> 1), y + (height >> 1));
    }

    /*
    ** Get an EasyX rectangle. I assume that there will be only
    ** one such rectangle at a time.
    */
    RECT* GetEasyXRect()
    {
        m_rect.left = pos.x;
        m_rect.top = pos.y;
        m_rect.right = pos.x + width;
        m_rect.bottom = pos.y + height;
        return &m_rect;
    }

    void GetEasyXRect(RECT* rect)
    {
        rect->left = pos.x;
        rect->top = pos.y;
        rect->right = pos.x + width;
        rect->bottom = pos.y + height;
    }

    Rect& operator=(const Rect& rect)
    {
        x = rect.x;
        y = rect.y;
        width = rect.width;
        height = rect.height;

        return *this;
    };

    Rect operator*(const double k)
    {
        return Rect(static_cast<int>(pos.x * k), static_cast<int>(pos.y * k), static_cast<int>(width * k),
                    static_cast<int>(height * k));
    }
};

/********************************************************************
** Default layer settings.
*/
enum DefaultLayer : int
{
    LAYER_NORMAL = 0,
    LAYER_MIDDLE = 10,
    LAYER_HIGH = 20,
    LAYER_TOP = 50,
    LAYER_UI = 100,
    LAYER_BACK = -10
};

enum class WidgetType
{
    UNKNOWN = 0,

    BUTTON,
    PROGRESSBAR,
    RADIO,
    RADIOLIST,
    SLIDER,
    STATIC,
    TEXTAREA, // On hold

    DETECTOR,

    AUDIO,

    WIDGET_NUM,
};

/********************************************************************
** Original UI panel size.
*/
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/********************************************************************
** Some error or warning info.
*/
#define INVALID_ATTRIBUTE     R"(Invalid attribute "%s" in "%s")"
#define MISSING_ATTRIBUTE     R"(Missing attribute "%s" in "%s")"
#define MISSING_TAG           R"(Missing tag "%s" of "%s")"
#define MISSING_TEXT          R"(Missing text in TextDrawer)"
#define TAG_MISMATCH          R"(Tag "%s" mismatch "%s")"
#define TYPE_MISMATCH         R"(Type "%s" mismatch "%s")"
#define UNKNOWN_TAG           R"(Unknown tag name "%s")"
#define UNKNOWN_TYPE          R"(Unknown type name "%s")"
#define MISSING_CHILD_ELEMENT R"(Missing child element of "%s")"

#define NAME_CONFLICT   R"(Conflict of %s name "%s" occurred)"
#define MISSING_MANAGER R"(Missing interface manager for "%s")"
#define DOUBLE_LAUNCH   R"(Double launch detected at "%s")"
#define MISSING_STARTUP R"(Missing start up interface")"
#define MISSING_WIDGET  R"(Missing widget "%s")"

#define CANNOT_LOAD       R"(Cannot load "%s")"
#define CANNOT_LOAD_XML   R"(Cannot load "%s" - Error code: %d)"
#define CANNOT_SAVE_XML   R"(Cannot load "%s" - Error code: %d)"
#define CANNOT_ACCESS_DIR R"(Cannot access "%s")"
#define CANNOT_CREATE_DIR R"(Cannot create "%s")"
#define FAILED_TO_LOAD    R"(Failed to load "%s")"

#define FAILED_TO_LOAD_RESOURCE R"(Failed to load resource "%s")"
#define RESOURCE_MISMATCH       R"(Resource type mismatch at "%s")"
#define MISSING_REFERENCE       R"(Missing resource reference of "%s")"
#define CANNOT_MAKE_RESOURCE    R"(Cannot make internal resource - id: %s)"
#define RESOURCE_CONFLICT       R"(Resource id conflict occurred - id: %s)"
#define INVALID_RESOURCE        R"(Invalid resource directory "%s")"
#define INVALID_RESOURCE_ID     R"(Invalid resource id "%s")"
#define RELEASE_UNREFERENCED    R"(Attempt to release unreferenced resource - id: %s)"

#define RESOURCE_LOAD   R"(Resource "%s" loaded)"
#define RESOURCE_UNLOAD R"(Resource "%s" unloaded)"

#define CANNOT_INIT_SPEAKER    R"(Cannot initialize speaker :()"
#define CANNOT_CLOSE_SPEAKER   R"(Cannot close speaker :()"
#define CANNOT_RELEASE_SPEAKER R"(Cannot release speaker :()"

#define FAILED_TO_LOAD_UI     R"(Failed to load ui)"
#define FAILED_TO_LOAD_UI_AT  R"(Failed to load ui at "%s")"
#define FAILED_TO_LAUNCH_INTF R"(Failed to launch interface "%s")"
#define MISSING_INTERFACE     R"(Missing interface "%s")"
#define MISSING_WIDGET        R"(Missing widget "%s")"

#define CRITICAL_ERROR R"(CRITICAL ERROR ENCOUNTERED!!!)"
#define INTERNAL_ERROR R"(INTERNAL ERROR ENCOUTNERED!!!)"

// Game fps
const int DUNGINE_FPS = 60;
const int DUNGINE_DELAY = 1000 / DUNGINE_FPS;

// Interface
const clock_t DEFAULT_TRANSIT_DURATION = 500;

/********************************************************************
** Learned from RedAlert.DLL. :)
** Run-Time Type Identification.
*/
enum class RTTIType
{
    RTTI_NONE,

    RTTI_ANIMATION,
    RTTI_BEHAVIOR,
    RTTI_COMPONENT,
    RTTI_IMAGE,
    RTTI_OBJECT,

    RTTI_COUNT
};

/********************************************************************
** Type of components. These are sorted by updating order. Component
** of higher priority's type are smaller. However, this is by default,
** update order can be assigned manually.
*/
enum class ComponentType : int
{
    COMP_UNKNOWN = 0,

    COMP_PROPERTY, // won't change actively

    COMP_RIGIDBODY,
    COMP_COLLIDEBOX,

    COMP_STATE,

    COMP_MOVE,

    COMP_ANIM,
    COMP_SOUND,
    // COMP_EFFECT,

    COMP_NUM
};

enum Direction : unsigned
{
    DIR_NONE = 0,
    DIR_LEFT = 1,
    DIR_RIGHT = 2,
    DIR_UP = 4,
    DIR_DOWN = 8,
};

#endif
