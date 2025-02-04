/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : WeaponBehavior.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : Julyt 19, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For common weapon behaviors.                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _WEAPON_BEHAVIOR_H_
#define _WEAPON_BEHAVIOR_H_

#include <dungine.h>

class WeaponBehavior : public Behavior
{
public:
    WeaponBehavior()
    {
    }

    ~WeaponBehavior() override
    {
    }

    const char* Name() const override = 0;

    WeaponBehavior* Clone() const override = 0;
    virtual void Clone(WeaponBehavior* clone) const;

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
    virtual void _Fire();

protected:
    enum WeaponAnimTag
    {
        WEAPON_ANIM_IDLE = 0,
        WEAPON_ANIM_READY,
        WEAPON_ANIM_FIRE,
        WEAPON_ANIM_COOLING,
        WEAPON_ANIM_CUSTOM_START
    };
};

class WeaponIdle : public WeaponBehavior
{
public:
    const char* Name() const override
    {
        return "Idle";
    }

    WeaponIdle* Clone() const override;

    virtual void Clone(WeaponIdle* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class WeaponUnequipped : public WeaponBehavior
{
public:
    const char* Name() const override
    {
        return "Unequipped";
    }

    WeaponUnequipped* Clone() const override;

    virtual void Clone(WeaponUnequipped* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }

    void OnEnter() override;
};

// This is uncertain.
class WeaponReady : public WeaponBehavior
{
public:
    const char* Name() const override
    {
        return "Ready";
    }

    WeaponReady* Clone() const override = 0;
    virtual void Clone(WeaponReady* clone) const;

    void Update(Event* evnt) override = 0;

    void OnEnter() override;
};

class WeaponFire : public WeaponBehavior
{
public:
    const char* Name() const override
    {
        return "Fire";
    }

    WeaponFire* Clone() const override;

    virtual void Clone(WeaponFire* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class WeaponCooling : public WeaponBehavior
{
public:
    const char* Name() const override
    {
        return "Cooling";
    }

    WeaponCooling* Clone() const override;

    virtual void Clone(WeaponCooling* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

private:
    // No need to clone.
    clock_t m_coolingTime;
    clock_t m_elapsedTime;
};

#endif
