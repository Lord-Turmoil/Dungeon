/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GenieBehavior.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 19, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Genie's behavior.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _GENIE_BEHAVIOR_H_
#define _GENIE_BEHAVIOR_H_

#include "EnemyBehavior.h"

/********************************************************************
** Genie's behaviors.
** Init    -- Need take care of weapon.
** Idle    -- To Patrol or Engage.
** Patrol  -- To Idle or Engage.
** Engage  -- To Attack or Retreat.
** Retreat -- To Patrol.
** Attack  -- To Rest.
** Rest    -- Need take care of weapon.
** Dead    -- Need take care of weapon.
** Perish  -- The same as enemy.
*/
class GenieInit : public EnemyInit
{
public:
	virtual GenieInit* Clone() const;
	virtual void Clone(GenieInit* clone) const;

	virtual void OnEnter();
	virtual void OnExit();
};

class GenieIdle : public EnemyIdle
{
public:
	virtual GenieIdle* Clone() const;
	virtual void Clone(GenieIdle* clone) const;

	virtual void Update(Event* evnt);
};

class GeniePatrol : public EnemyBehavior
{
public:
	virtual const char* Name() const { return "Patrol"; }

	virtual GeniePatrol* Clone() const;
	virtual void Clone(GeniePatrol* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();

private:
	Coordinate m_target;
};

class GenieEngage : public EnemyEngage
{
public:
	virtual GenieEngage* Clone() const;
	virtual void Clone(GenieEngage* clone) const;

	virtual void Update(Event* evnt);
};

class GenieRetreat : public EnemyBehavior
{
public:
	virtual const char* Name() const { return "Retreat"; }

	virtual GenieRetreat* Clone() const;
	virtual void Clone(GenieRetreat* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class GenieAttack : public EnemyAttack
{
public:
	virtual GenieAttack* Clone() const;
	virtual void Clone(GenieAttack* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();
};

class GenieRest : public EnemyRest
{
public:
	virtual GenieRest* Clone() const;
	virtual void Clone(GenieRest* clone) const;

	virtual void Update(Event* evnt);
};

class GenieDead : public EnemyDead
{
public:
	virtual GenieDead* Clone() const;
	virtual void Clone(GenieDead* clone) const;

	virtual void OnEnter();
};

#endif