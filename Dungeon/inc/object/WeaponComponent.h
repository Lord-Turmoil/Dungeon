/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : WeaponComponent.h                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 27, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Weapon slot.                                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _WEAPON_COMPONENT_H_
#define _WEAPON_COMPONENT_H_

#include <vector>
#include <dungine.h>

class Weapon;

/********************************************************************
** Weapon will be removed from the object pool of the scene if picked
** up and put into weapon slot. So weapon slot must do the release.
*/
class WeaponComponent : public AbstractComponent
{
public:
	WeaponComponent(int updateOrder = 0);
	virtual ~WeaponComponent();

	static const char* StaticName() { return "WeaponSlot"; }
	virtual const char* Name() { return StaticName(); }

	int UpdateOrder() const { return m_updateOrder; }

	virtual WeaponComponent* Clone() const;
	virtual void Clone(WeaponComponent* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt);

public:
	virtual void Translate(const Coordinate& offset);

	// This is only valid after Update.
	Coordinate GetCoord() const { return m_center; }

	bool IsEmpty() const { return m_weapons.empty(); }
	bool IsFull() const { return m_weapons.size() == m_capacity; }
	bool IsArmed() const { return m_isArmed; }

	/*
	** Equip the figure with current weapon. Used on figure loaded.
	*/
	void Equip();
	void UnEquip();

	void PickUpWeapon(Weapon* weapon);
	void SwitchWeapon();
	void TrigWeapon();
	void UnTrigWeapon();

	Weapon* GetCurrentWeapon();

private:
	std::vector<Weapon*> m_weapons;
	std::vector<Weapon*>::iterator m_current;
	int m_capacity;

	Coordinate m_center;

	// Offset from figure's center to its center.
	Coordinate m_offset;

	// Target of the weapon. Set by figure.
	Coordinate m_target;

	bool m_isArmed;
};


#endif