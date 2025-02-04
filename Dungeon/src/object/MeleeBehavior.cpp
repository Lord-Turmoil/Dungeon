/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : MeleeBehavior.cpp                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 25, 2022                             *
 *                                                                            *
 *                    Last Update : December 14, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Melee behavior.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Math.h"

#include "../../inc/object/Bullet.h"
#include "../../inc/object/Component.h"
#include "../../inc/object/Figure.h"
#include "../../inc/object/MeleeBehavior.h"
#include "../../inc/object/Weapon.h"

#include "../../inc/game/Dungeon.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** MeleeBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void MeleeBehavior::Clone(MeleeBehavior* clone) const
{
    WeaponBehavior::Clone(clone);
}

/********************************************************************
** 07/25/2022 : TS
** Hmm... for now, melee weapon will hurt friendly units.
*/
void MeleeBehavior::_Slash()
{
    std::vector<GameObject*> candidates;
    Melee* melee = static_cast<Melee*>(m_parent->GetGameObject());
    Dungeon* dungeon = static_cast<Dungeon*>(melee->GetScene());
    QuadTree* tree = dungeon->GetQuadTree();
    Vector base = GetDirection(melee->GetCoord(), dungeon->GetMouse());
    Vector dir;

    double range = melee->GetRange();
    double radian = melee->GetRadian();
    double force = melee->GetForce();
    int damage = melee->GetDamage();
    double dist;
    double ratio;
    Figure* owner = static_cast<Figure*>(melee->GetSlot()->GetGameObject());
    Object* target;
    Figure* victim;
    Coordinate center = melee->GetCoord();

    tree->Query(melee, candidates);
    for (auto it = candidates.begin(); it != candidates.end(); it++)
    {
        if (*it == owner)
        {
            continue;
        }

        target = static_cast<Object*>(*it);
        if (target->Type() & OBJ_FIGURE)
        {
            victim = static_cast<Figure*>(target);
            dir = GetDisplacement(center, victim->GetCenter());
            dist = Module(dir);
            if ((dist > range) || (GetAngle(dir, base) > radian))
            {
                continue;
            }

            ratio = 2.0 - dist / range; // 1.0 ~ 2.0
            victim->Hurt(static_cast<int>(damage * ratio));
            if (dist < EPSILON)
            {
                victim->GetComponent<RigidBodyComponent>()->ApplyForce(
                    GetDirection(owner->GetCenter(), victim->GetCenter()) * force * ratio);
            }
            else
            {
                victim->GetComponent<RigidBodyComponent>()->ApplyForce(dir / dist * force * ratio);
            }
        }
    }

    if (melee->BulletName() != "")
    {
        if (owner->GetMP() >= melee->GetCost())
        {
            owner->CostMP(melee->GetCost());
            _Fire();
        }
    }
}

/*
** 2022/11/25 TS:
** Hmm... Slash bullet to be seperated.
** 2022/12/14 TS:
** Added check for unneccessary bounce.
*/
void MeleeBehavior::_SlashBullet()
{
    std::vector<GameObject*> candidates;
    Melee* melee = static_cast<Melee*>(m_parent->GetGameObject());
    Dungeon* dungeon = static_cast<Dungeon*>(melee->GetScene());
    QuadTree* tree = dungeon->GetQuadTree();
    Vector base = GetDirection(melee->GetCoord(), dungeon->GetMouse());
    Vector dir;

    double range = melee->GetRange();
    double radian = melee->GetRadian();
    // double force = melee->GetForce();
    // int damage = melee->GetDamage();
    double dist;
    Figure* owner = static_cast<Figure*>(melee->GetSlot()->GetGameObject());
    Object* target;
    Bullet* victim;
    Coordinate center = melee->GetCoord();

    tree->Query(melee, candidates);
    for (auto it = candidates.begin(); it != candidates.end(); it++)
    {
        if (*it == owner)
        {
            continue;
        }

        target = static_cast<Object*>(*it);
        if (target->Type() & OBJ_BULLET)
        {
            victim = static_cast<Bullet*>(target);
            dir = GetDisplacement(center, victim->GetCenter());
            dist = Module(dir);
            if ((dist > range) || (GetAngle(dir, base) > radian))
            {
                continue;
            }

            RigidBodyComponent* rigid = victim->GetComponent<RigidBodyComponent>();
            int id = rigid->ID();
            CollisionType type = GetCollisionType(id, COLL_ID_HERO);
            if (type == COLL_NONE)
            {
                continue;
            }
            else if (type == COLL_CHECK)
            {
                victim->Corrupt();
            }
            else
            {
                // Valid bounce. :)
                Vector v = rigid->GetVelocity();
                Vector u = VectorProjection(v, dir); // less
                rigid->SetVelocity(v - 2 * u);
                if (id == COLL_ID_ENEMY_BULLET)
                {
                    rigid->SetID(COLL_ID_HERO_BULLET);
                }
                else if (id == COLL_ID_ENEMY_FLAME)
                {
                    rigid->SetID(COLL_ID_HERO_FLAME);
                }
            }
        }
    }
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** MeleeReady
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
MeleeReady* MeleeReady::Clone() const
{
    MeleeReady* clone = new MeleeReady();
    clone->_MakePrototype(false);

    MeleeBehavior::Clone(clone);

    return clone;
}

void MeleeReady::Update(Event* evnt)
{
    Melee* melee = static_cast<Melee*>(m_parent->GetGameObject());

    melee->AdjustPosture();
    if (melee->IsTriggered())
    {
        m_parent->ChangeBehavior("Fire");
    }
}

void MeleeReady::OnEnter()
{
    m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim()->SetMotion(WEAPON_ANIM_READY);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** MeleeFire
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
MeleeFire* MeleeFire::Clone() const
{
    MeleeFire* clone = new MeleeFire();
    clone->_MakePrototype(false);

    MeleeBehavior::Clone(clone);

    return clone;
}

void MeleeFire::Update(Event* evnt)
{
    Melee* melee = static_cast<Melee*>(m_parent->GetGameObject());

    melee->AdjustPosture();
    if (melee->GetComponent<AnimComponent>()->GetAnim()->IsOver())
    {
        m_parent->ChangeBehavior("Cooling");
    }

    _SlashBullet();
}

void MeleeFire::OnEnter()
{
    Melee* melee = static_cast<Melee*>(m_parent->GetGameObject());

    melee->GetComponent<AnimComponent>()->GetAnim()->SetMotion(WEAPON_ANIM_FIRE);
    melee->GetComponent<SoundComponent>()->Play("fire");

    _Slash();
}
