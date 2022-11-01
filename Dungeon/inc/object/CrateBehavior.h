/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : CrateBehavior.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 15, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Crate behavior.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _CRATE_BEHAVIOR_H_
#define _CRATE_BEHAVIOR_H_

#include <dungine.h>

class CrateBehavior : public Behavior
{
public:
	CrateBehavior() {}
	virtual ~CrateBehavior() {}

	virtual const char* Name() const = 0;

	virtual CrateBehavior* Clone() const = 0;
	virtual void Clone(CrateBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	enum CrateAnimTag
	{
		CRATE_ANIM_INIT,
		CRATE_ANIM_IDLE,
		CRATE_ANIM_OPENING,
		CRATE_ANIM_OPEN
	};
};

class CrateInit final : public CrateBehavior
{
public:
	virtual const char* Name() const { return "Init"; }

	virtual CrateInit* Clone() const;
	virtual void Clone(CrateInit* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class CrateIdle final : public CrateBehavior
{
public:
	virtual const char* Name() const { return "Idle"; }
	
	virtual CrateIdle* Clone() const;
	virtual void Clone(CrateIdle* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class CrateOpening final : public CrateBehavior
{
public:
	virtual const char* Name() const { return "Opening"; }

	virtual CrateOpening* Clone() const;
	virtual void Clone(CrateOpening* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class CrateOpen final : public CrateBehavior
{
public:
	virtual const char* Name() const { return "Open"; }

	virtual CrateOpen* Clone() const;
	virtual void Clone(CrateOpen* clone) const {}

	virtual void Update(Event* evnt) {}

	virtual void OnEnter();
};


#endif
