/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : CoinBehavior.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Coin behavior.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Buff.h"
#include "../../inc/object/Hero.h"
#include "../../inc/object/CoinBehavior.h"

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Settings.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CoinBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void CoinBehavior::Clone(CoinBehavior* clone) const
{
	Behavior::Clone(clone);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CoinFly
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
CoinFly* CoinFly::Clone() const
{
	CoinFly* clone = new CoinFly();
	clone->_MakePrototype(false);

	CoinBehavior::Clone(clone);

	return clone;
}

void CoinFly::Update(Event* evnt)
{
	Coin* coin = static_cast<Coin*>(m_parent->GetGameObject());
	RigidBodyComponent* rigid = coin->GetComponent<RigidBodyComponent>();
	Coordinate center = coin->GetCoord();
	Coordinate target = static_cast<Dungeon*>(coin->GetScene())->GetHero()->GetCenter();

	double dist = Distance(center, target);
	if (dist < coin->GetRadius())
	{
		m_parent->ChangeBehavior("Active");
		return;
	}

	rigid->ApplyForce(GetDirection(center, target) * rigid->GetStrength());
}

void CoinFly::OnEnter()
{
	m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(COIN_ANIM_FLY);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CoinActive
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
CoinActive* CoinActive::Clone() const
{
	CoinActive* clone = new CoinActive();
	clone->_MakePrototype(false);

	CoinBehavior::Clone(clone);

	return clone;
}

void CoinActive::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Perish");
	}
}

void CoinActive::OnEnter()
{
	Coin* coin = static_cast<Coin*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(coin->GetScene())->GetHero();

	coin->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(COIN_ANIM_ACTIVE);
	coin->GetComponent<RigidBodyComponent>()->Freeze();
	
	Settings::GetInstance()->AddCoin(coin->GetValue());
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CoinPerish
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
CoinPerish* CoinPerish::Clone() const
{
	CoinPerish* clone = new CoinPerish();
	clone->_MakePrototype(false);

	CoinBehavior::Clone(clone);

	return clone;
}

void CoinPerish::OnEnter()
{
	m_parent->GetGameObject()->SetValid(false);
}

