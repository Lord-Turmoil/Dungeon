/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : WeaponKit.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 22, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Factory of weapons.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/WeaponKit.h"
#include "../../inc/object/Weapon.h"

/******************************************************************************
 * WeaponKit::LoadObject -- Load weapon.                                      *
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
 *   2022/07/22 Tony : Created.                                               *
 *============================================================================*/
Weapon* WeaponKit::LoadObject(XMLElement* node)
{
    const char* name = node->Name();
    Weapon* weapon = nullptr;

    if (_STR_SAME(name, "Gun"))
    {
        weapon = new Gun(nullptr);
    }
    else if (_STR_SAME(name, "Bow"))
    {
        weapon = new Bow(nullptr);
    }
    else if (_STR_SAME(name, "Melee"))
    {
        weapon = new Melee(nullptr);
    }

    if (weapon)
    {
        weapon->Load(node);
    }
    else
    {
        LOG_ERROR(UNKNOWN_TAG, name);
    }

    return weapon;
}
