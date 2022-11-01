/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Genie.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 19, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Genie is enemy that holds weapon.                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _GENIE_H_
#define _GENIE_H_

#include "Enemy.h"

class Genie : public Enemy
{
public:
	Genie(Scene* pScene) : Enemy(pScene) {}
	virtual ~Genie() {}

	virtual Genie* Clone() const;
	virtual void Clone(Genie* clone) const;

	virtual bool Load(XMLElement* node);

public:
	double GetScareRadius() const { return m_scareRadius; }

public:
	virtual void _InitBehavior(XMLElement* node = nullptr);

	double m_scareRadius;
};

#endif