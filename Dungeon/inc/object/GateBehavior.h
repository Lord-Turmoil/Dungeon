/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GateComponent.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 26, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For gate component.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _GATE_BEHAVIOR_H_
#define _GATE_BEHAVIOR_H_

#include <dungine.h>

/********************************************************************
** Behaviors of gate.
*/
class GateBehavior : public Behavior
{
public:
    GateBehavior()
    {
    }

    ~GateBehavior() override
    {
    }

    const char* Name() const override = 0;

    GateBehavior* Clone() const override = 0;
    virtual void Clone(GateBehavior* clone) const;

    bool Load(XMLElement* node) override
    {
        return true;
    }

    void Update(Event* evnt) override = 0;

    void OnEnter() override
    {
    }

    void OnExit() override
    {
    }

protected:
    enum GateAnimTag
    {
        GATE_ANIM_CLOSED = 0,
        GATE_ANIM_OPENING,
        GATE_ANIM_OPEN,
        GATE_ANIM_CLOSING
    };
};

class GateOpen : public GateBehavior
{
public:
    GateOpen()
    {
    }

    ~GateOpen() override
    {
    }

    const char* Name() const override
    {
        return "Open";
    }

    GateOpen* Clone() const override;

    virtual void Clone(GateOpen* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class GateClosed : public GateBehavior
{
public:
    GateClosed()
    {
    }

    ~GateClosed() override
    {
    }

    const char* Name() const override
    {
        return "Closed";
    }

    GateClosed* Clone() const override;

    virtual void Clone(GateClosed* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class GateOpening : public GateBehavior
{
public:
    GateOpening()
    {
    }

    ~GateOpening() override
    {
    }

    const char* Name() const override
    {
        return "Opening";
    }

    GateOpening* Clone() const override;

    virtual void Clone(GateOpening* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class GateClosing : public GateBehavior
{
public:
    GateClosing()
    {
    }

    ~GateClosing() override
    {
    }

    const char* Name() const override
    {
        return "Closing";
    }

    GateClosing* Clone() const override;

    virtual void Clone(GateClosing* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

#endif
