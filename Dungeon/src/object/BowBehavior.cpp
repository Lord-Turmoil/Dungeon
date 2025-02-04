/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BowBehavior.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 25, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Bow behavior.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/BowBehavior.h"
#include "../../inc/object/Component.h"
#include "../../inc/object/Figure.h"
#include "../../inc/object/Weapon.h"

void BowBehavior::Clone(BowBehavior* clone) const
{
    WeaponBehavior::Clone(clone);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BowReady
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BowReady* BowReady::Clone() const
{
    BowReady* clone = new BowReady();
    clone->_MakePrototype(false);

    BowBehavior::Clone(clone);

    return clone;
}

void BowReady::Update(Event* evnt)
{
    Bow* bow = static_cast<Bow*>(m_parent->GetGameObject());

    bow->AdjustPosture();
    if (bow->IsTriggered())
    {
        m_parent->ChangeBehavior("Charge");
    }
}

void BowReady::OnEnter()
{
    m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim()->SetMotion(WEAPON_ANIM_READY);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BowCharge
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BowCharge* BowCharge::Clone() const
{
    BowCharge* clone = new BowCharge();
    clone->_MakePrototype(false);

    BowBehavior::Clone(clone);

    return clone;
}

/********************************************************************
** Charge progress will be interrupted if mouse is released.
*/
void BowCharge::Update(Event* evnt)
{
    Bow* bow = static_cast<Bow*>(m_parent->GetGameObject());

    bow->AdjustPosture();
    bow->GetComponent<SoundComponent>()->Play("charge");
    if (!bow->IsTriggered())
    {
        bow->SetInterrupted(true);
        m_parent->ChangeBehavior("Discharge");
    }
    else
    {
        if (bow->GetComponent<AnimComponent>()->GetAnim()->IsOver())
        {
            m_parent->ChangeBehavior("Hold");
        }
    }
}

void BowCharge::OnEnter()
{
    Bow* bow = static_cast<Bow*>(m_parent->GetGameObject());
    Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim();

    if (bow->IsInterrupted())
    {
        int frameNum = anim->GetTotalFrameNum();
        int curFrame = anim->GetCurrentFrameID();
        anim->SetMotion(BOW_ANIM_CHARGE);
        anim->SetFrame(frameNum - curFrame - 1);
        bow->SetInterrupted(false);
    }
    else
    {
        anim->SetMotion(BOW_ANIM_CHARGE);
    }
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BowDischarge
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BowDischarge* BowDischarge::Clone() const
{
    BowDischarge* clone = new BowDischarge();
    clone->_MakePrototype(false);

    BowBehavior::Clone(clone);

    return clone;
}

void BowDischarge::Update(Event* evnt)
{
    Bow* bow = static_cast<Bow*>(m_parent->GetGameObject());

    bow->AdjustPosture();
    bow->GetComponent<SoundComponent>()->Play("charge");
    if (bow->IsTriggered())
    {
        bow->SetInterrupted(true);
        m_parent->ChangeBehavior("Charge");
    }
    else
    {
        if (bow->GetComponent<AnimComponent>()->GetAnim()->IsOver())
        {
            m_parent->ChangeBehavior("Ready");
        }
    }
}

void BowDischarge::OnEnter()
{
    Bow* bow = static_cast<Bow*>(m_parent->GetGameObject());
    Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim();

    if (bow->IsInterrupted())
    {
        int frameNum = anim->GetTotalFrameNum();
        int curFrame = anim->GetCurrentFrameID();
        anim->SetMotion(BOW_ANIM_DISCHARGE);
        anim->SetFrame(frameNum - curFrame - 1);
        bow->SetInterrupted(false);
    }
    else
    {
        anim->SetMotion(BOW_ANIM_DISCHARGE);
    }
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BowHold
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
BowHold* BowHold::Clone() const
{
    BowHold* clone = new BowHold();
    clone->_MakePrototype(false);

    BowBehavior::Clone(clone);

    return clone;
}

void BowHold::Update(Event* evnt)
{
    Bow* bow = static_cast<Bow*>(m_parent->GetGameObject());

    bow->AdjustPosture();
    bow->GetComponent<SoundComponent>()->Play("charge");
    if (!bow->IsTriggered())
    {
        Figure* figure = static_cast<Figure*>(bow->GetSlot()->GetGameObject());
        if (figure->GetMP() >= bow->GetCost())
        {
            figure->CostMP(bow->GetCost());
            m_parent->ChangeBehavior("Fire");
        }
        else
        {
            m_parent->ChangeBehavior("Discharge");
        }
    }
}

void BowHold::OnEnter()
{
    Bow* bow = static_cast<Bow*>(m_parent->GetGameObject());

    bow->GetComponent<AnimComponent>()->GetAnim()->SetMotion(BOW_ANIM_HOLD);

    bow->SetCharged(true);
}

void BowHold::OnExit()
{
    static_cast<Bow*>(m_parent->GetGameObject())->SetCharged(false);
}
