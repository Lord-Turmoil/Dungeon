/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Figure.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 24, 2022                             *
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

#ifndef _FIGURE_H_
#define _FIGURE_H_

#include <string>

#include "Object.h"

class Weapon;

class Figure : public Object
{
public:
    Figure(ObjectType type, Scene* scene)
        : Object(type, scene), m_hurtDuration(0L), m_skillDuration(0L), m_skillCost(0), m_isInvincible(false)
    {
        m_symbol.SetLayer(LAYER_FIGURE);
    }

    ~Figure() override
    {
    }

    const std::string& Name() const
    {
        return m_name;
    }

    Figure* Clone() const override = 0;
    virtual void Clone(Figure* clone) const;

    bool Load(XMLElement* node) override;

public:
    void Initialize();

public:
    // Some adapters for weapon component.
    void PickUpWeapon(Weapon* weapon);
    void SwitchWeapon();
    void TrigWeapon();
    void UnTrigWeapon();
    Weapon* GetWeapon();

public:
    Coordinate GetWeaponCenter() const
    {
        return m_weaponCenter;
    }

    void SetInvincible(bool isInvincible)
    {
        m_isInvincible = isInvincible;
    }

    bool IsInvincible() const
    {
        return m_isInvincible;
    }

public:
    void Hurt(int val);

    void CostHP(int val);
    void CostMP(int val);
    void CostArmor(int val);
    void CostChi(int val);

    void HealHP(int val);
    void HealMP(int val);
    void HealArmor(int val);
    void HealChi(int val);

    void UpdateProperty();
    void Revitalize();
    void Suicide();

    bool IsDead();

public:
    int GetHP() const
    {
        return m_hp[0];
    }

    int GetMaxHP() const
    {
        return m_hp[1];
    }

    int GetMP() const
    {
        return m_mp[0];
    }

    int GetMaxMP() const
    {
        return m_mp[1];
    }

    int GetArmor() const
    {
        return m_armor[0];
    }

    int GetMaxArmor() const
    {
        return m_armor[1];
    }

    int GetChi() const
    {
        return m_chi[0];
    }

    int GetMaxChi() const
    {
        return m_chi[1];
    }

    // Used in status bar.
    double GetHPPercent() const
    {
        return static_cast<double>(m_hp[0]) / static_cast<double>(m_hp[1]);
    }

    double GetMPPercent() const
    {
        return static_cast<double>(m_mp[0]) / static_cast<double>(m_mp[1]);
    }

    double GetArmorPercent() const
    {
        return static_cast<double>(m_armor[0]) / static_cast<double>(m_armor[1]);
    }

    double GetChiPercent() const
    {
        return static_cast<double>(m_chi[0]) / static_cast<double>(m_chi[1]);
    }

    clock_t GetHurtDuration() const
    {
        return m_hurtDuration;
    }

    clock_t GetSkillDuration() const
    {
        return m_skillDuration;
    }

    int GetSkillCost() const
    {
        return m_skillCost;
    }

protected:
    void _InitBehavior(XMLElement* node = nullptr) override = 0;
    virtual void _InitState() = 0;

protected:
    Coordinate m_weaponCenter;

protected:
    std::string m_name;

    /*
    ** Properties:
    ** [0] is the current hp(mp, ...),
    ** [1] is the max hp(mp, ...),
    ** xxGap[0] is the elapsed time since last regeneration.
    ** xxGap[1] is the regenerating gap, how many ms to regain 1 point,
    **      0 means no auto regain.
    */
    int m_hp[2];
    clock_t m_hpGap[2];
    int m_mp[2];
    clock_t m_mpGap[2];
    int m_armor[2];
    clock_t m_armorGap[2];
    int m_chi[2];
    clock_t m_chiGap[2];

    clock_t m_hurtDuration;
    clock_t m_skillDuration;

    int m_skillCost;

    // Hmm, an aux-tag to prevent damage.
    bool m_isInvincible;
};

#endif
