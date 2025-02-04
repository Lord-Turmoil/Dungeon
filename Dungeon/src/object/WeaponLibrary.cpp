/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : WeaponLibrary.cpp                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 21, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Weapon library of the game.                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/WeaponLibrary.h"

#include "../../inc/object/Weapon.h"
#include "../../inc/object/WeaponKit.h"

#include <algorithm>

/******************************************************************************
 * WeaponLibrary::Load -- Load weapon library.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   filename                                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
bool WeaponLibrary::Load(const char* filename)
{
    XMLFile file;

    file.Load(filename);

    Load(file.GetRoot());

    file.UnLoad();

    _RETURN_STATE();
}

/******************************************************************************
 * WeaponLibrary::Load -- Load weapon prototypes.                             *
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
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
bool WeaponLibrary::Load(XMLElement* node)
{
    /*
    **	<WeaponLibrary>
    **		<WeaponName ...>...</WeaponName>
    **		<WeaponName ...>...</WeaponName>
    **		<WeaponName ...>...</WeaponName>
    **	</WeaponLibrary>
    */
    const char* name = node->Name();

    _CHECK_TAG("WeaponLibrary");
    _RETURN_IF_ERROR();

    WeaponKit kit;
    XMLElement* elem = node->FirstChildElement();
    while (elem)
    {
        AddWeapon(kit.LoadObject(elem));
        elem = elem->NextSiblingElement();
    }
    _AcquireWeapons();

    _RETURN_STATE();
}

/******************************************************************************
 * WeaponLibrary::UnLoad -- Unload all resources.                             *
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
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
void WeaponLibrary::UnLoad()
{
    for (auto it = m_pool.begin(); it != m_pool.end(); it++)
    {
        delete it->second;
    }
    m_pool.clear();
    m_unusedPool.clear();
}

/******************************************************************************
 * WeaponLibrary::AddWeapon -- Add weapon to the library.                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   weapon                                                            *
 *                                                                            *
 * OUTPUT:  Return the added weapon.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
Weapon* WeaponLibrary::AddWeapon(Weapon* weapon)
{
    return AddItem(weapon->Name(), weapon);
}

/******************************************************************************
 * WeaponLibrary::GetWeapon -- Get a random weapon.                           *
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
 *   2022/07/21 Tony : Created.                                               *
 *   2022/08/17 Tony : Weapon will not appear twice.                          *
 *============================================================================*/
Weapon* WeaponLibrary::GetWeapon()
{
    int size = static_cast<int>(m_unusedPool.size());
    Weapon* weapon;

    if (size == 0)
    {
        _AcquireWeapons();
    }
    weapon = m_unusedPool.back();
    m_unusedPool.pop_back();

    return weapon;
}

/******************************************************************************
 * WeaponLibrary::GetWeaponByName -- Get a weapon by its name.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/21 Tony : Created.                                               *
 *   2022/08/17 Tony : Added unusedPool.                                      *
 *============================================================================*/
Weapon* WeaponLibrary::GetWeaponByName(const std::string& name)
{
    return GetItem(name);
}

/******************************************************************************
 * WeaponLibrary::~WeaponLibrary -- Destructor of the object.                 *
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
 *   2022/08/23 Tony : Created.                                               *
 *============================================================================*/
WeaponLibrary::~WeaponLibrary()
{
    UnLoad();
}

/******************************************************************************
 * WeaponLibrary::_AcquireWeapons -- Acquire all non-exclusive weapons.       *
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
 *   2022/08/19 Tony : Created.                                               *
 *============================================================================*/
void WeaponLibrary::_AcquireWeapons()
{
    m_unusedPool.clear();
    for (auto it = m_pool.begin(); it != m_pool.end(); it++)
    {
        if (!it->second->IsExclusive())
        {
            m_unusedPool.push_back(it->second);
        }
    }
    std::random_shuffle(m_unusedPool.begin(), m_unusedPool.end());
}
