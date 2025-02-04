/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : PortalBehavior.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 4, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Behavior of Portal.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _PORTAL_BEHAVIOR_H_
#define _PORTAL_BEHAVIOR_H_

#include <dungine.h>

class PortalBehavior : public Behavior
{
public:
    PortalBehavior()
    {
    }

    ~PortalBehavior() override
    {
    }

    const char* Name() const override = 0;

    PortalBehavior* Clone() const override = 0;
    virtual void Clone(PortalBehavior* clone) const;

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
    enum PortalAnimTag
    {
        PORTAL_ANIM_INIT,
        PORTAL_ANIM_READY
    };
};

class PortalIdle final : public PortalBehavior
{
public:
    const char* Name() const override
    {
        return "Idle";
    }

    PortalIdle* Clone() const override;

    virtual void Clone(PortalIdle* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }

    void OnEnter() override;
    void OnExit() override;
};

class PortalInit final : public PortalBehavior
{
public:
    const char* Name() const override
    {
        return "Init";
    }

    PortalInit* Clone() const override;

    virtual void Clone(PortalInit* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

private:
};

class PortalReady final : public PortalBehavior
{
public:
    const char* Name() const override
    {
        return "Ready";
    }

    PortalReady* Clone() const override;

    virtual void Clone(PortalReady* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

#endif
