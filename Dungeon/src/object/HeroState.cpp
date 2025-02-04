/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroState.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 26, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For hero's state.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/HeroState.h"
#include "../../inc/object/Bullet.h"
#include "../../inc/object/BulletLibrary.h"
#include "../../inc/object/Component.h"
#include "../../inc/object/Hero.h"

#include "../../inc/game/Dungeon.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HeroState
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void HeroState::Clone(HeroState* clone) const
{
    State::Clone(clone);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HeroNone
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HeroNone* HeroNone::Clone() const
{
    HeroNone* clone = new HeroNone();
    clone->_MakePrototype(false);

    HeroState::Clone(clone);

    return clone;
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HeroHurt
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HeroHurt* HeroHurt::Clone() const
{
    HeroHurt* clone = new HeroHurt();
    clone->_MakePrototype(false);

    HeroState::Clone(clone);

    return clone;
}

void HeroHurt::Update(Event* evnt)
{
    m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

    if (m_elapsedTime > m_duration)
    {
        m_parent->ChangeState("None");
    }
}

void HeroHurt::OnEnter()
{
    Hero* hero = static_cast<Hero*>(m_parent->GetGameObject());

    hero->GetSymbol()->GetAttribute()->alpha = 200;
    m_duration = hero->GetHurtDuration();
    m_elapsedTime = 0L;
}

void HeroHurt::OnExit()
{
    m_parent->GetGameObject()->GetSymbol()->GetAttribute()->alpha = 255;
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** YonySkill
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
YonySkill* YonySkill::Clone() const
{
    YonySkill* clone = new YonySkill();
    clone->_MakePrototype(false);

    HeroState::Clone(clone);

    return clone;
}

void YonySkill::Update(Event* evnt)
{
    m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

    if (m_elapsedTime > m_duration)
    {
        m_parent->ChangeState("None");
    }
}

void YonySkill::OnEnter()
{
    Hero* hero = static_cast<Hero*>(m_parent->GetGameObject());

    hero->GetComponent<SoundComponent>()->Play("skill");
    hero->HealMP(10);

    m_duration = hero->GetSkillDuration();
    m_elapsedTime = 0L;
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** SpecterSkill
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
const int SpecterSkill::m_DELTA_ALPHA = 5;

SpecterSkill* SpecterSkill::Clone() const
{
    SpecterSkill* clone = new SpecterSkill();
    clone->_MakePrototype(false);

    HeroState::Clone(clone);

    return clone;
}

void SpecterSkill::Update(Event* evnt)
{
    m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

    if (m_elapsedTime > m_duration)
    {
        m_parent->ChangeState("None");
    }
    else
    {
        m_curAlpha += m_delta;
        if (m_curAlpha < m_minAlpha)
        {
            m_curAlpha = m_minAlpha;
            m_delta = m_DELTA_ALPHA;
        }
        else if (m_curAlpha > m_maxAlpha)
        {
            m_curAlpha = m_maxAlpha;
            m_delta = -m_DELTA_ALPHA;
        }
        m_parent->GetGameObject()->GetSymbol()->GetAttribute()->alpha = m_curAlpha;
    }
}

void SpecterSkill::OnEnter()
{
    Figure* specter = static_cast<Figure*>(m_parent->GetGameObject());

    specter->SetInvincible(true);
    specter->GetComponent<RigidBodyComponent>()->SetID(COLL_ID_SPECTER);

    m_curAlpha = m_maxAlpha;
    m_delta = -m_DELTA_ALPHA;

    m_duration = specter->GetSkillDuration();
    m_elapsedTime = 0L;
}

void SpecterSkill::OnExit()
{
    Figure* specter = static_cast<Figure*>(m_parent->GetGameObject());

    specter->SetInvincible(false);
    specter->GetComponent<RigidBodyComponent>()->SetID(COLL_ID_HERO);
    specter->GetSymbol()->GetAttribute()->alpha = 255;
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BlackHandSkill
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
const int BlackHandSkill::m_FLAME_NUM = 50;
const int BlackHandSkill::m_FLAME_RADIUS = 20;
const double BlackHandSkill::m_DELTA = TWO_PI / static_cast<double>(m_FLAME_NUM);

BlackHandSkill* BlackHandSkill::Clone() const
{
    BlackHandSkill* clone = new BlackHandSkill();
    clone->_MakePrototype(false);

    HeroState::Clone(clone);

    return clone;
}

void BlackHandSkill::Update(Event* evnt)
{
    m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

    if (m_elapsedTime > m_duration)
    {
        m_parent->ChangeState("None");
    }
}

void BlackHandSkill::OnEnter()
{
    HBlackHand* hero = static_cast<HBlackHand*>(m_parent->GetGameObject());
    hero->GetComponent<SoundComponent>()->Play("skill");

    hero->SetInvincible(true);
    m_duration = hero->GetSkillDuration();

    m_elapsedTime = 0L;

    _ThrowFlame();
}

void BlackHandSkill::OnExit()
{
    static_cast<Figure*>(m_parent->GetGameObject())->SetInvincible(false);
}

void BlackHandSkill::_ThrowFlame()
{
    Bullet* protoFlame = BulletLibrary::GetInstance()->GetBulletByName("Flame");
    Bullet* flame;
    HBlackHand* hero = static_cast<HBlackHand*>(m_parent->GetGameObject());
    Dungeon* dungeon = static_cast<Dungeon*>(hero->GetScene());
    Coordinate center = hero->GetCoord();
    Coordinate start;
    Vector dir;

    double radian = 0.0;
    for (int i = 0; i < m_FLAME_NUM; i++)
    {
        dir = Rotate(VECTOR_NX, radian);
        start.x = center.x + static_cast<int>(dir.x * m_FLAME_RADIUS);
        start.y = center.y + static_cast<int>(dir.y * m_FLAME_RADIUS);

        flame = protoFlame->Clone();
        flame->SetCoord(start);
        flame->SetDirection(dir);
        flame->Activate();

        dungeon->AddObject(flame);

        radian += m_DELTA;
    }
}
