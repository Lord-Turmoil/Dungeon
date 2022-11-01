/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnergyBehavior.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Energy behavior.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENERGY_BEHAVIOR_H_
#define _ENERGY_BEHAVIOR_H_

#include <dungine.h>

class EnergyBehavior : public Behavior
{
public:
	EnergyBehavior() {}
	virtual ~EnergyBehavior() {}

	virtual const char* Name() const = 0;

	virtual EnergyBehavior* Clone() const = 0;
	virtual void Clone(EnergyBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	enum EnergyAnimTag
	{
		ENERGY_ANIM_FLY,
		ENERGY_ANIM_ACTIVE
	};
};

class EnergyFly final : public EnergyBehavior
{
public:
	EnergyFly() {}
	virtual ~EnergyFly() {}

	virtual const char* Name() const { return "Fly"; }

	virtual EnergyFly* Clone() const;
	virtual void Clone(EnergyFly* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class EnergyActive final : public EnergyBehavior
{
public:
	EnergyActive() {}
	virtual ~EnergyActive() {}

	virtual const char* Name() const { return "Active"; }

	virtual EnergyActive* Clone() const;
	virtual void Clone(EnergyActive* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class EnergyPerish final : public EnergyBehavior
{
public:
	EnergyPerish() {}
	virtual ~EnergyPerish() {}

	virtual const char* Name() const { return "Perish"; }

	virtual EnergyPerish* Clone() const;
	virtual void Clone(EnergyPerish* clone) const {}

	virtual void Update(Event* evnt) {}

	virtual void OnEnter();
};

#endif