/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Bullet.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Bullet in the game.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Bullet.h"
#include "../../inc/object/BulletBehavior.h"


/******************************************************************************
 * Bullet::Clone -- Clone Bullet                                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/19 Tony : Created.                                               *
 *============================================================================*/
Bullet* Bullet::Clone() const
{
	Bullet* clone = new Bullet(m_pScene);
	clone->_MakePrototype(false);

	Object::Clone(clone);

	clone->m_name = m_name;
	clone->m_dir = m_dir;

	clone->m_damage = m_damage;
	clone->m_range = m_range;
	clone->m_force = m_force;
	clone->m_AOERadius = m_AOERadius;
	clone->m_isGood = m_isGood;
	clone->m_isDirectional = m_isDirectional;
	clone->m_isPenetrable = m_isPenetrable;

	return clone;
}


/******************************************************************************
 * Bullet::Clone -- Clone bullet as super class.                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/19 Tony : Created.                                               *
 *============================================================================*/
void Bullet::Clone(Bullet* clone) const
{
	Object::Clone(clone);

	clone->m_name = m_name;
	clone->m_dir = m_dir;

	clone->m_damage = m_damage;
	clone->m_range = m_range;
	clone->m_force = m_force;
	clone->m_AOERadius = m_AOERadius;
	clone->m_isGood = m_isGood;
	clone->m_isDirectional = m_isDirectional;
	clone->m_isPenetrable = m_isPenetrable;
}


/******************************************************************************
 * Bullet::Load -- Load bullet.                                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/19 Tony : Created.                                               *
 *============================================================================*/
bool Bullet::Load(XMLElement* node)
{
/*
**	<Bullet name="" damage="" range="" force="" aoe="">
**		<Components>
**			...
**		</Components>
**	</Bullet>
*/
	const char* name = node->Name();
	const char* attr;

	Object::Load(node);

	_PARSE_ESSENTIAL("name", m_name, name, "");
	_PARSE("damage", m_damage, name, 0);
	_PARSE("range", m_range, name, MAX_RANGE);
	_PARSE("force", m_force, name, 0.0);
	_PARSE("aoe", m_AOERadius, name, 0.0);
	_PARSE("directional", m_isDirectional, name, false);
	_PARSE("penetrable", m_isPenetrable, name, false);

	_InitBehavior();

	_RETURN_STATE();
}


/******************************************************************************
 * Bullet::Activate -- Activate bullet.                                       *
 *                                                                            *
 *    Since bullet must initialize coordinate and direction before enter fly  *
 *    behavior, so this intermeidate method is used.                          *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/26 Tony : Created.                                               *
 *============================================================================*/
void Bullet::Activate()
{
	GetComponent<BehaviorComponent>()->ChangeBehavior("Fly");
}


/******************************************************************************
 * Bullet::Corrupt -- Change to corrupt behavior.                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/28 Tony : Created.                                               *
 *============================================================================*/
void Bullet::Corrupt()
{
	if (m_isGood)
		GetComponent<BehaviorComponent>()->ChangeBehavior("Corrupt");
}


/******************************************************************************
 * Bullet::_InitBehavior -- Initialize behavior of bullet.                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/24 Tony : Created.                                               *
 *============================================================================*/
void Bullet::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new BulletNone());
	parent->AddBehavior(new BulletFly());
	parent->AddBehavior(new BulletExplode());
	parent->AddBehavior(new BulletCorrupt());
	parent->AddBehavior(new BulletPerish());

	parent->ChangeBehavior("None");
}