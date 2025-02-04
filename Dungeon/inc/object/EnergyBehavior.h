/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnergyBehavior.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Energy behavior.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENERGY_BEHAVIOR_H_
#define _ENERGY_BEHAVIOR_H_

#include <dungine.h>

class EnergyBehavior : public Behavior
{
public:
    EnergyBehavior()
    {
    }

    ~EnergyBehavior() override
    {
    }

    const char* Name() const override = 0;

    EnergyBehavior* Clone() const override = 0;
    virtual void Clone(EnergyBehavior* clone) const;

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
    enum EnergyAnimTag
    {
        ENERGY_ANIM_FLY,
        ENERGY_ANIM_ACTIVE
    };
};

class EnergyFly final : public EnergyBehavior
{
public:
    EnergyFly()
    {
    }

    ~EnergyFly() override
    {
    }

    const char* Name() const override
    {
        return "Fly";
    }

    EnergyFly* Clone() const override;

    virtual void Clone(EnergyFly* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class EnergyActive final : public EnergyBehavior
{
public:
    EnergyActive()
    {
    }

    ~EnergyActive() override
    {
    }

    const char* Name() const override
    {
        return "Active";
    }

    EnergyActive* Clone() const override;

    virtual void Clone(EnergyActive* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class EnergyPerish final : public EnergyBehavior
{
public:
    EnergyPerish()
    {
    }

    ~EnergyPerish() override
    {
    }

    const char* Name() const override
    {
        return "Perish";
    }

    EnergyPerish* Clone() const override;

    virtual void Clone(EnergyPerish* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }

    void OnEnter() override;
};

#endif
