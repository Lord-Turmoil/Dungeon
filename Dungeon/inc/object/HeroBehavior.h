/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroBehavior.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 23, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Behavior of hero. Generally, they are the same.                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _HERO_BEHAVIOR_H_
#define _HERO_BEHAVIOR_H_

#include <dungine.h>


/********************************************************************
** Hero's behavior:
**   Init   -- Play initialization animation.
**   Move   -- Normal and under control.
**   Dead   -- Play dead animation.
**   Perish -- Become invalid.
*/
class HeroBehavior : public Behavior
{
public:
	HeroBehavior() {}
	virtual ~HeroBehavior() {}

	virtual const char* Name() const = 0;

	virtual HeroBehavior* Clone() const = 0;
	virtual void Clone(HeroBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	virtual void _Collide();

protected:
	// INIT and DEAD only has one direction.
	enum HeroAnimTag
	{
		HERO_ANIM_INIT = 0,
		HERO_ANIM_IDLE,
		HERO_ANIM_MOVE,
		HERO_ANIM_DEAD
	};
};


/********************************************************************
** First show up in the battlefield.
*/
class HeroInit : public HeroBehavior
{
public:
	virtual const char* Name() const { return "Init"; }

	virtual HeroInit* Clone() const;
	virtual void Clone(HeroInit* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();
};

class HeroMove : public HeroBehavior
{
public:
	virtual const char* Name() const { return "Move"; }

	virtual HeroMove* Clone() const;
	virtual void Clone(HeroMove* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();

protected:
	void _Move(Event* evnt);
	void _Act(Event* evnt);
};

class HeroDead : public HeroBehavior
{
public:
	virtual const char* Name() const { return "Dead"; }

	virtual HeroDead* Clone() const;
	virtual void Clone(HeroDead* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class HeroPerish : public HeroBehavior
{
public:
	virtual const char* Name() const { return "Perish"; }

	virtual HeroPerish* Clone() const;
	virtual void Clone(HeroPerish* clone) const {}

	virtual void Update(Event* evnt) {}

	virtual void OnEnter();
};

#endif
