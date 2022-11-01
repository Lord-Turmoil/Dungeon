/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : PortalBehavior.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 4, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Behavior of Portal.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _PORTAL_BEHAVIOR_H_
#define _PORTAL_BEHAVIOR_H_

#include <dungine.h>

class PortalBehavior : public Behavior
{
public:
	PortalBehavior() {}
	virtual ~PortalBehavior() {}

	virtual const char* Name() const = 0;

	virtual PortalBehavior* Clone() const = 0;
	virtual void Clone(PortalBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	enum PortalAnimTag
	{
		PORTAL_ANIM_INIT,
		PORTAL_ANIM_READY
	};
};

class PortalIdle final : public PortalBehavior
{
public:
	virtual const char* Name() const { return "Idle"; }

	virtual PortalIdle* Clone() const;
	virtual void Clone(PortalIdle* clone) const {}

	virtual void Update(Event* evnt) {}

	virtual void OnEnter();
	virtual void OnExit();
};

class PortalInit final : public PortalBehavior
{
public:
	virtual const char* Name() const { return "Init"; }

	virtual PortalInit* Clone() const;
	virtual void Clone(PortalInit* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();

private:
};

class PortalReady final : public PortalBehavior
{
public:
	virtual const char* Name() const { return "Ready"; }

	virtual PortalReady* Clone() const;
	virtual void Clone(PortalReady* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

#endif