/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : WeaponLibrary.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Weapon library of the game.                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _WEAPON_LIBRARY_H_
#define _WEAPON_LIBRARY_H_

#include <dungine.h>

class Weapon;

class WeaponLibrary : private AbstractLibrary<Weapon>, public Singleton<WeaponLibrary>
{
	friend class Singleton<WeaponLibrary>;
public:
	bool Load(const char* filename);

	virtual bool Load(XMLElement* node);
	virtual void UnLoad();

	Weapon* AddWeapon(Weapon* weapon);
	Weapon* GetWeapon();	// Get a random weapon.
	Weapon* GetWeaponByName(const std::string& name);

private:
	WeaponLibrary() {}
	virtual ~WeaponLibrary();

	void _AcquireWeapons();

	// To avoid get same weapons repeatively.
	std::vector<Weapon*> m_unusedPool;
};

#endif