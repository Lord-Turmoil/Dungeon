/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Figure.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 13, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For the basic class of figures in game, including heros and enemies.     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <cstring>

#include "../../inc/object/Figure.h"
#include "../../inc/object/WeaponComponent.h"

/******************************************************************************
 * Figure::Clone -- Clone figure.                                             *
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
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Figure::Clone(Figure* clone) const
{
    Object::Clone(clone);

    memcpy(clone->m_hp, m_hp, sizeof(m_hp));
    memcpy(clone->m_mp, m_mp, sizeof(m_mp));
    memcpy(clone->m_armor, m_armor, sizeof(m_armor));
    memcpy(clone->m_chi, m_chi, sizeof(m_chi));

    memcpy(clone->m_hpGap, m_hpGap, sizeof(m_hpGap));
    memcpy(clone->m_mpGap, m_mpGap, sizeof(m_mpGap));
    memcpy(clone->m_armorGap, m_armorGap, sizeof(m_armorGap));
    memcpy(clone->m_chiGap, m_chiGap, sizeof(m_chiGap));

    clone->m_name = m_name;
    clone->m_hurtDuration = m_hurtDuration;
    clone->m_skillDuration = m_skillDuration;
    clone->m_skillCost = m_skillCost;
}

/******************************************************************************
 * Figure::Load -- Load figure's common properties.                           *
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
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
bool Figure::Load(XMLElement* node)
{
    /*
    **	<FigureName name="">	<- node
    **		<Property hurt-duration="" skill-duration="" skill-cost="">
    **			<HP>5, 1000</HP>
    **			<MP>5, 0</MP>
    **			<Armor>5, 500</Armor>
    **			<Chi>100, 100</Chi>
    **		</Property>
    **		...
    **	</FigureName>
    */
    Object::Load(node);

    const char* name = node->Name();
    const char* attr;

    _PARSE_ESSENTIAL("name", m_name, name, "");

    XMLElement* entry;
    Coordinate pair;
    node = GetElementByTagName(node, "Property");
    if (!node)
    {
        LOG_ERROR(INTERNAL_ERROR);
        return false;
    }

    // XP info
    entry = GetElementByTagName(node, "HP");
    if (entry)
    {
        ParsePrivateAttribute(&pair, entry->GetText(), ParseCoord);
    }
    else
    {
        pair = COORD_ZERO;
    }
    m_hp[1] = pair.x;
    m_hpGap[1] = pair.y;

    entry = GetElementByTagName(node, "MP");
    if (entry)
    {
        ParsePrivateAttribute(&pair, entry->GetText(), ParseCoord);
    }
    else
    {
        pair = COORD_ZERO;
    }
    m_mp[1] = pair.x;
    m_mpGap[1] = pair.y;

    entry = GetElementByTagName(node, "Armor");
    if (entry)
    {
        ParsePrivateAttribute(&pair, entry->GetText(), ParseCoord);
    }
    else
    {
        pair = COORD_ZERO;
    }
    m_armor[1] = pair.x;
    m_armorGap[1] = pair.y;

    entry = GetElementByTagName(node, "Chi");
    if (entry)
    {
        ParsePrivateAttribute(&pair, entry->GetText(), ParseCoord);
    }
    else
    {
        pair = COORD_ZERO;
    }
    m_chi[1] = pair.x;
    m_chiGap[1] = pair.y;

    Revitalize();

    // Other properties.
    _PARSE("hurt-duration", m_hurtDuration, name, 0L);
    _PARSE("skill-duration", m_skillDuration, name, 0L);
    _PARSE("skill-cost", m_skillCost, name, 0);

    _InitBehavior();
    _InitState();

    _RETURN_STATE();
}

/******************************************************************************
 * Figure::Initialize -- Change the figure to init behavior.                  *
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
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
void Figure::Initialize()
{
    GetComponent<BehaviorComponent>()->ChangeBehavior("Init");
}

/******************************************************************************
 * Figure::PickUpWeapon -- Pick up weapon.                                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   weapon                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/28 Tony : Created.                                               *
 *============================================================================*/
void Figure::PickUpWeapon(Weapon* weapon)
{
    GetComponent<WeaponComponent>()->PickUpWeapon(weapon);
}

/******************************************************************************
 * Figure::SwitchWeapon -- Switch weapon.                                     *
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
 *   2022/07/28 Tony : Created.                                               *
 *============================================================================*/
void Figure::SwitchWeapon()
{
    GetComponent<WeaponComponent>()->SwitchWeapon();
}

/******************************************************************************
 * Figure::TrigWeapon -- TrigWeapon.                                          *
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
 *   2022/07/28 Tony : Created.                                               *
 *============================================================================*/
void Figure::TrigWeapon()
{
    GetComponent<WeaponComponent>()->TrigWeapon();
}

void Figure::UnTrigWeapon()
{
    GetComponent<WeaponComponent>()->UnTrigWeapon();
}

/******************************************************************************
 * Figure::GetWeapon -- Get current weapon.                                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return the weapon.                                                *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/29 Tony : Created.                                               *
 *============================================================================*/
Weapon* Figure::GetWeapon()
{
    return GetComponent<WeaponComponent>()->GetCurrentWeapon();
}

/******************************************************************************
 * Figure::Cost/Heal -- Literally.                                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   val                                                               *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Figure::Hurt(int val)
{
    if (m_isInvincible)
    {
        return;
    }

    if (val > 0)
    {
        GetComponent<StateComponent>()->ChangeState("Hurt");

        // Extra damage will be ignored.
        if (m_armor[0] > 0)
        {
            m_armor[0] -= min(m_armor[0], val);
        }
        else
        {
            val -= m_armor[0];
            m_armor[0] = 0;
            m_hp[0] -= val;
            m_hp[0] = max(m_hp[0], 0);
        }
    }
}

void Figure::CostHP(int val)
{
    m_hp[0] -= min(m_hp[0], val);
    m_hpGap[0] = 0;
}

void Figure::CostMP(int val)
{
    m_mp[0] -= min(m_mp[0], val);
    m_mpGap[0] = 0;
}

void Figure::CostArmor(int val)
{
    m_armor[0] -= min(m_armor[0], val);
    m_armorGap[0] = 0;
}

void Figure::CostChi(int val)
{
    m_chi[0] -= min(m_chi[0], val);
    m_chiGap[0] = 0;
}

void Figure::HealHP(int val)
{
    if (!IsDead())
    {
        m_hp[0] += val;
        m_hp[0] = min(m_hp[0], m_hp[1]);
        m_hpGap[0] = 0;
    }
}

void Figure::HealMP(int val)
{
    m_mp[0] += val;
    m_mp[0] = min(m_mp[0], m_mp[1]);
    m_mpGap[0] = 0;
}

void Figure::HealArmor(int val)
{
    m_armor[0] += val;
    m_armor[0] = min(m_armor[0], m_armor[1]);
    m_armorGap[0] = 0;
}

void Figure::HealChi(int val)
{
    m_chi[0] += val;
    m_chi[0] = min(m_chi[0], m_chi[1]);
    m_chiGap[0] = 0;
}

/******************************************************************************
 * Figure::UpdateProperty -- Update all properties.                           *
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
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Figure::UpdateProperty()
{
    if (IsDead())
    {
        return;
    }

    clock_t delta = Timer::GetInstance()->GetDeltaTimestamp();

    if (m_hpGap[1] > 0)
    {
        m_hpGap[0] += delta;
        if (m_hpGap[0] > m_hpGap[1])
        {
            HealHP(1);
        }
    }

    if (m_mpGap[1] > 0)
    {
        m_mpGap[0] += delta;
        if (m_mpGap[0] > m_mpGap[1])
        {
            HealMP(1);
        }
    }

    if (m_armorGap[1] > 0)
    {
        m_armorGap[0] += delta;
        if (m_armorGap[0] > m_armorGap[1])
        {
            HealArmor(1);
        }
    }

    if (m_chiGap[1] > 0)
    {
        m_chiGap[0] += delta;
        if (m_chiGap[0] > m_chiGap[1])
        {
            HealChi(1);
        }
    }
}

/******************************************************************************
 * Figure::Revitalize -- Restore the figure's status.                         *
 *                                                                            *
 *    Cannot bring the dead to life.                                          *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Figure::Revitalize()
{
    m_hp[0] = m_hp[1];
    m_hpGap[0] = 0;
    m_mp[0] = m_mp[1];
    m_mpGap[0] = 0;
    m_armor[0] = m_armor[1];
    m_armorGap[0] = 0;
    m_chi[0] = m_chi[1];
    m_chiGap[0] = 0;
}

/******************************************************************************
 * Figure::Suicide -- Kill the figure instantly.                              *
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
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Figure::Suicide()
{
    m_hp[0] = 0;
}

/******************************************************************************
 * Figure::IsDead -- Check if the figure is dead or not.                      *
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
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
bool Figure::IsDead()
{
    return (m_hp[0] == 0);
}
