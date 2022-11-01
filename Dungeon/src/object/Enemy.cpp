/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Enemy.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 28, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Enemies of the game.                                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Enemy.h"
#include "../../inc/object/EnemyBehavior.h"
#include "../../inc/object/BossBehavior.h"
#include "../../inc/object/EnemyState.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Enemy
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Enemy* Enemy::Clone() const
{
	Enemy* clone = new Enemy(m_pScene);
	clone->_MakePrototype(false);

	Figure::Clone(clone);

	clone->m_level = m_level;
	clone->m_alertRadius = m_alertRadius;
	clone->m_attackRadius = m_attackRadius;
	clone->m_restDuration = m_restDuration;
	clone->m_damage = m_damage;
	clone->m_coin = m_coin;
	clone->m_energy = m_energy;
	clone->m_facing = m_facing;

	return clone;
}

void Enemy::Clone(Enemy* clone) const
{
	Figure::Clone(clone);

	clone->m_level = m_level;
	clone->m_alertRadius = m_alertRadius;
	clone->m_attackRadius = m_attackRadius;
	clone->m_restDuration = m_restDuration;
	clone->m_damage = m_damage;
	clone->m_coin = m_coin;
	clone->m_energy = m_energy;
	clone->m_facing = m_facing;
}

bool Enemy::Load(XMLElement* node)
{
/*
**	<Enemy ... level="" patrol-radius="" scare-radius="" vitality="">
**	</Enemy>
*/
	const char* name = node->Name();
	const char* attr;

	Figure::Load(node);

	_PARSE_ESSENTIAL("level", m_level, name, 0);
	_PARSE("damage", m_damage, name, 0);
	_PARSE("coin", m_coin, name, 0);
	_PARSE("energy", m_energy, name, 0);
	_PARSE_ESSENTIAL("alert-radius", m_alertRadius, name, 0);
	_PARSE_ESSENTIAL("attack-radius", m_attackRadius, name, 0);
	_PARSE("rest", m_restDuration, name, 0);

	_RETURN_STATE();
}

void Enemy::SetFacing(Direction facing)
{
	m_facing = facing;
	if (facing == DIR_LEFT)
		GetComponent<AnimComponent>()->GetAnim()->SetDir(ANIM_LEFT);
	else
		GetComponent<AnimComponent>()->GetAnim()->SetDir(ANIM_RIGHT);
}

void Enemy::ChangeFacing()
{
	if (m_facing == DIR_LEFT)
		SetFacing(DIR_RIGHT);
	else
		SetFacing(DIR_LEFT);
}

void Enemy::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new EnemyInit());
	parent->AddBehavior(new EnemyIdle());
	parent->AddBehavior(new EnemyEngage());
	parent->AddBehavior(new EnemyAttack());
	parent->AddBehavior(new EnemyRest());
	parent->AddBehavior(new EnemyDead());
	parent->AddBehavior(new EnemyPerish());

	parent->ChangeBehavior("Init");
}

void Enemy::_InitState()
{
	StateComponent* parent = GetComponent<StateComponent>();

	parent->AddState(new EnemyNone());
	parent->AddState(new EnemyHurt());

	parent->ChangeState("None");
}
