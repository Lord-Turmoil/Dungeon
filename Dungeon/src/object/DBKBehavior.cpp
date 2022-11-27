/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : DBKBehavior.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 25, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Dark Black Hand behavior.                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Boss.h"
#include "../../inc/object/BossBehavior.h"
#include "../../inc/object/Enemy.h"
#include "../../inc/object/Hero.h"
#include "../../inc/object/Weapon.h"
#include "../../inc/object/Component.h"
#include "../../inc/object/Library.h"

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Settings.h"

#include "../../inc/terrain/Terrain.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DBKInit
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DBKInit* DBKInit::Clone() const
{
	DBKInit* clone = new DBKInit();
	clone->_MakePrototype(false);

	GenieInit::Clone(clone);

	return clone;
}

void DBKInit::OnExit()
{
	EBlackHand* black = static_cast<EBlackHand*>(m_parent->GetGameObject());
	black->GetComponent<RigidBodyComponent>()
		->SetID(CollisionID::COLL_ID_ENEMY);
	black->GetComponent<WeaponComponent>()->Equip();
	black->GetComponent<SoundComponent>()->Play("init");
	black->CostMP(INF_INT);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DBKIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DBKIdle* DBKIdle::Clone() const
{
	DBKIdle* clone = new DBKIdle();
	clone->_MakePrototype(false);

	GenieIdle::Clone(clone);

	return clone;
}

void DBKIdle::Update(Event* evnt)
{
	EBlackHand* black = static_cast<EBlackHand*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(black->GetScene());
	double dist = Distance(black->GetCoord(), dungeon->GetHero()->GetCoord());

	black->UpdateProperty();

	Coordinate target = black->GetCenter();
	target.x += (black->GetFacing() == DIR_LEFT) ? (-100) : (100);
	black->GetWeapon()->SetTarget(target);

	if (dist < black->GetScareRadius())
	{
		m_parent->ChangeBehavior("Retreat");
		return;
	}
	else if (dist < black->GetAlertRadius())
	{
		m_parent->ChangeBehavior("Engage");
		return;
	}

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > DECISION_GAP)
	{
		if (Random(3) == 0)
			black->ChangeFacing();

		m_elapsedTime = 0L;
	}

	// Keep alert.
	_Alert();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DBKEngage
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DBKEngage* DBKEngage::Clone() const
{
	DBKEngage* clone = new DBKEngage();
	clone->_MakePrototype(false);

	GenieEngage::Clone(clone);

	return clone;
}

void DBKEngage::Update(Event* evnt)
{
	EBlackHand* black = static_cast<EBlackHand*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(black->GetScene());
	Arena* arena = dungeon->GetArena();
	Coordinate center = black->GetCoord();
	Coordinate hero = dungeon->GetHero()->GetCoord();
	Coordinate target = arena->Engage(center, hero);

	if (black->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	black->UpdateProperty();
	_Move((target == center) ? VECTOR_ZERO : GetDirection(center, target));
	_Evade(Settings::GetInstance()->Difficulty());
	_Collide();

	black->SetFacing((hero.x < center.x) ? DIR_LEFT : DIR_RIGHT);
	black->GetWeapon()->SetTarget(hero);

	double dist = Distance(center, hero);
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > DECISION_GAP)
	{
		if (dist > black->GetAlertRadius())
		{
			m_parent->ChangeBehavior("Idle");
			return;
		}
		m_elapsedTime = 0;
	}

	black->GetWeapon()->UnTrig();
	if (dist < black->GetScareRadius())
		m_parent->ChangeBehavior("Retreat");
	else if ((dist < black->GetAttackRadius()) && (arena->InSight(center, hero)))
	{
		if (black->GetChi() >= black->GetSkillCost())
		{
			black->CostChi(black->GetSkillCost());
			black->GetComponent<StateComponent>()->ChangeState("Skill");
		}
		else if (!black->IsUsingSkill())
			black->GetWeapon()->Trig();
	}

	double percent = black->GetHPPercent();
	int mp = black->GetMP();
	if ((mp == 2) && (percent < 0.25))
	{
		black->HealMP(1);
		_OnDefeat(dungeon, black);
	}
	else if ((mp == 1) && (percent < 0.5))
	{
		black->HealMP(1);
		_OnDefeat(dungeon, black);
	}
	else if ((mp == 0) && (percent < 0.75))
	{
		black->HealMP(1);
		_OnDefeat(dungeon, black);
	}
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DBKRetreat
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DBKRetreat* DBKRetreat::Clone() const
{
	DBKRetreat* clone = new DBKRetreat();
	clone->_MakePrototype(false);

	GenieRetreat::Clone(clone);

	return clone;
}

void DBKRetreat::Update(Event* evnt)
{
	EBlackHand* black = static_cast<EBlackHand*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(black->GetScene());
	Arena* arena = dungeon->GetArena();
	Coordinate center = black->GetCoord();
	Coordinate hero = dungeon->GetHero()->GetCoord();
	Coordinate target = arena->Retreat(center, hero);

	if (black->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}
	
	black->UpdateProperty();
	_Move((target == center) ? VECTOR_ZERO : GetDirection(center, target));
	_Evade(Settings::GetInstance()->Difficulty());
	_Collide();

	black->SetFacing((hero.x < center.x) ? DIR_LEFT : DIR_RIGHT);
	black->GetWeapon()->SetTarget(hero);

	double dist = Distance(center, hero);
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > DECISION_GAP)
	{
		if (dist > black->GetScareRadius() * 1.2)
		{
			m_parent->ChangeBehavior("Idle");
			return;
		}
		m_elapsedTime = 0;
	}

	if ((dist < black->GetAttackRadius()) && (arena->InSight(center, hero)))
		black->GetWeapon()->Trig();
	else
		black->GetWeapon()->UnTrig();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DBKDead
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DBKDead* DBKDead::Clone() const
{
	DBKDead* clone = new DBKDead();
	clone->_MakePrototype(false);

	BossDead::Clone(clone);

	return clone;
}

void DBKDead::OnEnter()
{
	EBlackHand* black = static_cast<EBlackHand*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(black->GetScene());

	black->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(ENEMY_ANIM_DEAD);
	black->GetComponent<WeaponComponent>()->UnEquip();

	dungeon->RemoveBoss();
	dungeon->KillAllEnemy();

	_OnDefeat(dungeon, black);
}