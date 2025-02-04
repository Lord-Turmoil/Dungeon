/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnergyBehavior.cpp                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Energy behavior.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/EnergyBehavior.h"
#include "../../inc/object/Buff.h"
#include "../../inc/object/Hero.h"

#include "../../inc/game/Dungeon.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnergyBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void EnergyBehavior::Clone(EnergyBehavior* clone) const
{
    Behavior::Clone(clone);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnergyFly
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnergyFly* EnergyFly::Clone() const
{
    EnergyFly* clone = new EnergyFly();
    clone->_MakePrototype(false);

    EnergyBehavior::Clone(clone);

    return clone;
}

void EnergyFly::Update(Event* evnt)
{
    Energy* energy = static_cast<Energy*>(m_parent->GetGameObject());
    RigidBodyComponent* rigid = energy->GetComponent<RigidBodyComponent>();
    Coordinate center = energy->GetCoord();
    Coordinate target = static_cast<Dungeon*>(energy->GetScene())->GetHero()->GetCenter();

    double dist = Distance(center, target);
    if (dist < energy->GetRadius())
    {
        m_parent->ChangeBehavior("Active");
        return;
    }

    rigid->ApplyForce(GetDirection(center, target) * rigid->GetStrength());
}

void EnergyFly::OnEnter()
{
    m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim()->SetMotion(ENERGY_ANIM_FLY);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnergyActive
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnergyActive* EnergyActive::Clone() const
{
    EnergyActive* clone = new EnergyActive();
    clone->_MakePrototype(false);

    EnergyBehavior::Clone(clone);

    return clone;
}

void EnergyActive::Update(Event* evnt)
{
    if (m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim()->IsOver())
    {
        m_parent->ChangeBehavior("Perish");
    }
}

void EnergyActive::OnEnter()
{
    Energy* energy = static_cast<Energy*>(m_parent->GetGameObject());
    Hero* hero = static_cast<Dungeon*>(energy->GetScene())->GetHero();

    energy->GetComponent<AnimComponent>()->GetAnim()->SetMotion(ENERGY_ANIM_ACTIVE);
    energy->GetComponent<RigidBodyComponent>()->Freeze();

    hero->HealMP(energy->GetValue());
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** EnergyPerish
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
EnergyPerish* EnergyPerish::Clone() const
{
    EnergyPerish* clone = new EnergyPerish();
    clone->_MakePrototype(false);

    EnergyBehavior::Clone(clone);

    return clone;
}

void EnergyPerish::OnEnter()
{
    m_parent->GetGameObject()->SetValid(false);
}
