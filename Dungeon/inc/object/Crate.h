/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Crate.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 15, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Crate contains some buff or weapon.                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _CRATE_H_
#define _CRATE_H_

#include <vector>

#include "Object.h"

class Weapon;
class Buff;

/********************************************************************
** A crate can contain one weapon, or one drug, or several coins and
** "energies". :P
*/
class Crate final : public Object
{
public:
	Crate(Scene* scene) : Object(ObjectType::OBJ_CRATE),
		m_weapon(nullptr), m_radius(0.0), m_isOpen(false)
	{
		m_symbol.SetLayer(LAYER_FIGURE);
	}
	virtual ~Crate();

	virtual Crate* Clone() const;
	virtual void Clone(Crate* clone) const {}

	virtual bool Load(XMLElement* node);

public:
	void Generate();	// Generate contents.
	void Open();		// Open the crate (When activated).

	double GetRadius() const { return m_radius; }
	bool IsOpen() const { return m_isOpen; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);

private:
	void _GenerateDrug(int hpPercent);
	bool _GenerateWeapon();
	void _GenerateEnergyAndCoin();

	std::vector<Buff*> m_buffs;
	Weapon* m_weapon;

	double m_radius;

	bool m_isOpen;
};

#endif