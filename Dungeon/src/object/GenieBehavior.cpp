/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GenieBehavior.cpp                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 19, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Genie's behavior.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/GenieBehavior.h"
#include "../../inc/object/Genie.h"
#include "../../inc/object/Hero.h"
#include "../../inc/object/Weapon.h"
#include "../../inc/object/Component.h"

#include "../../inc/terrain/Terrain.h"

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Settings.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GenieInit
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
GenieInit* GenieInit::Clone() const
{
	GenieInit* clone = new GenieInit();
	clone->_MakePrototype(false);

	EnemyInit::Clone(clone);

	return clone;
}

void GenieInit::Clone(GenieInit* clone) const
{
	EnemyInit::Clone(clone);
}

void GenieInit::OnEnter()
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());

	Animation* anim = genie->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(ENEMY_ANIM_INIT);
	anim->SetDir((AnimDirection)Random(2));

	genie->GetComponent<RigidBodyComponent>()
		->SetID(CollisionID::COLL_ID_SPECTER);
	genie->GetComponent<WeaponComponent>()->UnEquip();
}

void GenieInit::OnExit()
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());
	genie->GetComponent<RigidBodyComponent>()
		->SetID(CollisionID::COLL_ID_ENEMY);
	genie->GetComponent<WeaponComponent>()->Equip();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GenieIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
GenieIdle* GenieIdle::Clone() const
{
	GenieIdle* clone = new GenieIdle();
	clone->_MakePrototype(false);

	EnemyIdle::Clone(clone);

	return clone;
}

void GenieIdle::Clone(GenieIdle* clone) const
{
	EnemyIdle::Clone(clone);
}

void GenieIdle::Update(Event* evnt)
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(genie->GetScene());
	double dist = Distance(genie->GetCoord(), dungeon->GetHero()->GetCoord());


	Coordinate target = genie->GetCenter();
	target.x += (genie->GetFacing() == DIR_LEFT) ? (-100) : (100);
	genie->GetWeapon()->SetTarget(target);

	if (dist < genie->GetScareRadius())
	{
		m_parent->ChangeBehavior("Retreat");
		return;
	}
	else if (dist < genie->GetAlertRadius())
	{
		m_parent->ChangeBehavior("Engage");
		return;
	}

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > DECISION_GAP)
	{
		if (Random(3) == 0)
			genie->ChangeFacing();
		else if (Random(2))
			m_parent->ChangeBehavior("Patrol");

		m_elapsedTime = 0L;
	}

	// Keep alert.
	_Alert();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GeniePatrol
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
GeniePatrol* GeniePatrol::Clone() const
{
	GeniePatrol* clone = new GeniePatrol();
	clone->_MakePrototype(false);

	EnemyBehavior::Clone(clone);

	return clone;
}

void GeniePatrol::Clone(GeniePatrol* clone) const
{
	EnemyBehavior::Clone(clone);
}

void GeniePatrol::Update(Event* evnt)
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(genie->GetScene());
	Coordinate center = genie->GetCoord();
	Coordinate hero = dungeon->GetHero()->GetCoord();

	if (genie->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	// Basic move.
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if ((IsTrivia(Distance(center, m_target))) || (m_elapsedTime > DECISION_GAP))
		m_target = dungeon->GetArena()->Wander(center);
	_Move((m_target == center) ? VECTOR_ZERO : GetDirection(center, m_target));
	_Evade(Settings::GetInstance()->Difficulty());
	_Collide();

	// Adjust posture.
	genie->SetFacing((hero.x < center.x) ? DIR_LEFT : DIR_RIGHT);
	Coordinate target = genie->GetCenter();
	target.x += (genie->GetFacing() == DIR_LEFT) ? (-100) : (100);
	genie->GetWeapon()->SetTarget(target);

	double dist = Distance(center, hero);
	if (dist > genie->GetAlertRadius())
	{
		m_parent->ChangeBehavior("Idle");
		return;
	}
	else if (dist < genie->GetAttackRadius())
	{
		m_parent->ChangeBehavior("Attack");
		return;
	}
}

void GeniePatrol::OnEnter()
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());

	genie->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(ENEMY_ANIM_MOVE);
	m_target = genie->GetCoord();
	m_elapsedTime = 0L;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GenieEngage
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
GenieEngage* GenieEngage::Clone() const
{
	GenieEngage* clone = new GenieEngage();
	clone->_MakePrototype(false);

	EnemyEngage::Clone(clone);

	return clone;
}

void GenieEngage::Clone(GenieEngage* clone) const
{
	EnemyEngage::Clone(clone);
}

void GenieEngage::Update(Event* evnt)
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(genie->GetScene());
	Coordinate center = genie->GetCoord();
	Coordinate hero = dungeon->GetHero()->GetCoord();
	Coordinate target = dungeon->GetArena()->Engage(center, hero);

	if (genie->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	_Move((target == center) ? VECTOR_ZERO : GetDirection(center, target));
	_Evade(Settings::GetInstance()->Difficulty());
	_Collide();

	genie->SetFacing((hero.x < center.x) ? DIR_LEFT : DIR_RIGHT);
	genie->GetWeapon()->SetTarget(hero);

	double dist = Distance(center, hero);
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > DECISION_GAP)
	{
		if (dist > genie->GetAlertRadius())
		{
			m_parent->ChangeBehavior("Idle");
			return;
		}
		m_elapsedTime = 0;
	}
	if (dist < genie->GetScareRadius())
		m_parent->ChangeBehavior("Retreat");
	else if (dist < genie->GetAttackRadius())
	{
		if (dungeon->GetArena()->InSight(center, hero))
			m_parent->ChangeBehavior("Attack");
	}
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GenieRetreat
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
GenieRetreat* GenieRetreat::Clone() const
{
	GenieRetreat* clone = new GenieRetreat();
	clone->_MakePrototype(false);

	EnemyBehavior::Clone(clone);

	return clone;
}

void GenieRetreat::Clone(GenieRetreat* clone) const
{
	EnemyBehavior::Clone(clone);
}

void GenieRetreat::Update(Event* evnt)
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(genie->GetScene());
	Coordinate center = genie->GetCoord();
	Coordinate hero = dungeon->GetHero()->GetCoord();
	Arena* arena = dungeon->GetArena();
	Coordinate target = arena->Retreat(center, hero);

	if (genie->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	_Move((target == center) ? VECTOR_ZERO : GetDirection(center, target));
	_Evade(Settings::GetInstance()->Difficulty());
	_Collide();

	genie->SetFacing((hero.x < center.x) ? DIR_LEFT : DIR_RIGHT);
	genie->GetWeapon()->SetTarget(hero);

	double dist = Distance(center, hero);
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > DECISION_GAP)
	{
		if (dist > genie->GetAttackRadius())
		{
			m_parent->ChangeBehavior("Idle");
			return;
		}
		m_elapsedTime = 0;
	}
}

void GenieRetreat::OnEnter()
{
	m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(ENEMY_ANIM_MOVE);
	m_elapsedTime = 0L;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GenieAttack
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
GenieAttack* GenieAttack::Clone() const
{
	GenieAttack* clone = new GenieAttack();
	clone->_MakePrototype(false);

	EnemyAttack::Clone(clone);

	return clone;
}

void GenieAttack::Clone(GenieAttack* clone) const
{
	EnemyAttack::Clone(clone);
}

void GenieAttack::Update(Event* evnt)
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());
	Coordinate center = genie->GetCoord();
	Coordinate hero = static_cast<Dungeon*>(genie->GetScene())->GetHero()->GetCoord();
	
	if (genie->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	_Evade(Settings::GetInstance()->Difficulty());
	_Collide();

	genie->SetFacing((hero.x < center.x) ? DIR_LEFT : DIR_RIGHT);
	genie->GetWeapon()->SetTarget(hero);

	if (genie->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Rest");
	}
}

void GenieAttack::OnEnter()
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());
	Coordinate hero = static_cast<Dungeon*>(genie->GetScene())->GetHero()->GetCenter();

	genie->GetComponent<AnimComponent>()->GetAnim()
		->SetMotion(ENEMY_ANIM_ATTACK);

	if (hero.x < genie->GetCoord().x)
		genie->SetFacing(DIR_LEFT);
	else
		genie->SetFacing(DIR_RIGHT);
	genie->GetWeapon()->SetTarget(hero);
	genie->GetWeapon()->Trig();
}

void GenieAttack::OnExit()
{
	static_cast<Genie*>(m_parent->GetGameObject())
		->GetWeapon()->UnTrig();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GenieRest
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
GenieRest* GenieRest::Clone() const
{
	GenieRest* clone = new GenieRest();
	clone->_MakePrototype(false);

	EnemyRest::Clone(clone);

	return clone;
}

void GenieRest::Clone(GenieRest* clone) const
{
	EnemyRest::Clone(clone);
}

void GenieRest::Update(Event* evnt)
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());
	Coordinate center = genie->GetCoord();
	Coordinate hero = static_cast<Dungeon*>(genie->GetScene())->GetHero()->GetCoord();
	
	if (genie->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	_Evade(Settings::GetInstance()->Difficulty());
	_Collide();

	genie->SetFacing((hero.x < center.x) ? DIR_LEFT : DIR_RIGHT);

	Coordinate target = genie->GetCenter();
	target.x += (genie->GetFacing() == DIR_LEFT) ? (-100) : (100);
	genie->GetWeapon()->SetTarget(target);

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > m_duration)
	{
		m_parent->ChangeBehavior("Idle");
		return;
	}

	_Alert();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GenieDead
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
GenieDead* GenieDead::Clone() const
{
	GenieDead* clone = new GenieDead();
	clone->_MakePrototype(false);

	EnemyDead::Clone(clone);

	return clone;
}

void GenieDead::Clone(GenieDead* clone) const
{
	EnemyDead::Clone(clone);
}

void GenieDead::OnEnter()
{
	Genie* genie = static_cast<Genie*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(genie->GetScene());
	genie->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(ENEMY_ANIM_DEAD);
	genie->GetComponent<RigidBodyComponent>()
		->SetID(CollisionID::COLL_ID_SPECTER);
	genie->GetComponent<WeaponComponent>()->UnEquip();

	dungeon->RemoveEnemy();

	_OnDefeat(dungeon, genie);
}