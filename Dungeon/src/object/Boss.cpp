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

#include "../../inc/object/Boss.h"
#include "../../inc/object/BossBehavior.h"
#include "../../inc/object/EnemyState.h"

#include "../../inc/game/Settings.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Boss
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void Boss::Clone(Boss* clone) const
{
	Genie::Clone(clone);
	clone->m_isUsingSkill = m_isUsingSkill;
}

bool Boss::Load(XMLElement* node)
{
	Genie::Load(node);

	int difficulty = Settings::GetInstance()->Difficulty() + 1;

	m_hp[0] = m_hp[1] = m_hp[1] * difficulty / DIFFICULTY_NUM;

	_RETURN_STATE();
}

// Enhance boss by increase health by rate.
void Boss::Enhance(float rate)
{
	m_hp[1] = (int)((float)m_hp[1] * (1.0f + rate));
	Revitalize();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Rock
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
ERock* ERock::Clone() const
{
	ERock* clone = new ERock(m_pScene);
	clone->_MakePrototype(false);

	Boss::Clone(clone);

	return clone;
}

bool ERock::Load(XMLElement* node)
{
	const char* name = node->Name();

	_CHECK_TAG("Rock");

	Boss::Load(node);

	_RETURN_STATE();
}

void ERock::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new BossInit());
	parent->AddBehavior(new RockIdle());
	parent->AddBehavior(new RockEngage());
	parent->AddBehavior(new RockRetreat());
	parent->AddBehavior(new RockAttack());
	parent->AddBehavior(new EnemyRest());
	parent->AddBehavior(new BossDead());
	parent->AddBehavior(new BossPerish());

	parent->ChangeBehavior("Init");
}

void ERock::_InitState()
{
	auto parent = GetComponent<StateComponent>();

	parent->AddState(new EnemyNone());
	parent->AddState(new EnemyHurt());

	parent->ChangeState("None");
}


/********************************************************************
** Well, this is Dark Black Hand, the same as the hero Black Hand.
*/
EBlackHand* EBlackHand::Clone() const
{
	EBlackHand* clone = new EBlackHand(m_pScene);
	clone->_MakePrototype(false);

	Boss::Clone(clone);

	return clone;
}

bool EBlackHand::Load(XMLElement* node)
{
	const char* name = node->Name();

	_CHECK_TAG("DBlackHand");

	Boss::Load(node);

	_RETURN_STATE();
}

void EBlackHand::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new DBKInit());
	parent->AddBehavior(new DBKIdle());
	parent->AddBehavior(new DBKEngage());
	parent->AddBehavior(new DBKRetreat());
	parent->AddBehavior(new DBKDead());
	parent->AddBehavior(new BossPerish());

	parent->ChangeBehavior("Init");
}

void EBlackHand::_InitState()
{
	auto parent = GetComponent<StateComponent>();

	parent->AddState(new EnemyNone());
	parent->AddState(new EnemyHurt());
	parent->AddState(new DBKSkill());

	parent->ChangeState("None");
}