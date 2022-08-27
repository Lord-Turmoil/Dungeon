/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : WeaponBehavior.cpp                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 24, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For common weapon behaviors.                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/common/Math.h"

#include "../../inc/object/Weapon.h"
#include "../../inc/object/WeaponBehavior.h"
#include "../../inc/object/Hero.h"

#include "../../inc/object/Bullet.h"

#include "../../inc/object/Component.h"
#include "../../inc/object/Library.h"

#include "../../inc/game/Dungeon.h"



/********************************************************************
** Notice that weapon need adjust direction.
*/


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** WeaponBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void WeaponBehavior::Clone(WeaponBehavior* clone) const
{
	Behavior::Clone(clone);
}

void WeaponBehavior::_Fire()
{
	Weapon* weapon = static_cast<Weapon*>(m_parent->GetGameObject());
	Figure* figure = static_cast<Figure*>(weapon->GetSlot()->GetGameObject());

	int burst = weapon->GetBurstNum();
	double error = HALF_PI * (1.0 - weapon->GetAccuracy());
	double radian = weapon->GetSymbol()->GetAttribute()->rotationAngle;
	Coordinate center = weapon->GetCoord();
	Coordinate target = weapon->GetTarget();
	Coordinate muzzle;
	Dungeon* dungeon = static_cast<Dungeon*>(figure->GetScene());
	Bullet* proto = BulletLibrary::GetInstance()->
		GetBulletByName(weapon->BulletName());
	Bullet* bullet;

	if (target.x < center.x)
		muzzle = center + Rotate(weapon->GetMuzzleOffset().GetMirrorX(), radian);
	else
		muzzle = center + Rotate(weapon->GetMuzzleOffset(), radian);

	for (int i = 0; i < burst; i++)
	{
		bullet = proto->Clone();
		bullet->SetDirection(Rotate(GetDirection(center, target), Random(-error, error)));
		bullet->SetCoord(muzzle);
		bullet->Activate();
		dungeon->AddObject(bullet);
	}
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** WeaponIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
WeaponIdle* WeaponIdle::Clone() const
{
	WeaponIdle* clone = new WeaponIdle();
	clone->_MakePrototype(false);

	WeaponBehavior::Clone(clone);

	return clone;
}

void WeaponIdle::Update(Event* evnt)
{
	Weapon* weapon = static_cast<Weapon*>(m_parent->GetGameObject());
	Hero * hero = static_cast<Dungeon*>(weapon->GetScene())->GetHero();
	
	double dist = Distance(hero->GetCenter(), weapon->GetCenter());

	if (dist < weapon->GetRadius())
	{
		m_parent->GetGameObject()->GetComponent<AnimComponent>()
			->GetAnim()->SetDir(ANIM_RIGHT);
		if (evnt->Sluggish(CMD_ACTIVATE))
			hero->PickUpWeapon(weapon);
	}
	else
	{
		m_parent->GetGameObject()->GetComponent<AnimComponent>()
			->GetAnim()->SetDir(ANIM_LEFT);
	}
}


void WeaponIdle::OnEnter()
{
	Weapon* weapon = static_cast<Weapon*>(m_parent->GetGameObject());
	Animation* anim = weapon->GetComponent<AnimComponent>()->GetAnim();
	
	anim->SetMotion(WEAPON_ANIM_IDLE);
	anim->SetDir(ANIM_LEFT);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** WeaponUnequipped
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
WeaponUnequipped* WeaponUnequipped::Clone() const
{
	WeaponUnequipped* clone = new WeaponUnequipped();
	clone->_MakePrototype(false);

	WeaponBehavior::Clone(clone);

	return clone;
}

void WeaponUnequipped::OnEnter()
{
	Weapon* weapon = static_cast<Weapon*>(m_parent->GetGameObject());
	weapon->GetSymbol()->GetAttribute()->rotationAngle = 0.0;
	weapon->UnTrig();
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** WeaponReady
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void WeaponReady::Clone(WeaponReady* clone) const
{
	WeaponBehavior::Clone(clone);
}

void WeaponReady::OnEnter()
{
	m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(WEAPON_ANIM_READY);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** WeaponFire
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
WeaponFire* WeaponFire::Clone() const
{
	WeaponFire* clone = new WeaponFire();
	clone->_MakePrototype(false);

	WeaponBehavior::Clone(clone);

	return clone;
}

void WeaponFire::Update(Event* evnt)
{
	Weapon* weapon = static_cast<Weapon*>(m_parent->GetGameObject());
	
	weapon->AdjustPosture();
	if (weapon->GetComponent<AnimComponent>()->GetAnim()->IsOver())
		m_parent->ChangeBehavior("Cooling");
}

void WeaponFire::OnEnter()
{
	Weapon* weapon = static_cast<Weapon*>(m_parent->GetGameObject());

	weapon->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(WEAPON_ANIM_FIRE);
	weapon->GetComponent<SoundComponent>()->Play("fire");

	_Fire();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** WeaponCooling
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
WeaponCooling* WeaponCooling::Clone() const
{
	WeaponCooling* clone = new WeaponCooling();
	clone->_MakePrototype(false);

	WeaponBehavior::Clone(clone);

	return clone;
}

void WeaponCooling::Update(Event* evnt)
{
	Weapon* weapon = static_cast<Weapon*>(m_parent->GetGameObject());

	weapon->AdjustPosture();

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > m_coolingTime)
	{
		m_parent->ChangeBehavior("Ready");
		return;
	}
}

void WeaponCooling::OnEnter()
{
	Weapon* weapon = static_cast<Weapon*>(m_parent->GetGameObject());
	
	weapon->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(WEAPON_ANIM_COOLING);

	m_coolingTime = weapon->GetCoolingTime();
	m_elapsedTime = 0;
}