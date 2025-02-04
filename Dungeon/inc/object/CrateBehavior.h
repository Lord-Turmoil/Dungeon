/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : CrateBehavior.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 15, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Crate behavior.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _CRATE_BEHAVIOR_H_
#define _CRATE_BEHAVIOR_H_

#include <dungine.h>

class CrateBehavior : public Behavior
{
public:
    CrateBehavior()
    {
    }

    ~CrateBehavior() override
    {
    }

    const char* Name() const override = 0;

    CrateBehavior* Clone() const override = 0;
    virtual void Clone(CrateBehavior* clone) const;

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
    enum CrateAnimTag
    {
        CRATE_ANIM_INIT,
        CRATE_ANIM_IDLE,
        CRATE_ANIM_OPENING,
        CRATE_ANIM_OPEN
    };
};

class CrateInit final : public CrateBehavior
{
public:
    const char* Name() const override
    {
        return "Init";
    }

    CrateInit* Clone() const override;

    virtual void Clone(CrateInit* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class CrateIdle final : public CrateBehavior
{
public:
    const char* Name() const override
    {
        return "Idle";
    }

    CrateIdle* Clone() const override;

    virtual void Clone(CrateIdle* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class CrateOpening final : public CrateBehavior
{
public:
    const char* Name() const override
    {
        return "Opening";
    }

    CrateOpening* Clone() const override;

    virtual void Clone(CrateOpening* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class CrateOpen final : public CrateBehavior
{
public:
    const char* Name() const override
    {
        return "Open";
    }

    CrateOpen* Clone() const override;

    virtual void Clone(CrateOpen* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }

    void OnEnter() override;
};

#endif
