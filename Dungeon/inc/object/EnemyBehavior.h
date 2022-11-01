/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyBehavior.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 27, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Behaviors of enemies.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENEMY_BEHAVIOR_H_
#define _ENEMY_BEHAVIOR_H_

#include <dungine.h>

// Gap of decision.
const clock_t DECISION_GAP = 1000L;

const double DIST_MARGIN = 5.0;

class Enemy;
class Arena;
class Dungeon;

/********************************************************************
** Enemy behaviors:
**   Init    -- Play initialization animation.
**   Patrol  -- Quite far from the hero.	 (Harmless)
**   Idle    -- Sometimes may take a break.  (Harmless)
**   Attack  -- Spotted the hero and engage. (Aggressive)
**   Retreat -- Too close to the hero.       (Aggressive)
**   Dead    -- Play dead animation.
**   Perish  -- Become invalid.
**
** Some behaviors may change slightly with different difficulty
** level.
*/

class EnemyBehavior : public Behavior
{
public:
	EnemyBehavior() : m_elapsedTime(0L) {}
	virtual ~EnemyBehavior() {}

	/*
	** Get the name of the behavior. I think... C-Style string is
	** enough.
	*/
	virtual const char* Name() const = 0;

	virtual EnemyBehavior* Clone() const = 0;
	virtual void Clone(EnemyBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:

	bool _Alert();
	void _Move(const Vector& dir);
	void _Evade(int level);

	virtual void _Collide();
	virtual void _OnDefeat(Dungeon* dungeon, Enemy* enemy);


	clock_t m_elapsedTime;

protected:
	enum EnemyAnimTag
	{
		ENEMY_ANIM_INIT = 0,
		ENEMY_ANIM_IDLE,
		ENEMY_ANIM_MOVE,
		ENEMY_ANIM_ATTACK,
		ENEMY_ANIM_DEAD
	};
};

class EnemyInit : public EnemyBehavior
{
public:
	virtual const char* Name() const { return "Init"; }

	virtual EnemyInit* Clone() const;
	virtual void Clone(EnemyInit* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();
};

class EnemyIdle : public EnemyBehavior
{
public:
	virtual const char* Name() const { return "Idle"; }

	virtual EnemyIdle* Clone() const;
	virtual void Clone(EnemyIdle* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class EnemyEngage : public EnemyBehavior
{
public:
	virtual const char* Name() const { return "Engage"; }

	virtual EnemyEngage* Clone() const;
	virtual void Clone(EnemyEngage* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class EnemyAttack : public EnemyBehavior
{
public:
	virtual const char* Name() const { return "Attack"; }

	virtual EnemyAttack* Clone() const;
	virtual void Clone(EnemyAttack* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class EnemyRest : public EnemyBehavior
{
public:
	virtual const char* Name() const { return "Rest"; }

	virtual EnemyRest* Clone() const;
	virtual void Clone(EnemyRest* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();

protected:
	clock_t m_duration;
};

class EnemyDead : public EnemyBehavior
{
public:
	virtual const char* Name() const { return "Dead"; }

	virtual EnemyDead* Clone() const;
	virtual void Clone(EnemyDead* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class EnemyPerish : public EnemyBehavior
{
public:
	virtual const char* Name() const { return "Perish"; }

	virtual EnemyPerish* Clone() const;
	virtual void Clone(EnemyPerish* clone) const {}

	virtual void Update(Event* evnt) {}

	virtual void OnEnter();
};

#endif