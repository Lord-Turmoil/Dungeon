/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyState.h                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 28, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   State of enemy.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENEMY_STATE_H_
#define _ENEMY_STATE_H_

#include <dungine.h>


/********************************************************************
** Enemy's state:
**   None
**   Hurt
**   Skill
*/
class EnemyState : public State
{
public:
	EnemyState() {}
	virtual ~EnemyState() {}

	virtual const char* Name() const = 0;

	virtual EnemyState* Clone() const = 0;
	virtual void Clone(EnemyState* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	
};

class EnemyNone : public EnemyState
{
	virtual const char* Name() const { return "None"; }

	virtual EnemyNone* Clone() const;
	virtual void Clone(EnemyNone* clone) const {}

	virtual void Update(Event* evnt) {}
};

class EnemyHurt : public EnemyState
{
public:
	virtual const char* Name() const { return "Hurt"; }

	virtual EnemyHurt* Clone() const;
	virtual void Clone(EnemyHurt* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();
};


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Skill
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class DBKSkill : public EnemyState
{
public:
	virtual const char* Name() const { return "Skill"; }

	virtual DBKSkill* Clone() const;
	virtual void Clone(DBKSkill* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();

private:
	static const int m_FLAME_NUM;
	static const int m_FLAME_RADIUS;
	static const double m_DELTA;

	void _ThrowFlame();
};

#endif