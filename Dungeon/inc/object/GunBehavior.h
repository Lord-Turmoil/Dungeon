/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GunBehavior.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Gun behavior.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _GUN_BEHAVIOR_H_
#define _GUN_BEHAVIOR_H_

#include "WeaponBehavior.h"

/********************************************************************
** Gun behavior:
**   Ready
*/
class GunBehavior : public WeaponBehavior
{
public:
	GunBehavior() {}
	virtual ~GunBehavior() {}

	virtual const char* Name() const = 0;

	virtual GunBehavior* Clone() const = 0;
	virtual void Clone(GunBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:

};

class GunReady : public GunBehavior
{
public:
	virtual const char* Name() const { return "Ready"; }

	virtual GunReady* Clone() const;
	virtual void Clone(WeaponReady* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

#endif