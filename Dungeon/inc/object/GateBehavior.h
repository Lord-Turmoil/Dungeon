/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GateComponent.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 26, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For gate component.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _GATE_BEHAVIOR_H_
#define _GATE_BEHAVIOR_H_

#include <dungine.h>


/********************************************************************
** Behaviors of gate.
*/
class GateBehavior : public Behavior
{
public:
	GateBehavior() {}
	virtual ~GateBehavior() {}

	virtual const char* Name() const = 0;

	virtual GateBehavior* Clone() const = 0;
	virtual void Clone(GateBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	enum GateAnimTag
	{
		GATE_ANIM_CLOSED = 0,
		GATE_ANIM_OPENING,
		GATE_ANIM_OPEN,
		GATE_ANIM_CLOSING
	};
};

class GateOpen : public GateBehavior
{
public:
	GateOpen() {}
	virtual ~GateOpen() {}

	virtual const char* Name() const { return "Open"; }
	virtual GateOpen* Clone() const;
	virtual void Clone(GateOpen* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class GateClosed : public GateBehavior
{
public:
	GateClosed() {}
	virtual ~GateClosed() {}

	virtual const char* Name() const { return "Closed"; }
	virtual GateClosed* Clone() const;
	virtual void Clone(GateClosed* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class GateOpening : public GateBehavior
{
public:
	GateOpening() {}
	virtual ~GateOpening() {}

	virtual const char* Name() const { return "Opening"; }
	virtual GateOpening* Clone() const;
	virtual void Clone(GateOpening* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class GateClosing : public GateBehavior
{
public:
	GateClosing() {}
	virtual ~GateClosing() {}

	virtual const char* Name() const { return "Closing"; }
	virtual GateClosing* Clone() const;
	virtual void Clone(GateClosing* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

#endif