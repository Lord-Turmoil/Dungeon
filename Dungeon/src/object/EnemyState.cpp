/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyState.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 28, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   State of enemy.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/object/Enemy.h"
#include "../../inc/object/EnemyState.h"
#include "../../inc/object/Boss.h"
#include "../../inc/object/Bullet.h"
#include "../../inc/object/Library.h"

#include "../../inc/game/Dungeon.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyState
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void EnemyState::Clone(EnemyState* clone) const
{
	State::Clone(clone);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyNone
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnemyNone* EnemyNone::Clone() const
{
	EnemyNone* clone = new EnemyNone();
	clone->_MakePrototype(false);

	EnemyState::Clone(clone);

	return clone;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnemyHurt
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnemyHurt* EnemyHurt::Clone() const
{
	EnemyHurt* clone = new EnemyHurt();
	clone->_MakePrototype(false);

	EnemyHurt::Clone(clone);

	return clone;
}

void EnemyHurt::Update(Event* evnt)
{
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	if (m_elapsedTime > m_duration)
		m_parent->ChangeState("None");
}

void EnemyHurt::OnEnter()
{
	Enemy* enemy = static_cast<Enemy*>(m_parent->GetGameObject());

	enemy->GetSymbol()->GetAttribute()->alpha = 200;
	m_duration = enemy->GetHurtDuration();
	m_elapsedTime = 0L;
}

void EnemyHurt::OnExit()
{
	m_parent->GetGameObject()->GetSymbol()->GetAttribute()->alpha = 255;
}



/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DBKSkill
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
const int DBKSkill::m_FLAME_NUM = 64;
const int DBKSkill::m_FLAME_RADIUS = 20;
const double DBKSkill::m_DELTA = TWO_PI / (double)m_FLAME_NUM;

DBKSkill* DBKSkill::Clone() const
{
	DBKSkill* clone = new DBKSkill();
	clone->_MakePrototype(false);

	EnemyState::Clone(clone);

	return clone;
}

void DBKSkill::Update(Event* evnt)
{
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	if (m_elapsedTime > m_duration)
		m_parent->ChangeState("None");
}

/*
** Nothing special??
** This skill can only be used once, so skill using flag won't
** be cleared.
*/
void DBKSkill::OnEnter()
{
	EBlackHand* black = static_cast<EBlackHand*>(m_parent->GetGameObject());

	m_duration = black->GetSkillDuration();
	m_elapsedTime = 0L;

	black->SetUsingSkill(true);

	_ThrowFlame();
}

void DBKSkill::OnExit()
{
	EBlackHand* black = static_cast<EBlackHand*>(m_parent->GetGameObject());
	black->SetUsingSkill(false);
}

void DBKSkill::_ThrowFlame()
{
	Bullet* protoFlame = BulletLibrary::GetInstance()->GetBulletByName("DFlame");
	Bullet* flame;
	EBlackHand* black = static_cast<EBlackHand*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(black->GetScene());
	Coordinate center = black->GetCoord();
	Coordinate start;
	Vector dir;

	double radian = 0.0;
	for (int i = 0; i < m_FLAME_NUM; i++)
	{
		dir = Rotate(VECTOR_NX, radian);
		start.x = center.x + (int)(dir.x * m_FLAME_RADIUS);
		start.y = center.y + (int)(dir.y * m_FLAME_RADIUS);

		flame = protoFlame->Clone();
		flame->SetCoord(start);
		flame->SetDirection(dir);
		flame->Activate();

		dungeon->AddObject(flame);

		radian += m_DELTA;
	}
}