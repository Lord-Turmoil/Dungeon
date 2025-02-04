/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BowBehavior.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Bow behavior.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _BOW_BEHAVIOR_H_
#define _BOW_BEHAVIOR_H_

#include "WeaponBehavior.h"

/********************************************************************
** Bow behaviors:
**  Ready
**  Charge
**  Discharge
**  Hold
*/
class BowBehavior : public WeaponBehavior
{
public:
    BowBehavior()
    {
    }

    ~BowBehavior() override
    {
    }

    const char* Name() const override = 0;

    BowBehavior* Clone() const override = 0;
    virtual void Clone(BowBehavior* clone) const;

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
    enum BowAnimTag
    {
        BOW_ANIM_CHARGE = WEAPON_ANIM_CUSTOM_START,
        BOW_ANIM_DISCHARGE,
        BOW_ANIM_HOLD, // fully charged
    };
};

class BowReady : public BowBehavior
{
public:
    const char* Name() const override
    {
        return "Ready";
    }

    BowReady* Clone() const override;

    virtual void Clone(BowReady* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class BowCharge : public BowBehavior
{
public:
    const char* Name() const override
    {
        return "Charge";
    }

    BowCharge* Clone() const override;

    virtual void Clone(BowCharge* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class BowDischarge : public BowBehavior
{
public:
    const char* Name() const override
    {
        return "Discharge";
    }

    BowDischarge* Clone() const override;

    virtual void Clone(BowDischarge* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class BowHold : public BowBehavior
{
public:
    const char* Name() const override
    {
        return "Hold";
    }

    BowHold* Clone() const override;

    virtual void Clone(BowHold* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;
};

#endif
