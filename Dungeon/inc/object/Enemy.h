/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Enemy.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 18, 2022                             *
 *                                                                            *
 *                    Last Update : August 25, 2022                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Enemies of the game.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <string>

#include "Figure.h"


/********************************************************************
** Enemy is slightly different from hero, for it should behave on it
** self. Since enemies varies, in this version, enemies do not have 
** specific class, they are just categoried by level, which is used
** for generation. Of course, Boss still have name, and yes, specific
** class. Enemies with negative level is intended to be found only
** by name.
*/

/********************************************************************
** Base class of all enemies.
** Specific enemies only differ in behavior. For more flexibility,
** enemy's behavior will be generated on load, decided by xml node's
** attribute. However, Boss has some more advanced behaviors and
** skills.
*/
class Enemy : public Figure
{
public:
	Enemy(Scene* pScene) : Figure(ObjectType::OBJ_ENEMY, pScene),
		m_facing(DIR_LEFT) {}
	virtual ~Enemy() {}

	int Level() const { return m_level; }

	/*
	** Here, enemy is both a final and an intermediate
	** class, so it has both Clone functions.
	*/
	virtual Enemy* Clone() const;
	virtual void Clone(Enemy* clone) const;

	virtual bool Load(XMLElement* node);

public:
	int GetDamage() const { return m_damage; }
	int GetCoin() const { return m_coin; }
	int GetEnergy() const { return m_energy; }
	double GetAlertRadius() const { return m_alertRadius; }
	double GetAttackRadius() const { return m_attackRadius; }
	clock_t GetRestDuration() const { return m_restDuration; }

	void SetFacing(Direction facing);
	Direction GetFacing() const { return m_facing; }
	void ChangeFacing();

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);
	virtual void _InitState();

protected:
	int m_level;

	int m_damage;
	
	// For generation on defeat. These are max value.
	int m_coin;
	int m_energy;

	double m_alertRadius;
	double m_attackRadius;
	clock_t m_restDuration;

	Direction m_facing;
};

#endif