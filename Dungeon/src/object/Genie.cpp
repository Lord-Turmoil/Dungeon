/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Genie.cpp                                 *
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

#include "../../inc/object/Genie.h"
#include "../../inc/object/GenieBehavior.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Genie
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Genie* Genie::Clone() const
{
	Genie* clone = new Genie(nullptr);
	clone->_MakePrototype(false);

	Enemy::Clone(clone);

	clone->m_scareRadius = m_scareRadius;

	return clone;
}

void Genie::Clone(Genie* clone) const
{
	Enemy::Clone(clone);

	clone->m_scareRadius = m_scareRadius;
}

bool Genie::Load(XMLElement* node)
{
	// const char* name = node->Name();
	const char* attr;

	Enemy::Load(node);

	_PARSE("scare-radius", m_scareRadius, name, 0);

	_RETURN_STATE();
}

void Genie::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new GenieInit());
	parent->AddBehavior(new GenieIdle());
	parent->AddBehavior(new GeniePatrol());
	parent->AddBehavior(new GenieEngage());
	parent->AddBehavior(new GenieRetreat());
	parent->AddBehavior(new GenieAttack());
	parent->AddBehavior(new GenieRest());
	parent->AddBehavior(new GenieDead());
	parent->AddBehavior(new EnemyPerish());

	parent->ChangeBehavior("Init");
}