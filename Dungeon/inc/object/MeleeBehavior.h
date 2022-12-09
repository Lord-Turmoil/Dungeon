/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : MeleeBehavior.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Melee behavior.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _MELEE_BEHAVIOR_H_
#define _MELEE_BEHAVIOR_H_

#include "WeaponBehavior.h"

/********************************************************************
** Melee behavior:
**   Ready
**   Fire
*/
class MeleeBehavior : public WeaponBehavior
{
public:
	MeleeBehavior() {}
	virtual ~MeleeBehavior() {}

	virtual const char* Name() const = 0;

	virtual MeleeBehavior* Clone() const = 0;
	virtual void Clone(MeleeBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	virtual void _Slash();

	/*
	** 2022/11/25 TS:
	** If melee can slash bullet during the whole fire progress?
	*/
	virtual void _SlashBullet();
};

class MeleeReady : public MeleeBehavior
{
public:
	virtual const char* Name() const { return "Ready"; }

	virtual MeleeReady* Clone() const;
	virtual void Clone(MeleeReady* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class MeleeFire : public MeleeBehavior
{
public:
	virtual const char* Name() const { return "Fire"; }

	virtual MeleeFire* Clone() const;
	virtual void Clone(MeleeFire* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

#endif