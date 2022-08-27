/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BossBehavior.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 30, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Behavior of boss.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/object/Boss.h"
#include "../../inc/object/BossBehavior.h"
#include "../../inc/object/Component.h"

#include "../../inc/game/Dungeon.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BossInit
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BossInit* BossInit::Clone() const
{
	BossInit* clone = new BossInit();
	clone->_MakePrototype(false);

	EnemyInit::Clone(clone);

	return clone;
}

void BossInit::Clone(BossInit* clone) const
{
	EnemyInit::Clone(clone);
}

void BossInit::OnExit()
{
	Boss* boss = static_cast<Boss*>(m_parent->GetGameObject());

	boss->GetComponent<RigidBodyComponent>()
		->SetID(CollisionID::COLL_ID_ENEMY);
	boss->GetComponent<SoundComponent>()->Play("init");
	boss->CostMP(INF_INT);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BossDead
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BossDead* BossDead::Clone() const
{
	BossDead* clone = new BossDead();
	clone->_MakePrototype(false);

	EnemyDead::Clone(clone);

	// Really? XD
	// __super::Clone(clone);

	return clone;
}

void BossDead::OnEnter()
{
	Boss* boss = static_cast<Boss*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(boss->GetScene());
	boss->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(ENEMY_ANIM_DEAD);

	dungeon->RemoveBoss();
	dungeon->KillAllEnemy();

	_OnDefeat(dungeon, boss);
}

void BossDead::OnExit()
{
	Boss* boss = static_cast<Boss*>(m_parent->GetGameObject());
	boss->GetComponent<RigidBodyComponent>()->Freeze();
	boss->GetComponent<RigidBodyComponent>()->SetMass(0.0);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BossPerish
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BossPerish* BossPerish::Clone() const
{
	BossPerish* clone = new BossPerish();
	clone->_MakePrototype(false);

	EnemyPerish::Clone(clone);

	return clone;
}