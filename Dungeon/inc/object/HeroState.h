/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroState.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 26, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For hero's state.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _HERO_STATE_H_
#define _HERO_STATE_H_

#include <dungine.h>

/********************************************************************
** Hero's state:
**   None
**   Hurt
**   Skill
*/
class HeroState : public State
{
public:
	HeroState() {}
	virtual ~HeroState() {}

	virtual const char* Name() const = 0;

	virtual HeroState* Clone() const = 0;
	virtual void Clone(HeroState* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:

};

class HeroNone : public HeroState
{
	virtual const char* Name() const { return "None"; }

	virtual HeroNone* Clone() const;
	virtual void Clone(HeroNone* clone) const {}

	virtual void Update(Event* evnt) {}
};

class HeroHurt : public HeroState
{
public:
	virtual const char* Name() const { return "Hurt"; }

	virtual HeroHurt* Clone() const;
	virtual void Clone(HeroHurt* clone) const {}

	virtual void Update(Event* evnt);
	
	virtual void OnEnter();
	virtual void OnExit();
};

class YonySkill : public HeroState
{
public:
	virtual const char* Name() const { return "Skill"; }

	virtual YonySkill* Clone() const;
	virtual void Clone(YonySkill* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class SpecterSkill : public HeroState
{
public:
	SpecterSkill() : m_minAlpha(100), m_maxAlpha(200) {}
	virtual const char* Name() const { return "Skill"; }

	virtual SpecterSkill* Clone() const;
	virtual void Clone(SpecterSkill* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();

private:
	static const int m_DELTA_ALPHA;

	int m_curAlpha;
	int m_minAlpha;
	int m_maxAlpha;
	int m_delta;
};

class BlackHandSkill : public HeroState
{
public:
	virtual const char* Name() const { return "Skill"; }

	virtual BlackHandSkill* Clone() const;
	virtual void Clone(BlackHandSkill* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();

private:
	virtual void _ThrowFlame();

	static const int m_FLAME_NUM;
	static const int m_FLAME_RADIUS;
	static const double m_DELTA;
};

#endif