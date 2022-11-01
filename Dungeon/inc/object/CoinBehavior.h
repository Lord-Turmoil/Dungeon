/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : CoinBehavior.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Coin behavior.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _COIN_BEHAVIOR_H_
#define _COIN_BEHAVIOR_H_

#include <dungine.h>

class CoinBehavior : public Behavior
{
public:
	CoinBehavior() {}
	virtual ~CoinBehavior() {}

	virtual const char* Name() const = 0;

	virtual CoinBehavior* Clone() const = 0;
	virtual void Clone(CoinBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	enum CoinAnimTag
	{
		COIN_ANIM_FLY,
		COIN_ANIM_ACTIVE
	};
};

class CoinFly final : public CoinBehavior
{
public:
	CoinFly() {}
	virtual ~CoinFly() {}

	virtual const char* Name() const { return "Fly"; }

	virtual CoinFly* Clone() const;
	virtual void Clone(CoinFly* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class CoinActive final : public CoinBehavior
{
public:
	CoinActive() {}
	virtual ~CoinActive() {}

	virtual const char* Name() const { return "Active"; }

	virtual CoinActive* Clone() const;
	virtual void Clone(CoinActive* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class CoinPerish final  : public CoinBehavior
{
public:
	CoinPerish() {}
	virtual ~CoinPerish() {}

	virtual const char* Name() const { return "Perish"; }

	virtual CoinPerish* Clone() const;
	virtual void Clone(CoinPerish* clone) const {}

	virtual void Update(Event* evnt) {}

	virtual void OnEnter();
};

#endif