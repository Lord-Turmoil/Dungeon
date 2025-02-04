/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : RockBehavior.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 30, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Behavior of Rock.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Boss.h"
#include "../../inc/object/BossBehavior.h"
#include "../../inc/object/Component.h"
#include "../../inc/object/Enemy.h"
#include "../../inc/object/Hero.h"
#include "../../inc/object/Library.h"
#include "../../inc/object/Weapon.h"

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Settings.h"

#include "../../inc/terrain/Terrain.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** RockIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
RockIdle* RockIdle::Clone() const
{
    RockIdle* clone = new RockIdle();
    clone->_MakePrototype(false);

    EnemyIdle::Clone(clone);

    return clone;
}

void RockIdle::Update(Event* evnt)
{
    ERock* rock = static_cast<ERock*>(m_parent->GetGameObject());
    Dungeon* dungeon = static_cast<Dungeon*>(rock->GetScene());
    double dist = Distance(rock->GetCoord(), dungeon->GetHero()->GetCoord());

    rock->UpdateProperty();

    if (dist < rock->GetScareRadius())
    {
        m_parent->ChangeBehavior("Retreat");
        return;
    }
    else if (dist < rock->GetAlertRadius())
    {
        m_parent->ChangeBehavior("Engage");
        return;
    }

    m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
    if (m_elapsedTime > DECISION_GAP)
    {
        if (Random(3) == 0)
        {
            rock->ChangeFacing();
        }
        m_elapsedTime = 0L;
    }

    // Keep alert.
    _Alert();
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** RockEngage
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
RockEngage* RockEngage::Clone() const
{
    RockEngage* clone = new RockEngage();
    clone->_MakePrototype(false);

    GenieEngage::Clone(clone);

    return clone;
}

void RockEngage::Update(Event* evnt)
{
    ERock* rock = static_cast<ERock*>(m_parent->GetGameObject());
    Dungeon* dungeon = static_cast<Dungeon*>(rock->GetScene());
    Coordinate center = rock->GetCoord();
    Coordinate hero = dungeon->GetHero()->GetCoord();
    Coordinate target = dungeon->GetArena()->Engage(center, hero);

    if (rock->IsDead())
    {
        m_parent->ChangeBehavior("Dead");
        return;
    }

    rock->UpdateProperty();
    _Move((target == center) ? VECTOR_ZERO : GetDirection(center, target));
    _Evade(Settings::GetInstance()->Difficulty());
    _Collide();

    rock->SetFacing((hero.x < center.x) ? DIR_LEFT : DIR_RIGHT);

    double dist = Distance(center, hero);
    m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
    if (m_elapsedTime > DECISION_GAP)
    {
        if (dist > rock->GetAlertRadius())
        {
            m_parent->ChangeBehavior("Idle");
            return;
        }
        if (dist < rock->GetScareRadius())
        {
            m_parent->ChangeBehavior("Retreat");
            return;
        }
        else if (dist < rock->GetAttackRadius())
        {
            m_parent->ChangeBehavior("Attack");
            return;
        }
        m_elapsedTime = 0;
    }

    double percent = rock->GetHPPercent();
    int mp = rock->GetMP();
    if ((mp == 2) && (percent < 0.25))
    {
        rock->HealMP(1);
        _OnDefeat(dungeon, rock);
    }
    else if ((mp == 1) && (percent < 0.5))
    {
        rock->HealMP(1);
        _OnDefeat(dungeon, rock);
    }
    else if ((mp == 0) && (percent < 0.75))
    {
        rock->HealMP(1);
        _OnDefeat(dungeon, rock);
    }
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** RockRetreat
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
RockRetreat* RockRetreat::Clone() const
{
    RockRetreat* clone = new RockRetreat();
    clone->_MakePrototype(false);

    GenieRetreat::Clone(clone);

    return clone;
}

void RockRetreat::Update(Event* evnt)
{
    ERock* rock = static_cast<ERock*>(m_parent->GetGameObject());
    Dungeon* dungeon = static_cast<Dungeon*>(rock->GetScene());
    Coordinate center = rock->GetCoord();
    Coordinate hero = dungeon->GetHero()->GetCoord();
    Arena* arena = dungeon->GetArena();
    Coordinate target = arena->Retreat(center, hero);

    if (rock->IsDead())
    {
        m_parent->ChangeBehavior("Dead");
        return;
    }

    rock->UpdateProperty();
    _Move((target == center) ? VECTOR_ZERO : GetDirection(center, target));
    _Evade(Settings::GetInstance()->Difficulty());
    _Collide();

    rock->SetFacing((hero.x < center.x) ? DIR_LEFT : DIR_RIGHT);

    double dist = Distance(center, hero);
    m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
    if (m_elapsedTime > DECISION_GAP)
    {
        if (dist > rock->GetAlertRadius())
        {
            m_parent->ChangeBehavior("Idle");
            return;
        }
        else if (dist < rock->GetAttackRadius())
        {
            m_parent->ChangeBehavior("Attack");
            return;
        }
        m_elapsedTime = 0;
    }
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** RockAttack
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
RockAttack* RockAttack::Clone() const
{
    RockAttack* clone = new RockAttack();
    clone->_MakePrototype(false);

    EnemyAttack::Clone(clone);

    return clone;
}

void RockAttack::OnEnter()
{
    ERock* rock = static_cast<ERock*>(m_parent->GetGameObject());
    Hero* hero = static_cast<Dungeon*>(rock->GetScene())->GetHero();

    // RigidBodyComponent* heroRigid = hero->GetComponent<RigidBodyComponent>();
    Vector dir = GetDirection(rock->GetCoord(), hero->GetCoord());

    rock->UpdateProperty();
    rock->GetComponent<AnimComponent>()->GetAnim()->SetMotion(ENEMY_ANIM_ATTACK);

    if (dir.x < 0)
    {
        rock->SetFacing(DIR_LEFT);
    }
    else
    {
        rock->SetFacing(DIR_RIGHT);
    }

    _GenerateStone();
}

void RockAttack::_GenerateStone()
{
    ERock* rock = static_cast<ERock*>(m_parent->GetGameObject());
    Dungeon* dungeon = static_cast<Dungeon*>(rock->GetScene());

    /*
    ** 2022/08/26 TS:
    ** Not too much.
    */
    if (dungeon->GetEnemyCount() > 5)
    {
        return;
    }

    EnemyLibrary* lib = EnemyLibrary::GetInstance();
    Enemy* enemy;
    Coordinate center = rock->GetCoord();

    int num = 1 + Settings::GetInstance()->Difficulty();
    int level = dmin(1 + Settings::GetInstance()->Difficulty(), 3);
    auto& pool = lib->GetEnemyByLevel(level);
    for (int i = 0; i < num; i++)
    {
        enemy = Random(pool)->Clone();
        dungeon->AddEnemy(enemy, center);
    }
}
