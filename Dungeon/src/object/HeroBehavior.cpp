/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroBehavior.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 26, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Behavior of hero. Generally, they are the same.                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/common/Math.h"

#include "../../inc/object/Hero.h"
#include "../../inc/object/HeroBehavior.h"
#include "../../inc/object/Weapon.h"
#include "../../inc/object/Component.h"

#include "../../inc/game/Dungeon.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HeroBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void HeroBehavior::Clone(HeroBehavior* clone) const
{
	Behavior::Clone(clone);
}

void HeroBehavior::_Collide()
{
	Hero* hero = static_cast<Hero*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(hero->GetScene());
	QuadTree* tree = dungeon->GetQuadTree();

	std::vector<GameObject*> candidates = tree->Query(hero);
	for (auto it = candidates.begin(); it != candidates.end(); it++)
	{
		if (*it != hero)
			Collider::Collide(hero, *it);
	}
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HeroInit
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HeroInit* HeroInit::Clone() const
{
	HeroInit* clone = new HeroInit();
	clone->_MakePrototype(false);

	HeroBehavior::Clone(clone);

	return clone;
}

void HeroInit::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Move");
	}
}

void HeroInit::OnEnter()
{
	Hero* hero = static_cast<Hero*>(m_parent->GetGameObject());

	Animation* anim = hero->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(HERO_ANIM_INIT);
	anim->SetDir((AnimDirection)Random(2));
	
	hero->GetComponent<WeaponComponent>()->UnEquip();
	hero->GetComponent<RigidBodyComponent>()->Freeze();
}

void HeroInit::OnExit()
{
	m_parent->GetGameObject()->GetComponent<WeaponComponent>()->Equip();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HeroMove
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HeroMove* HeroMove::Clone() const
{
	HeroMove* clone = new HeroMove();
	clone->_MakePrototype(false);

	HeroBehavior::Clone(clone);

	return clone;
}

void HeroMove::Update(Event* evnt)
{
	Hero* hero = static_cast<Hero*>(m_parent->GetGameObject());

	hero->GetWeapon()->SetTarget(static_cast<Dungeon*>(hero->GetScene())->GetMouse());
	if (hero->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	hero->UpdateProperty();

	_Move(evnt);
	_Act(evnt);
	_Collide();
}

void HeroMove::OnEnter()
{
	m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(HERO_ANIM_IDLE);
}

void HeroMove::_Move(Event* evnt)
{
	Hero* hero = static_cast<Hero*>(m_parent->GetGameObject());
	Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim();
	Vector force;

	if (evnt->Instant(CMD_LEFT))
		force += VECTOR_NX;
	if (evnt->Instant(CMD_RIGHT))
		force += VECTOR_PX;
	if (evnt->Instant(CMD_UP))
		force += VECTOR_NY;
	if (evnt->Instant(CMD_DOWN))
		force += VECTOR_PY;

	// Now, anim is based on command, rather than actual speed.
	if (!IsZero(force))
	{
		RigidBodyComponent* rigid = hero->GetComponent<RigidBodyComponent>();
		rigid->ApplyForce(Unitize(force) * rigid->GetStrength());
		anim->SetMotion(HERO_ANIM_MOVE);
	}
	else
		anim->SetMotion(HERO_ANIM_IDLE);

	if (static_cast<Dungeon*>(hero->GetScene())->GetMouse().x < hero->GetCoord().x)
		anim->SetDir(ANIM_LEFT);
	else
		anim->SetDir(ANIM_RIGHT);
}

void HeroMove::_Act(Event* evnt)
{
	Hero* hero = static_cast<Hero*>(m_parent->GetGameObject());

	// Fire
	if (evnt->Instant(CMD_FIRE))
		hero->GetComponent<WeaponComponent>()->TrigWeapon();
	else
		hero->GetComponent<WeaponComponent>()->UnTrigWeapon();

	// Skill
	if (evnt->Sluggish(CMD_SKILL))
	{
		if (hero->GetChi() >= hero->GetSkillCost())
		{
			hero->CostChi(hero->GetSkillCost());
			hero->GetComponent<StateComponent>()->ChangeState("Skill");
		}
	}

	// Switch weapon
	if (evnt->Sluggish(CMD_SWITCH))
		hero->GetComponent<WeaponComponent>()->SwitchWeapon();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HeroDead
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HeroDead* HeroDead::Clone() const
{
	HeroDead* clone = new HeroDead();
	clone->_MakePrototype(false);

	HeroBehavior::Clone(clone);

	return clone;
}

void HeroDead::Update(Event* evnt)
{
	_Collide();

	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Perish");
	}
}

void HeroDead::OnEnter()
{
	GameObject* hero = m_parent->GetGameObject();
	
	Animation* anim = hero->GetComponent<AnimComponent>()->GetAnim();
	anim->SetMotion(HERO_ANIM_DEAD);

	hero->GetComponent<StateComponent>()->ChangeState("None");
	hero->GetComponent<WeaponComponent>()->UnEquip();
	hero->GetComponent<RigidBodyComponent>()->SetID(CollisionID::COLL_ID_SPECTER);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HeroPerish
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HeroPerish* HeroPerish::Clone() const
{
	HeroPerish* clone = new HeroPerish();
	clone->_MakePrototype(false);

	HeroBehavior::Clone(clone);

	return clone;
}

void HeroPerish::OnEnter()
{
	m_parent->GetGameObject()->SetValid(false);
}