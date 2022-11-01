/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Weapon.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 18, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Weapon system of the game.                                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <string>

#include "Object.h"

class WeaponComponent;

/********************************************************************
** All weapons are divided to three categories, Gun, Bow and Melee.
** Every weapon has its unique name, but only as a reference. Their
** different behavior is achieved by different configurations. If
** the weapon is really special, then a specific class is derived.
** Weapon's coordinate is the rotation center, which is also its
** center.
*/
class Figure;

class Weapon : public Object
{
public:
	Weapon(Scene* scene) : Object(ObjectType::OBJ_WEAPON, scene),
		m_pSlot(nullptr), m_isEquipped(false), m_isTriggered(false),
		m_isExclusive(false)
	{
		m_symbol.SetLayer(LAYER_WEAPON);
	}
	virtual ~Weapon() {}

	const std::string& Name() const { return m_name; }

	virtual Weapon* Clone() const = 0;
	virtual void Clone(Weapon* clone) const;

	virtual bool Load(XMLElement* node);

public:
	/*
	** Equip the weapon when picked up.
	*/
	void Equip();
	void UnEquip();
	void Drop();

	/*
	** Adjust coordinate and rotation angle.
	*/
	void AdjustPosture();
	void AdjustPosture(const Coordinate& target);

public:
	const std::string& BulletName() const { return m_bulletName; }

	/*
	** For both player and enemy can fire the right way.
	*/
	void Trig() { m_isTriggered = true; }
	void UnTrig() { m_isTriggered = false; }
	bool IsTriggered() const { return m_isTriggered; }

	void SetTarget(const Coordinate& target) { m_target = target; }
	void SetTarget(Direction dir);
	Coordinate GetTarget() const { return m_target; }

	void SetSlot(WeaponComponent* pSlot) { m_pSlot = pSlot; }
	WeaponComponent* GetSlot() { return m_pSlot; }

	clock_t GetCoolingTime() const { return m_coolingTime; }
	Coordinate GetOffset() const { return m_offset; }
	Coordinate GetMuzzleOffset() const { return m_muzzleOffset; }
	
	int GetCost() const { return m_cost; }
	int GetBurstNum() const { return m_burstNum; }
	double GetAccuracy() const { return m_accuracy; }
	double GetRadius() const { return m_radius; }

	bool IsExclusive() const { return m_isExclusive; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr) {}

	std::string m_name;
	std::string m_bulletName;

	WeaponComponent* m_pSlot;

	// offset from standard rotation center to its custom rotation center
	Coordinate m_offset;
	Coordinate m_muzzleOffset;

	Coordinate m_target;

	clock_t m_coolingTime;

	int m_cost;	// cost of mp
	
	int m_burstNum;		// how many bullets at a time
	double m_accuracy;
	double m_radius;	// pickup radius

	bool m_isEquipped;
	bool m_isTriggered;

	/*
	** If is exclusive, then it won't appear in the crate.
	*/
	bool m_isExclusive;
};


/********************************************************************
** Gun is the standard weapon, just like the guns in real life.
*/
class Gun : public Weapon
{
public:
	Gun(Scene* scene) : Weapon(scene) {}
	virtual ~Gun() {}

	virtual Gun* Clone() const;
	virtual void Clone(Gun* clone) const;

	virtual bool Load(XMLElement* node);

public:
	Coordinate MuzzleOffset() const { return m_muzzleOffset; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);
};

class Bow : public Weapon
{
public:
	Bow(Scene* scene) : Weapon(scene),
		m_isInterrupted(false), m_isCharged(false) {}
	virtual ~Bow() {}

	virtual Bow* Clone() const;
	virtual void Clone(Bow* clone) const;

	virtual bool Load(XMLElement* node);

public:
	Coordinate MuzzleOffset() const { return m_muzzleOffset; }

	void SetInterrupted(bool isInterrupted) { m_isInterrupted = isInterrupted; }
	bool IsInterrupted() const { return m_isInterrupted; }

	void SetCharged(bool isCharged) { m_isCharged = isCharged; }
	bool IsCharged() const { return m_isCharged; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);

	/*
	** If charge or discharge progress is interrupted, this will be
	** set to true;
	*/
	bool m_isInterrupted;
	bool m_isCharged;
};

class Melee : public Weapon
{
public:
	Melee(Scene* scene) : Weapon(scene) {}
	virtual ~Melee() {}

	virtual Melee* Clone() const;
	virtual void Clone(Melee* clone) const;

	virtual bool Load(XMLElement* node);

public:
	double GetRadian() const { return m_radian; }
	double GetRange() const { return m_range; }
	double GetForce() const { return m_force; }
	int GetDamage() const { return m_damage; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);

	/*
	** The attack range of melee is a fan area?
	** radian is half of the range.
	*/
	double m_radian;
	double m_range;

	/*
	** Melee weapon doesn't have bullet.
	*/
	double m_force;
	int m_damage;
};

#endif