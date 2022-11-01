/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : DrugBehavior.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Drug behavior.                                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _DRUG_BEHAVIOR_H_
#define _DRUG_BEHAVIOR_H_

#include <dungine.h>

class DrugBehavior : public Behavior
{
public:
	DrugBehavior() {}
	virtual ~DrugBehavior() {}

	virtual const char* Name() const = 0;

	virtual DrugBehavior* Clone() const = 0;
	virtual void Clone(DrugBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	enum DrugAnimTag
	{
		DRUG_ANIM_IDLE,
		DRUG_ANIM_ACTIVE
	};
};

class DrugIdle : public DrugBehavior
{
public:
	DrugIdle() {}
	virtual ~DrugIdle() {}

	virtual const char* Name() const { return "Idle"; }

	virtual DrugIdle* Clone() const;
	virtual void Clone(DrugIdle* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class DrugActive : public DrugBehavior
{
public:
	DrugActive() {}
	virtual ~DrugActive() {}

	virtual const char* Name() const { return "Active"; }

	virtual DrugActive* Clone() const = 0;
	virtual void Clone(DrugActive* clone) const;

	virtual void Update(Event* evnt);

	virtual void OnEnter();

protected:
	virtual void _Activate() = 0;
};

class HPDrugActive : public DrugActive
{
public:
	HPDrugActive() {}
	virtual ~HPDrugActive() {}

	virtual HPDrugActive* Clone() const;
	virtual void Clone(HPDrugActive* clone) const {}

protected:
	virtual void _Activate();
};

class MPDrugActive : public DrugActive
{
public:
	MPDrugActive() {}
	virtual ~MPDrugActive() {}

	virtual MPDrugActive* Clone() const;
	virtual void Clone(MPDrugActive* clone) const {}

protected:
	virtual void _Activate();
};

class DrugPerish : public DrugBehavior
{
public:
	DrugPerish() {}
	virtual ~DrugPerish() {}

	virtual const char* Name() const { return "Perish"; }

	virtual DrugPerish* Clone() const;
	virtual void Clone(DrugPerish* clone) const {}

	virtual void Update(Event* evnt) {}

	virtual void OnEnter();
};

#endif