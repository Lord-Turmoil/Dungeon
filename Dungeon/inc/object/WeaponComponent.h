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
 *                    Last Update : December 9, 2022                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Weapon slot.                                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _WEAPON_COMPONENT_H_
#define _WEAPON_COMPONENT_H_

#include <dungine.h>
#include <vector>

class Weapon;

/********************************************************************
** Weapon will be removed from the object pool of the scene if picked
** up and put into weapon slot. So weapon slot must do the release.
*/
class WeaponComponent : public AbstractComponent
{
public:
    WeaponComponent(int updateOrder = 0);
    ~WeaponComponent() override;

    static const char* StaticName()
    {
        return "WeaponSlot";
    }

    const char* Name() override
    {
        return StaticName();
    }

    int UpdateOrder() const
    {
        return m_updateOrder;
    }

    WeaponComponent* Clone() const override;

    virtual void Clone(WeaponComponent* clone) const
    {
    }

    bool Load(XMLElement* node) override;

    void Update(Event* evnt) override;

public:
    void Translate(const Coordinate& offset) override;

    // This is only valid after Update.
    Coordinate GetCoord() const
    {
        return m_center;
    }

    bool IsEmpty() const
    {
        return m_weapons.empty();
    }

    bool IsFull() const
    {
        return m_weapons.size() == m_capacity;
    }

    bool IsArmed() const
    {
        return m_isArmed;
    }

    // 2022/12/09 TS: Get the weapon names for flashback.
    std::vector<std::string> GetWeaponList() const;

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

    /*
    ** 2022/12/08 TS:
    ** Clear the weapon slot.
    */
    void Clear();

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
