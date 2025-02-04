/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BossBehavior.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 30, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Behavior of boss.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _BOSS_BEHAVIOR_H_
#define _BOSS_BEHAVIOR_H_

#include "GenieBehavior.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Boss
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class BossInit : public EnemyInit
{
public:
    BossInit* Clone() const override;
    virtual void Clone(BossInit* clone) const;

    void OnExit() override;
};

class BossDead : public EnemyDead
{
public:
    BossDead* Clone() const override;

    virtual void Clone(BossDead* clone) const
    {
    }

    void OnEnter() override;
    void OnExit() override;
};

class BossPerish : public EnemyPerish
{
public:
    BossPerish* Clone() const override;

    virtual void Clone(BossPerish* clone) const
    {
    }

    void OnEnter() override
    {
    }
};

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Rock
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class RockIdle : public EnemyIdle
{
public:
    RockIdle* Clone() const override;

    virtual void Clone(RockIdle* clone) const
    {
    }

    void Update(Event* evnt) override;
};

class RockEngage : public GenieEngage
{
public:
    RockEngage* Clone() const override;

    virtual void Clone(RockEngage* clone) const
    {
    }

    void Update(Event* evnt) override;
};

class RockRetreat : public GenieRetreat
{
public:
    RockRetreat* Clone() const override;

    virtual void Clone(RockRetreat* clone) const
    {
    }

    void Update(Event* evnt) override;
};

class RockAttack : public EnemyAttack
{
public:
    RockAttack* Clone() const override;

    virtual void Clone(RockAttack* clone) const
    {
    }

    void OnEnter() override;

private:
    void _GenerateStone();
};

// class RockRest = EnemeyRest

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Dark Black Hand
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class DBKInit : public GenieInit
{
public:
    DBKInit* Clone() const override;

    virtual void Clone(DBKInit* clone) const
    {
    }

    void OnExit() override;
};

class DBKIdle : public GenieIdle
{
public:
    DBKIdle* Clone() const override;

    virtual void Clone(DBKIdle* clone) const
    {
    }

    void Update(Event* evnt) override;
};

class DBKEngage : public GenieEngage
{
public:
    DBKEngage* Clone() const override;

    virtual void Clone(DBKEngage* clone) const
    {
    }

    void Update(Event* evnt) override;
};

class DBKRetreat : public GenieRetreat
{
public:
    DBKRetreat* Clone() const override;

    virtual void Clone(DBKRetreat* clone) const
    {
    }

    void Update(Event* evnt) override;
};

class DBKDead : public BossDead
{
public:
    DBKDead* Clone() const override;

    virtual void Clone(DBKDead* clone) const
    {
    }

    void OnEnter() override;
};

#endif
