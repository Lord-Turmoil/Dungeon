/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Boss.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 25, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For boss.                                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _BOSS_H_
#define _BOSS_H_

#include "Genie.h"


/********************************************************************
** Boss has its specific behavior and state. And it won't be removed
** after defeated.
*/
class Boss : public Genie
{
public:
	Boss(Scene* pScene) : Genie(pScene), m_isUsingSkill(false) {}
	virtual ~Boss() {}

	virtual Boss* Clone() const = 0;
	virtual void Clone(Boss* clone) const;

	virtual bool Load(XMLElement* node);

public:
	void SetUsingSkill(bool isUsingSkill) { m_isUsingSkill = isUsingSkill; }
	bool IsUsingSkill() const { return m_isUsingSkill; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr) {}
	virtual void _InitState() {}

	bool m_isUsingSkill;
};


/********************************************************************
** Rock is a giant stone, it can summon small stones. :)
*/
class ERock final : public Boss
{
public:
	ERock(Scene* pScene) : Boss(pScene) {}
	virtual ~ERock() {}

	virtual ERock* Clone() const;
	virtual void Clone(ERock* clone) const {}

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);
	virtual void _InitState();
};


/********************************************************************
** Well, this is Dark Black Hand, the same as the hero Black Hand.
** abbr. DBK
*/
class EBlackHand final : public Boss
{
public:
	EBlackHand(Scene* pScene) : Boss(pScene) {}
	virtual ~EBlackHand() {}

	virtual EBlackHand* Clone() const;
	virtual void Clone(EBlackHand* clone) const {}

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);
	virtual void _InitState();
};

#endif