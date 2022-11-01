/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : WeaponBehavior.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : Julyt 19, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For common weapon behaviors.                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _WEAPON_BEHAVIOR_H_
#define _WEAPON_BEHAVIOR_H_

#include <dungine.h>

class WeaponBehavior : public Behavior
{
public:
	WeaponBehavior() {}
	virtual ~WeaponBehavior() {}

	virtual const char* Name() const = 0;

	virtual WeaponBehavior* Clone() const = 0;
	virtual void Clone(WeaponBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	virtual void _Fire();

protected:
	enum WeaponAnimTag
	{
		WEAPON_ANIM_IDLE = 0,
		WEAPON_ANIM_READY,
		WEAPON_ANIM_FIRE,
		WEAPON_ANIM_COOLING,
		WEAPON_ANIM_CUSTOM_START
	};
};

class WeaponIdle : public WeaponBehavior
{
public:
	virtual const char* Name() const { return "Idle"; }

	virtual WeaponIdle* Clone() const;
	virtual void Clone(WeaponIdle* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class WeaponUnequipped : public WeaponBehavior
{
public:
	virtual const char* Name() const { return "Unequipped"; }

	virtual WeaponUnequipped* Clone() const;
	virtual void Clone(WeaponUnequipped* clone) const {}

	virtual void Update(Event* evnt) {}

	virtual void OnEnter();
};


// This is uncertain.
class WeaponReady : public WeaponBehavior
{
public:
	virtual const char* Name() const { return "Ready"; }

	virtual WeaponReady* Clone() const = 0;
	virtual void Clone(WeaponReady* clone) const;

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter();
};

class WeaponFire : public WeaponBehavior
{
public:
	virtual const char* Name() const { return "Fire"; }

	virtual WeaponFire* Clone() const;
	virtual void Clone(WeaponFire* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class WeaponCooling : public WeaponBehavior
{
public:
	virtual const char* Name() const { return "Cooling"; }

	virtual WeaponCooling* Clone() const;
	virtual void Clone(WeaponCooling* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();

private:
	// No need to clone.
	clock_t m_coolingTime;
	clock_t m_elapsedTime;
};

#endif