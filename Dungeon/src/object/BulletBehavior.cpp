/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BulletBehavior.cpp                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Behavior of basic bullets.                                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Math.h"

#include "../../inc/object/BulletBehavior.h"
#include "../../inc/object/Bullet.h"
#include "../../inc/object/Figure.h"

#include "../../inc/game/Dungeon.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BulletBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void BulletBehavior::Clone(BulletBehavior* clone) const
{
	Behavior::Clone(clone);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BulletNone
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BulletNone* BulletNone::Clone() const
{
	BulletNone* clone = new BulletNone();
	clone->_MakePrototype(false);

	BulletBehavior::Clone(clone);

	return clone;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BulletFly
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BulletFly* BulletFly::Clone() const
{
	BulletFly* clone = new BulletFly();
	clone->_MakePrototype(false);

	BulletBehavior::Clone(clone);

	clone->m_start = m_start;
	// No need to clone candidates.

	return clone;
}

void BulletFly::Update(Event* evnt)
{
	Bullet* bullet = static_cast<Bullet*>(m_parent->GetGameObject());

	if ((Distance(bullet->GetCoord(), m_start) > bullet->GetRange()) || \
		(IsTrivia(bullet->GetComponent<RigidBodyComponent>()->GetVelocity())))
	{
		m_parent->ChangeBehavior("Corrupt");
		return;
	}
	
	Dungeon* dungeon = static_cast<Dungeon*>(bullet->GetScene());
	QuadTree* tree = dungeon->GetQuadTree();

	/*
	** A little flaw that... Query returns game objects in a rectangle area...
	*/
	m_candidates.clear();
	tree->Query(bullet, m_candidates);
	for (auto it = m_candidates.begin(); it != m_candidates.end(); it++)
	{
		if (Collider::Collide(bullet, *it))
		{
			/*
			** if didn't hit wall (hit enemy) and is penetrable,
			** then won't go to explode.
			*/
			if (!(!_Explode(*it) && bullet->IsPenetrable()))
				m_parent->ChangeBehavior("Explode");
			break;
		}
	}
}

void BulletFly::OnEnter()
{
	Bullet* bullet = static_cast<Bullet*>(m_parent->GetGameObject());

	bullet->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(BULLET_ANIM_FLY);

	RigidBodyComponent* rigid = bullet->GetComponent<RigidBodyComponent>();
	Vector dir = bullet->GetDirection();
	rigid->SetVelocity(dir * rigid->GetMaxVelocity());
	if (bullet->IsDirectional())
	{
		bullet->GetSymbol()->GetAttribute()->rotationAngle =
			GetRotationRadian(dir, false);
	}

	m_start = bullet->GetCoord();
}

/********************************************************************
** Return whether hit wall or not.
*/
bool BulletFly::_Explode(GameObject* obj)
{
	Bullet* bullet = static_cast<Bullet*>(m_parent->GetGameObject());
	Object* target = static_cast<Object*>(obj);
	Figure* victim;
	bool hitWall = false;

	if (target->Type() & ObjectType::OBJ_FIGURE)
	{
		victim = static_cast<Figure*>(target);
		victim->Hurt(bullet->GetDamage());
		victim->GetComponent<RigidBodyComponent>()
			->ApplyForce(GetDirection(bullet->GetCoord(), victim->GetCoord()) * bullet->GetForce());
	}
	else
		hitWall = true;

	double radius = bullet->GetAOERadius();
	if (radius < EPSILON)	// no AOE
		return hitWall;

	Coordinate src = bullet->GetCoord();
	Coordinate dest;
	int damage = bullet->GetDamage();
	double force = bullet->GetForce();
	double dist;
	double ratio;
	for (auto it = m_candidates.begin(); it != m_candidates.end(); it++)
	{
		if (*it == obj)
			continue;

		target = static_cast<Object*>(*it);
		if (target->Type() & ObjectType::OBJ_FIGURE)
		{
			victim = static_cast<Figure*>(target);
			dest = victim->GetCoord();
			dist = Distance(src, dest);
			if (dist > radius)
				continue;
			ratio = 1.0 - dist / radius;

			victim->Hurt((int)(damage * ratio));
			victim->GetComponent<RigidBodyComponent>()
				->ApplyForce(GetDirection(src, dest) * force * ratio);
		}
		else if (target->Type() & ObjectType::OBJ_BRICK)
			hitWall = true;
	}

	return hitWall;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BulletExplode
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BulletExplode* BulletExplode::Clone() const
{
	BulletExplode* clone = new BulletExplode();
	clone->_MakePrototype(false);

	BulletBehavior::Clone(clone);

	return clone;
}

void BulletExplode::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Perish");
	}
}

void BulletExplode::OnEnter()
{
	Bullet* bullet = static_cast<Bullet*>(m_parent->GetGameObject());

	bullet->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(BULLET_ANIM_EXPLODE);
	bullet->GetComponent<RigidBodyComponent>()->Freeze();

	bullet->SetGood(false);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BulletCorrupt
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BulletCorrupt* BulletCorrupt::Clone() const
{
	BulletCorrupt* clone = new BulletCorrupt();
	clone->_MakePrototype(false);

	BulletBehavior::Clone(clone);

	return clone;
}

void BulletCorrupt::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Perish");
	}
}

void BulletCorrupt::OnEnter()
{
	Bullet* bullet = static_cast<Bullet*>(m_parent->GetGameObject());

	bullet->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(BULLET_ANIM_CORRUPT);
	bullet->GetComponent<RigidBodyComponent>()->Freeze();
	
	bullet->SetGood(false);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BulletPerish
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BulletPerish* BulletPerish::Clone() const
{
	BulletPerish* clone = new BulletPerish();
	clone->_MakePrototype(false);

	BulletBehavior::Clone(clone);

	return clone;
}

void BulletPerish::Update(Event* evnt)
{
}

void BulletPerish::OnEnter()
{
	m_parent->GetGameObject()->SetValid(false);
}