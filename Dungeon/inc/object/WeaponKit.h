/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : WeaponKit.h                               *
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

#ifndef _WEAPON_KIT_H_
#define _WEAPON_KIT_H_

#include <dungine.h>

class Weapon;

class WeaponKit : public AbstractKit<Weapon>
{
public:
    WeaponKit()
    {
    }

    ~WeaponKit() override
    {
    }

    Weapon* LoadObject(XMLElement* node) override;
};

#endif
