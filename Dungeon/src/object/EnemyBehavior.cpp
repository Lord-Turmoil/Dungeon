/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyBehavior.cpp                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 28, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Behaviors of enemies.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/common/Defines.h"

#include "../../inc/object/Enemy.h"
#include "../../inc/object/EnemyBehavior.h"
#include "../../inc/object/Hero.h"
#include "../../inc/object/Buff.h"
#include "../../inc/object/Bullet.h"
#include "../../inc/object/WeaponComponent.h"

#include "../../inc/object/Library.h"

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Settings.h"

#include "../../inc/terrain/Terrain.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void EnemyBehavior::Clone(EnemyBehavior* clone) const
{
	Behavior::Clone(clone);
	clone->m_elapsedTime = m_elapsedTime;
}

bool EnemyBehavior::_Alert()
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());
	StateComponent* state = enemy->GetComponent<StateComponent>();

	if (_STR_SAME(state->CurrentStateName(), "Hurt"))
	{
		m_parent->ChangeBehavior("Engage");
		return true;
	}
	
	return false;
}

void EnemyBehavior::_Move(const Vector& dir)
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());
	RigidBodyComponent* rigid = enemy->GetComponent<RigidBodyComponent>();
	Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim();
	
	if (!IsZero(dir))
	{
		RigidBodyComponent* rigid = enemy->GetComponent<RigidBodyComponent>();
		rigid->ApplyForce(dir * rigid->GetStrength());
		anim->SetMotion(ENEMY_ANIM_MOVE);
	}
	else
		anim->SetMotion(ENEMY_ANIM_IDLE);
}


/******************************************************************************
 * EnemyBehavior::_Evade -- Evade bullets.                                    *
 *                                                                            *
 *    This allow enemy to evade incoming bullets. However, since I didn't add *
 *    tag to identify whether the bullet belongs to the figure or enemy, this *
 *    will also make enemies evade their own bullets. but, I filtered bullets *
 *    that moves away from the enemy, so it won't matter a lot.               *
 *                                                                            *
 *    Evade action depends on incoming angle and distance, and the coefficient*
 *    is determined by k = a * (1- cosx) * (1 - dist / maxDist), where a and  *
 *    maxDist are two constants. Then, all vecters will be sumed up, and apply*
 *    to the enemy as external force. :)                                      *
 *                                                                            *
 * INPUT:   level -- Evade level, the higher, the better.                     *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/29 Tony : Created.                                               *
 *============================================================================*/
void EnemyBehavior::_Evade(int level)
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(enemy->GetScene());
	QuadTree* tree = dungeon->GetQuadTree();
	std::vector<GameObject*> targets = tree->Query(enemy);
	RigidBodyComponent* enemyRigid = enemy->GetComponent<RigidBodyComponent>();
	RigidBodyComponent* rigid;
	Object* obj;

	const double A = (double)level * 8.0;
	const double B = (double)(level + 1) * 4.0;
	const double MAX_DIST = (double)(enemy->GetComponent<ColliderBoxComponent>()->GetBorderWidth());
	double cosx;
	Coordinate center = enemy->GetCoord();
	Vector v0 = enemyRigid->GetVelocity();
	Vector d;		// displacement from bullet to enemy
	Vector v;		// enemy's velocity
	Vector sum;		// sum of all result vectors
	for (auto it = targets.begin(); it != targets.end(); it++)
	{
		if ((*it) == enemy)
			continue;

		obj = static_cast<Object*>(*it);
		switch (obj->Type())
		{
		case ObjectType::OBJ_BULLET:
			rigid = obj->GetComponent<RigidBodyComponent>();

			d = GetDisplacement(obj->GetCoord(), center);
			v = rigid->GetVelocity() - v0;

			// Ignore if the bullet is flying away.
			cosx = GetAngleCosine(d, v);
			if (cosx > 0.0)
			{
				// Do not move towards the bullet. :)
				Normalize(v);
				if (GetAngleCosine(d, v) < 0.0)
					v.Mirror();

				sum += Unitize(v) * A * (1 - cosx) * (1 - Module(d) / MAX_DIST);
			}
			break;
		case ObjectType::OBJ_ENEMY:
			rigid = obj->GetComponent<RigidBodyComponent>();

			d = GetDisplacement(obj->GetCoord(), center);
			v = rigid->GetVelocity() - v0;

			// Ignore if the bullet is flying away.
			cosx = GetAngleCosine(d, v);
			if (cosx > 0.0)
			{
				// Do not move towards the bullet. :)
				Normalize(v);
				if (GetAngleCosine(d, v) < 0.0)
					v.Mirror();

				sum += Unitize(v) * B * (1 - cosx) * (1 - Module(d) / MAX_DIST);
			}
			break;
		default:
			break;
		}
	}

	enemyRigid->ApplyForce(enemyRigid->GetStrength() * sum);
}

void EnemyBehavior::_Collide()
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(enemy->GetScene());
	QuadTree* tree = dungeon->GetQuadTree();

	std::vector<GameObject*> candidates = tree->Query(enemy);
	for (auto it = candidates.begin(); it != candidates.end(); it++)
	{
		if (*it != enemy)
			Collider::Collide(enemy, *it);
	}
}

void EnemyBehavior::_OnDefeat(Dungeon* dungeon, Enemy* enemy)
{
	int num;
	int extra = Settings::GetInstance()->Difficulty();
	Coordinate center = enemy->GetCenter();

	// Generate coin.
	num = Random(enemy->GetCoin() + extra);
	if (num)
	{
		Coin* proto = MiscLibrary::GetInstance()->GetMiscObject<Coin>("Coin");
		Coin* coin;

		for (int i = 0; i < num; i++)
		{
			coin = proto->Clone();
			coin->SetCoord(center);
			coin->Activate();
			dungeon->AddObject(coin);
		}
	}

	// Generate energy.
	num = Random(enemy->GetEnergy() + extra);
	if (num)
	{
		Energy* proto = MiscLibrary::GetInstance()->GetMiscObject<Energy>("Energy");
		Energy* energy;
		for (int i = 0; i < num; i++)
		{
			energy = proto->Clone();
			energy->SetCoord(center);
			energy->Activate();
			dungeon->AddObject(energy);
		}
	}
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyInit
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnemyInit* EnemyInit::Clone() const
{
	EnemyInit* clone = new EnemyInit();
	clone->_MakePrototype(false);

	EnemyBehavior::Clone(clone);

	return clone;
}

void EnemyInit::Clone(EnemyInit* clone) const
{
	EnemyBehavior::Clone(clone);
}

void EnemyInit::Update(Event* evnt)
{
	_Collide();
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Idle");
	}
}

void EnemyInit::OnEnter()
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());

	Animation* anim = enemy->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(ENEMY_ANIM_INIT);
	anim->SetDir((AnimDirection)Random(2));

	enemy->GetComponent<RigidBodyComponent>()
		->SetID(CollisionID::COLL_ID_SPECTER);
}

void EnemyInit::OnExit()
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());

	enemy->GetComponent<RigidBodyComponent>()
		->SetID(CollisionID::COLL_ID_ENEMY);
}



/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnemyIdle* EnemyIdle::Clone() const
{
	EnemyIdle* clone = new EnemyIdle();
	clone->_MakePrototype(false);

	EnemyBehavior::Clone(clone);

	return clone;
}

void EnemyIdle::Clone(EnemyIdle* clone) const
{
	EnemyBehavior::Clone(clone);
}

void EnemyIdle::Update(Event* evnt)
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(enemy->GetScene());

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	if (Distance(enemy->GetCoord(), dungeon->GetHero()->GetCoord()) < enemy->GetAlertRadius())
	{
		m_parent->ChangeBehavior("Engage");
		return;
	}

	if (m_elapsedTime > DECISION_GAP)
	{
		if (Random(3) == 0)
			enemy->ChangeFacing();
		m_elapsedTime = 0L;
	}

	// Keep alert.
	_Alert();
}

void EnemyIdle::OnEnter()
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());

	enemy->GetComponent<AnimComponent>()->GetAnim()
		->SetMotion(ENEMY_ANIM_IDLE);

	m_elapsedTime = 0L;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyEngage
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnemyEngage* EnemyEngage::Clone() const
{
	EnemyEngage* clone = new EnemyEngage();
	clone->_MakePrototype(false);

	EnemyBehavior::Clone(clone);

	return clone;
}

void EnemyEngage::Clone(EnemyEngage* clone) const
{
	EnemyBehavior::Clone(clone);
}

void EnemyEngage::Update(Event* evnt)
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(enemy->GetScene());
	Coordinate center = enemy->GetCoord();
	Coordinate hero = dungeon->GetHero()->GetCoord();
	Coordinate target = dungeon->GetArena()->Engage(center, hero);

	if (enemy->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	
	/*
	printf("(%4d, %4d) -> (%4d %4d) -> (%4d, %4d)\r",
		center.x, center.y,
		target.x, target.y,
		hero.x, hero.y);
	*/

	_Move((target == center) ? VECTOR_ZERO : GetDirection(center, target));
	_Evade(Settings::GetInstance()->Difficulty());
	_Collide();

	if (hero.x < center.x)
		enemy->SetFacing(DIR_LEFT);
	else
		enemy->SetFacing(DIR_RIGHT);

	double dist = Distance(center, hero);
	if (m_elapsedTime > DECISION_GAP)
	{
		if (dist > enemy->GetAlertRadius())
		{
			m_parent->ChangeBehavior("Idle");
			return;
		}
		m_elapsedTime = 0;
	}
	if (dist < enemy->GetAttackRadius())
	{
		m_parent->ChangeBehavior("Attack");
	}
}

void EnemyEngage::OnEnter()
{
	m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(ENEMY_ANIM_MOVE);
	m_elapsedTime = 0L;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyAttack
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnemyAttack* EnemyAttack::Clone() const
{
	EnemyAttack* clone = new EnemyAttack();
	clone->_MakePrototype(false);

	EnemyBehavior::Clone(clone);

	return clone;
}

void EnemyAttack::Clone(EnemyAttack* clone) const
{
	EnemyBehavior::Clone(clone);
}

void EnemyAttack::Update(Event* evnt)
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());
	
	_Collide();

	if (enemy->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	if (enemy->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Rest");
	}
}

void EnemyAttack::OnEnter()
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(enemy->GetScene())->GetHero();

	RigidBodyComponent* enemyRigid = enemy->GetComponent<RigidBodyComponent>();
	RigidBodyComponent* heroRigid = hero->GetComponent<RigidBodyComponent>();
	double strength = enemyRigid->GetStrength();
	Vector dir = GetDirection(enemy->GetCoord(), hero->GetCoord());
	
	heroRigid->ApplyForce(Random(strength, strength * 10) * dir);
	hero->Hurt(enemy->GetDamage());

	enemy->GetComponent<AnimComponent>()->GetAnim()
		->SetMotion(ENEMY_ANIM_ATTACK);

	if (dir.x < 0)
		enemy->SetFacing(DIR_LEFT);
	else
		enemy->SetFacing(DIR_RIGHT);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyRest
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnemyRest* EnemyRest::Clone() const
{
	EnemyRest* clone = new EnemyRest();
	clone->_MakePrototype(false);

	EnemyBehavior::Clone(clone);

	return clone;
}

void EnemyRest::Clone(EnemyRest* clone) const
{
	EnemyBehavior::Clone(clone);
}

void EnemyRest::Update(Event* evnt)
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());

	if (enemy->IsDead())
	{
		m_parent->ChangeBehavior("Dead");
		return;
	}

	_Collide();

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > m_duration)
	{
		m_parent->ChangeBehavior("Idle");
		return;
	}

	_Alert();
}

void EnemyRest::OnEnter()
{
	m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim()
		->SetMotion(ENEMY_ANIM_IDLE);

	m_elapsedTime = 0L;
	
	clock_t duration = static_cast<Enemy*>(m_parent->GetGameObject())->GetRestDuration();
	m_duration = Random((int)(duration << 1), (int)(duration >> 1));
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyDead
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnemyDead* EnemyDead::Clone() const
{
	EnemyDead* clone = new EnemyDead();
	clone->_MakePrototype(false);

	EnemyBehavior::Clone(clone);

	return clone;
}

void EnemyDead::Clone(EnemyDead* clone) const
{
	EnemyBehavior::Clone(clone);
}

void EnemyDead::Update(Event* evnt)
{
	_Collide();
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Perish");
	}
}

void EnemyDead::OnEnter()
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(enemy->GetScene());
	enemy->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(ENEMY_ANIM_DEAD);
	enemy->GetComponent<RigidBodyComponent>()
		->SetID(CollisionID::COLL_ID_SPECTER);

	dungeon->RemoveEnemy();

	_OnDefeat(dungeon, enemy);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyPerish
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnemyPerish* EnemyPerish::Clone() const
{
	EnemyPerish* clone = new EnemyPerish();
	clone->_MakePrototype(false);

	EnemyBehavior::Clone(clone);

	return clone;
}

void EnemyPerish::OnEnter()
{
	m_parent->GetGameObject()->SetValid(false);
}
