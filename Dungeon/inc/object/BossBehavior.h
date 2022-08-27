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
 * Over View:                                                                 *
 *   Behavior of boss.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
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
	virtual BossInit* Clone() const;
	virtual void Clone(BossInit* clone) const;

	virtual void OnExit();
};


class BossDead : public EnemyDead
{
public:
	virtual BossDead* Clone() const;
	virtual void Clone(BossDead* clone) const {}

	virtual void OnEnter();
	virtual void OnExit();
};

class BossPerish : public EnemyPerish
{
public:
	virtual BossPerish* Clone() const;
	virtual void Clone(BossPerish* clone) const {}

	virtual void OnEnter() {}
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Rock
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class RockIdle : public EnemyIdle
{
public:
	virtual RockIdle* Clone() const;
	virtual void Clone(RockIdle* clone) const {}

	virtual void Update(Event* evnt);
};

class RockEngage : public GenieEngage
{
public:
	virtual RockEngage* Clone() const;
	virtual void Clone(RockEngage* clone) const {}

	virtual void Update(Event* evnt);
};

class RockRetreat : public GenieRetreat
{
public:
	virtual RockRetreat* Clone() const;
	virtual void Clone(RockRetreat* clone) const {}

	virtual void Update(Event* evnt);
};

class RockAttack : public EnemyAttack
{
public:
	virtual RockAttack* Clone() const;
	virtual void Clone(RockAttack* clone) const {}

	virtual void OnEnter();

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
	virtual DBKInit* Clone() const;
	virtual void Clone(DBKInit* clone) const {}

	virtual void OnExit();
};

class DBKIdle : public GenieIdle
{
public:
	virtual DBKIdle* Clone() const;
	virtual void Clone(DBKIdle* clone) const {}

	virtual void Update(Event* evnt);
};

class DBKEngage : public GenieEngage
{
public:
	virtual DBKEngage* Clone() const;
	virtual void Clone(DBKEngage* clone) const {}

	virtual void Update(Event* evnt);
};

class DBKRetreat : public GenieRetreat
{
public:
	virtual DBKRetreat* Clone() const;
	virtual void Clone(DBKRetreat* clone) const {}

	virtual void Update(Event* evnt);
};

class DBKDead : public BossDead
{
public:
	virtual DBKDead* Clone() const;
	virtual void Clone(DBKDead* clone) const {}

	virtual void OnEnter();
};

#endif