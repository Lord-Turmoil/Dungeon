/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BowBehavior.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Bow behavior.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _BOW_BEHAVIOR_H_
#define _BOW_BEHAVIOR_H_

#include "WeaponBehavior.h"

/********************************************************************
** Bow behaviors:
**  Ready
**  Charge
**  Discharge
**  Hold
*/
class BowBehavior : public WeaponBehavior
{
public:
	BowBehavior() {}
	virtual ~BowBehavior() {}

	virtual const char* Name() const = 0;

	virtual BowBehavior* Clone() const = 0;
	virtual void Clone(BowBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	enum BowAnimTag
	{
		BOW_ANIM_CHARGE = WEAPON_ANIM_CUSTOM_START,
		BOW_ANIM_DISCHARGE,
		BOW_ANIM_HOLD,		// fully charged
	};
};

class BowReady : public BowBehavior
{
public:
	virtual const char* Name() const { return "Ready"; }

	virtual BowReady* Clone() const;
	virtual void Clone(BowReady* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class BowCharge : public BowBehavior
{
public:
	virtual const char* Name() const { return "Charge"; }

	virtual BowCharge* Clone() const;
	virtual void Clone(BowCharge* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class BowDischarge : public BowBehavior
{
public:
	virtual const char* Name() const { return "Discharge"; }

	virtual BowDischarge* Clone() const;
	virtual void Clone(BowDischarge* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class BowHold : public BowBehavior
{
public:
	virtual const char* Name() const { return "Hold"; }

	virtual BowHold* Clone() const;
	virtual void Clone(BowHold* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();
};

#endif