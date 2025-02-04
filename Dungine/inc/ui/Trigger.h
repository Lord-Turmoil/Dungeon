/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Trigger.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 9, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Trigger of widgets.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _TRIGGER_H_
#define _TRIGGER_H_

#include "Cell.h"

/********************************************************************
** Forward declaration. Class of user event, including keyboard and
** mouse. All triggers can respond to at most one virtual key.
*/
class Event;

#ifndef VK_NONE // it will be defined in event.h
#define VK_NONE -1
#endif

enum class TriggerValue
{
    TV_NONE,

    TV_MOVE,  // mouse outside of trigger and not down
    TV_HOVER, // mouse in trigger but not down

    TV_HOLD,  // mouse down outside of trigger
    TV_PRESS, // mouse down in trigger

    TV_CLICK,   // mouse up in trigger
    TV_RELEASE, // mouse up outside of trigger

    TV_KEYDOWN,
};

/********************************************************************
** Trigger is the base element to detect events.
*/
class Trigger
{
public:
    Trigger() : m_vKey(VK_NONE)
    {
    }

    virtual ~Trigger()
    {
    }

    Trigger* SetVirtualKey(int vKey)
    {
        m_vKey = vKey;
        return this;
    }

    int GetVirtualKey() const
    {
        return m_vKey;
    }

    /*
    ** event is a keyword??? So "evnt" instead.
    */
    virtual TriggerValue Detect(Event& evnt) = 0;

    virtual void Update() = 0;

    virtual bool Load(XMLElement* node);

protected:
    /*
    ** The virtual key to response. VK_NONE(-1) for none.
    */
    int m_vKey;
};

/********************************************************************
** KeyboardTrigger only react to keyboard event.
*/
class KeyboardTrigger : public Trigger
{
public:
    KeyboardTrigger()
    {
    }

    ~KeyboardTrigger() override
    {
    }

    TriggerValue Detect(Event& evnt) override;

    void Update() override
    {
    } // Nothing for it to update. :)

    bool Load(XMLElement* node) override;

protected:
};

/********************************************************************
** Mouse trigger detects mouse events.
*/
class MouseTrigger : public Trigger
{
public:
    MouseTrigger()
    {
    }

    ~MouseTrigger() override
    {
    }

    TriggerValue Detect(Event& evnt) override = 0;

    /*
     ** Update range for loose detect.
     */
    void Update() override = 0;

    // Same as Trigger.
    // virtual bool Load(XMLElement* node);

protected:
    Rect m_range;
};

class RectTrigger : public RectCell, public MouseTrigger
{
public:
    RectTrigger()
    {
    }

    RectTrigger(int width, int height) : RectCell(width, height)
    {
    }

    ~RectTrigger() override
    {
    }

    TriggerValue Detect(Event& evnt) override;
    void Update() override;
    bool Load(XMLElement* node) override;

protected:
};

class RoundRectTrigger : public RoundRectCell, public MouseTrigger
{
public:
    RoundRectTrigger()
    {
    }

    RoundRectTrigger(int width, int height, int radius) : RoundRectCell(width, height, radius)
    {
    }

    ~RoundRectTrigger() override
    {
    }

    TriggerValue Detect(Event& evnt) override;
    void Update() override;
    bool Load(XMLElement* node) override;

protected:
};

class CircleTrigger : public CircleCell, public MouseTrigger
{
public:
    CircleTrigger()
    {
    }

    CircleTrigger(int radius) : CircleCell(radius)
    {
    }

    ~CircleTrigger() override
    {
    }

    TriggerValue Detect(Event& evnt) override;
    void Update() override;
    bool Load(XMLElement* node) override;

protected:
    Coordinate m_center;
};

class MaskTrigger : public RectCell, public MouseTrigger
{
public:
    MaskTrigger()
    {
    }

    MaskTrigger(int width, int height) : RectCell(width, height)
    {
    }

    ~MaskTrigger() override
    {
    }

    MaskTrigger* SetMask(const IMAGE* mask);

    const IMAGE* GetMask() const
    {
        return &m_mask;
    }

    TriggerValue Detect(Event& evnt) override;
    void Update() override;
    bool Load(XMLElement* node) override;

protected:
    IMAGE m_mask;
};

Trigger* LoadTrigger(XMLElement* node);

#endif
