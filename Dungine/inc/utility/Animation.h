/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Animation.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 9, 2022                              *
 *                                                                            *
 *                    Last Update : November 29, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Provide animation for game objects.                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <vector>

#include "../common/Common.h"
#include "tinyxml.h"

/********************************************************************
** The animation system of Dungine consists of three major parts.
** 1. A Motion alone is a complete image series, and only stores the
**    images of the motion. It contains all directions of the motion.
**    Each motion can have different frame rate.
** 2. A MotionSet contains a series of motions. It represents all the
**    motions a game object has. Also, it only stores the image, so
**    all game objects with same motions can share one MotionSet.
** 3. Animation is the manager of motions, it possesses a MotionSet
**    and deliver the image to the game object each frame.
**
** Notice:
**   For now, they are not ready for override.
**   All of them don't check if index is valid or not.
**   All of them do not have ability to avoid mis-use. :(
*/

enum AnimDirection : int
{
    ANIM_LEFT = 0,
    ANIM_RIGHT,
    ANIM_UP,
    ANIM_DOWN,

    ANIM_DIR_NUM
};

/*
** Motion is only accessible for MotionSet.
*/
class MonoMotion
{
    friend class Motion;

public:
    MonoMotion()
    {
    }

    MonoMotion(IMAGE* pImage, int srcX, int srcY, int frameNum, int frameWidth, int frameHeight);

    ~MonoMotion()
    {
    }

private:
    bool Load(IMAGE* pImage, int srcX, int srcY, int frameNum, int frameWidth, int frameHeight);

    IMAGE* GetFrame(int frame)
    {
        return &m_frame[frame];
    }

private:
    std::vector<IMAGE> m_frame;
};

class Motion
{
    friend class MotionSet;
    friend class Animation;

public:
    Motion() : m_frameSpeed(0), m_frameNum(0), m_isLoop(false)
    {
    }

    Motion(XMLElement* node, IMAGE* pImage, int frameWidth, int frameHeight);
    ~Motion();

private:
    bool Load(XMLElement* node, IMAGE* pImage, int frameWidth, int frameHeight);

    IMAGE* GetFrame(int frame, AnimDirection dir = static_cast<AnimDirection>(0));

    int GetFrameNum() const
    {
        return m_frameNum;
    }

    clock_t GetFrameSpeed() const
    {
        return m_frameSpeed;
    }

private:
    std::vector<MonoMotion*> m_pMonoMotion;
    clock_t m_frameSpeed;
    int m_frameNum;
    bool m_isLoop;
};

/********************************************************************
** 2022/11/29 TS:
** Added frame width and height.
*/
class MotionSet
{
    friend class Animation;

public:
    MotionSet() : m_motionNum(0), m_frameWidth(0), m_frameHeight(0)
    {
    }

    ~MotionSet();

    bool Load(XMLElement* node);
    Motion* GetMotion(int motion);

public:
    int GetFrameWidth() const
    {
        return m_frameWidth;
    }

    int GetFrameHeight() const
    {
        return m_frameHeight;
    }

private:
    std::vector<Motion*> m_pMotion;
    int m_motionNum;
    int m_frameWidth;
    int m_frameHeight;
};

class Animation
{
    friend class AnimComponent;

public:
    Animation();
    Animation(MotionSet* pMotionSet);
    ~Animation();

    void Initialize(MotionSet* pMotionSet);
    void Reset();

    void Update();
    IMAGE* GetFrame();

    bool IsOver() const
    {
        return m_isOver;
    }

public:
    void SetMotion(int motion);

    void SetFrame(int frame)
    {
        m_curFrame = frame;
    } // deprecated

    void SetDir(AnimDirection dir)
    {
        m_curDir = dir;
    }

public:
    int GetFrameWidth() const;
    int GetFrameHeight() const;

    int GetCurrentFrameID() const
    {
        return m_curFrame;
    }

    int GetTotalFrameNum() const
    {
        return m_frameNum;
    }

    clock_t GetDuration() const
    {
        return m_frameSpeed * m_frameNum;
    }

private:
    MotionSet* m_pMotionSet; // This must be initialized!
    Motion* m_pCurMotion;    // pointer to current motion
    clock_t m_elapsedTime;

    /*
    ** These two and m_pMotion are got when motion change
    ** occurs to reduce function calls.
    */
    clock_t m_frameSpeed;
    int m_frameNum;

    int m_curMotion;
    int m_curFrame;
    AnimDirection m_curDir;

    bool m_isLoop;
    bool m_isOver;
};

#endif
