/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : DrugBehavior.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Drug behavior.                                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _DRUG_BEHAVIOR_H_
#define _DRUG_BEHAVIOR_H_

#include <dungine.h>

class DrugBehavior : public Behavior
{
public:
    DrugBehavior()
    {
    }

    ~DrugBehavior() override
    {
    }

    const char* Name() const override = 0;

    DrugBehavior* Clone() const override = 0;
    virtual void Clone(DrugBehavior* clone) const;

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
    enum DrugAnimTag
    {
        DRUG_ANIM_IDLE,
        DRUG_ANIM_ACTIVE
    };
};

class DrugIdle : public DrugBehavior
{
public:
    DrugIdle()
    {
    }

    ~DrugIdle() override
    {
    }

    const char* Name() const override
    {
        return "Idle";
    }

    DrugIdle* Clone() const override;

    virtual void Clone(DrugIdle* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class DrugActive : public DrugBehavior
{
public:
    DrugActive()
    {
    }

    ~DrugActive() override
    {
    }

    const char* Name() const override
    {
        return "Active";
    }

    DrugActive* Clone() const override = 0;
    virtual void Clone(DrugActive* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    virtual void _Activate() = 0;
};

class HPDrugActive : public DrugActive
{
public:
    HPDrugActive()
    {
    }

    ~HPDrugActive() override
    {
    }

    HPDrugActive* Clone() const override;

    virtual void Clone(HPDrugActive* clone) const
    {
    }

protected:
    void _Activate() override;
};

class MPDrugActive : public DrugActive
{
public:
    MPDrugActive()
    {
    }

    ~MPDrugActive() override
    {
    }

    MPDrugActive* Clone() const override;

    virtual void Clone(MPDrugActive* clone) const
    {
    }

protected:
    void _Activate() override;
};

class DrugPerish : public DrugBehavior
{
public:
    DrugPerish()
    {
    }

    ~DrugPerish() override
    {
    }

    const char* Name() const override
    {
        return "Perish";
    }

    DrugPerish* Clone() const override;

    virtual void Clone(DrugPerish* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }

    void OnEnter() override;
};

#endif
