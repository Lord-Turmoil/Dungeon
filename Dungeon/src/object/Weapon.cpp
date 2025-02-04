/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Weapon.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 18, 2022                             *
 *                                                                            *
 *                    Last Update : November 25, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Weapon system of the game.                                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Math.h"

#include "../../inc/object/BowBehavior.h"
#include "../../inc/object/Figure.h"
#include "../../inc/object/GunBehavior.h"
#include "../../inc/object/MeleeBehavior.h"
#include "../../inc/object/Weapon.h"
#include "../../inc/object/WeaponBehavior.h"
#include "../../inc/object/WeaponComponent.h"

/******************************************************************************
 * Weapon::Clone -- Clone weapon.                                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   clone                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/19 Tony : Created.                                               *
 *============================================================================*/
void Weapon::Clone(Weapon* clone) const
{
    Object::Clone(clone);
    clone->m_name = m_name;
    clone->m_bulletName = m_bulletName;

    // 2022/08/02: TS
    // Don't clone parent... :(
    // clone->m_pSlot = m_pSlot;

    clone->m_offset = m_offset;
    clone->m_muzzleOffset = m_muzzleOffset;
    clone->m_coolingTime = m_coolingTime;
    clone->m_cost = m_cost;
    clone->m_burstNum = m_burstNum;
    clone->m_accuracy = m_accuracy;
    clone->m_radius = m_radius;
    clone->m_isEquipped = m_isEquipped;
    clone->m_isTriggered = m_isTriggered;
    clone->m_isExclusive = m_isExclusive;
}

/******************************************************************************
 * Weapon::Load -- Load Weapon.                                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/19 Tony : Created.                                               *
 *============================================================================*/
bool Weapon::Load(XMLElement* node)
{
    /*
    **	<WeaponName name="" bullet="" offset="" muzzle="" cooling="" cost="">
    **		<Components>...</Components>
    **	</WeaponName>
    */
    const char* name = node->Name();
    const char* attr;

    Object::Load(node);

    _PARSE_ESSENTIAL("name", m_name, name, "");
    _PARSE("bullet", m_bulletName, name, ""); // Melee may not use this.
    _PARSE_ESSENTIAL("radius", m_radius, name, 0.0);
    _PARSE_PRIVATE("offset", m_offset, name, ParseCoord);
    _PARSE_PRIVATE("muzzle", m_muzzleOffset, name, ParseCoord);
    _PARSE("cooling", m_coolingTime, name, 0L);
    _PARSE("cost", m_cost, name, 0);
    _PARSE("burst", m_burstNum, name, 1);
    _PARSE("accuracy", m_accuracy, name, 1.0);
    _PARSE("exclusive", m_isExclusive, name, false);

    _InitBehavior();

    _RETURN_STATE();
}

/******************************************************************************
 * Weapon::Equip -- Equip weapon.                                             *
 *                                                                            *
 *    Here, the slot is already known.                                        *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/25 Tony : Created.                                               *
 *============================================================================*/
void Weapon::Equip()
{
    if (m_isEquipped)
    {
        return;
    }

    m_isEquipped = true;

    GetComponent<BehaviorComponent>()->ChangeBehavior("Ready");
}

/******************************************************************************
 * Weapon::UnEquip -- Unequip the weapon.                                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/25 Tony : Created.                                               *
 *============================================================================*/
void Weapon::UnEquip()
{
    if (!m_isEquipped)
    {
        return;
    }

    m_isEquipped = false;

    GetComponent<BehaviorComponent>()->ChangeBehavior("Unequipped");
}

/******************************************************************************
 * Weapon::Drop -- Drop weapon. Return to unequipped behavior.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/01 Tony : Created.                                               *
 *============================================================================*/
void Weapon::Drop()
{
    GetComponent<BehaviorComponent>()->ChangeBehavior("Idle");
}

/******************************************************************************
 * Weapon::AdjustPosture -- Adjust weapon's posture.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   target                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/29 Tony : Created.                                               *
 *============================================================================*/
void Weapon::AdjustPosture()
{
    if (!m_target.IsZero())
    {
        AdjustPosture(m_target);
    }
}

void Weapon::AdjustPosture(const Coordinate& target)
{
    Animation* anim = GetComponent<AnimComponent>()->GetAnim();
    Coordinate center = m_pSlot->GetCoord() + GetOffset();

    SetCoord(center);
    m_symbol.SetCoord(center);

    if (target.x < GetCoord().x)
    {
        anim->SetDir(ANIM_LEFT);
    }
    else
    {
        anim->SetDir(ANIM_RIGHT);
    }

    GetSymbol()->GetAttribute()->rotationAngle = GetRotationRadian(target - GetCoord(), -GetMuzzleOffset().y, true);
}

void Weapon::SetTarget(Direction dir)
{
    m_target = GetCoord();
    m_target.x += (dir == DIR_LEFT) ? (-10) : (10);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Gun
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Gun* Gun::Clone() const
{
    Gun* clone = new Gun(m_pScene);
    clone->_MakePrototype(false);

    Weapon::Clone(clone);

    return clone;
}

void Gun::Clone(Gun* clone) const
{
    Weapon::Clone(clone);
}

bool Gun::Load(XMLElement* node)
{
    /*
    **	<Gun name="" offset="" cooling="" muzzle="">
    **		...
    **	</Gun>
    */
    const char* name = node->Name();

    Weapon::Load(node);

    _RETURN_STATE();
}

void Gun::_InitBehavior(XMLElement* node)
{
    auto parent = GetComponent<BehaviorComponent>();

    parent->AddBehavior(new WeaponIdle());
    parent->AddBehavior(new WeaponUnequipped());
    parent->AddBehavior(new GunReady());
    parent->AddBehavior(new WeaponFire());
    parent->AddBehavior(new WeaponCooling());

    parent->ChangeBehavior("Idle");
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Bow
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Bow* Bow::Clone() const
{
    Bow* clone = new Bow(m_pScene);
    clone->_MakePrototype(false);

    Weapon::Clone(clone);

    clone->m_isInterrupted = m_isInterrupted;
    clone->m_isCharged = m_isCharged;

    return clone;
}

void Bow::Clone(Bow* clone) const
{
    Weapon::Clone(clone);

    clone->m_isInterrupted = m_isInterrupted;
    clone->m_isCharged = m_isCharged;
}

bool Bow::Load(XMLElement* node)
{
    /*
    **	<Bow ...>
    **	</Bow>
    */
    const char* name = node->Name();

    Weapon::Load(node);

    _RETURN_STATE();
}

void Bow::_InitBehavior(XMLElement* node)
{
    auto parent = GetComponent<BehaviorComponent>();

    parent->AddBehavior(new WeaponIdle());
    parent->AddBehavior(new WeaponUnequipped());
    parent->AddBehavior(new BowReady());
    parent->AddBehavior(new BowCharge());
    parent->AddBehavior(new BowHold());
    parent->AddBehavior(new BowDischarge());
    parent->AddBehavior(new WeaponFire());
    parent->AddBehavior(new WeaponCooling());

    parent->ChangeBehavior("Idle");
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Melee
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Melee* Melee::Clone() const
{
    Melee* clone = new Melee(m_pScene);
    clone->_MakePrototype(false);

    Weapon::Clone(clone);

    clone->m_radian = m_radian;
    clone->m_range = m_range;
    clone->m_force = m_force;
    clone->m_damage = m_damage;

    return clone;
}

void Melee::Clone(Melee* clone) const
{
    Weapon::Clone(clone);

    clone->m_radian = m_radian;
    clone->m_range = m_range;
}

bool Melee::Load(XMLElement* node)
{
    /*
    **	<Melee ... damage="" force="" radian="" range="">
    **		<Components>...</Components>
    **	</Melee>
    */
    const char* name = node->Name();
    const char* attr;

    Weapon::Load(node);

    _PARSE("force", m_force, name, 0.0);
    _PARSE("damage", m_damage, name, 0);
    _PARSE("radian", m_radian, name, 0.0);
    _PARSE("range", m_range, name, 0.0);

    /*
    ** 2022/11/25 TS:
    ** radian read in as degree for convenience.
    */
    m_radian *= DEG_TO_RAD;

    _RETURN_STATE();
}

void Melee::_InitBehavior(XMLElement* node)
{
    auto parent = GetComponent<BehaviorComponent>();

    parent->AddBehavior(new WeaponIdle());
    parent->AddBehavior(new WeaponUnequipped());
    parent->AddBehavior(new MeleeReady());
    parent->AddBehavior(new MeleeFire());
    parent->AddBehavior(new WeaponCooling());

    parent->ChangeBehavior("Idle");
}
