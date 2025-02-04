/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Buff.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 13, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Buff is some accessaries to the game.                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENERGY_H_
#define _ENERGY_H_

#include "Object.h"

/********************************************************************
** Buff includes energy(restore mp), hp/mp-drug and coin.
*/
class Buff : public Object
{
public:
    Buff(Scene* scene) : Object(OBJ_BUFF, scene), m_value(0)
    {
        m_symbol.SetLayer(LAYER_BUFF);
    }

    ~Buff() override
    {
    }

    virtual const char* Name() const = 0;

    Buff* Clone() const override = 0;
    virtual void Clone(Buff* clone) const;

    bool Load(XMLElement* node) override;

public:
    int GetValue() const
    {
        return m_value;
    }

protected:
    void _InitBehavior(XMLElement* node = nullptr) override
    {
    }

private:
    int m_value;
};

/********************************************************************
** Energy flies towards the hero and restore mp.
*/
class Energy final : public Buff
{
public:
    Energy(Scene* scene) : Buff(scene), m_radius(0.0)
    {
    }

    ~Energy() override
    {
    }

    const char* Name() const override
    {
        return "Energy";
    }

    Energy* Clone() const override;

    virtual void Clone(Energy* clone) const
    {
    }

    bool Load(XMLElement* node) override;

    void Activate();

public:
    double GetRadius() const
    {
        return m_radius;
    }

protected:
    void _InitBehavior(XMLElement* node = nullptr) override;

private:
    double m_radius;
};

/********************************************************************
** Coin is almost the same as Energy.
*/
class Coin final : public Buff
{
public:
    Coin(Scene* scene) : Buff(scene), m_radius(0.0)
    {
    }

    ~Coin() override
    {
    }

    const char* Name() const override
    {
        return "Coin";
    }

    Coin* Clone() const override;

    virtual void Clone(Coin* clone) const
    {
    }

    bool Load(XMLElement* node) override;

    virtual void Activate();

public:
    double GetRadius() const
    {
        return m_radius;
    }

protected:
    void _InitBehavior(XMLElement* node = nullptr) override;

private:
    double m_radius;
};

/********************************************************************
** Drug restores hero's mp or hp.
*/
class Drug : public Buff
{
public:
    Drug(Scene* scene) : Buff(scene)
    {
    }

    ~Drug() override
    {
    }

    const char* Name() const override = 0;

    Drug* Clone() const override = 0;
    virtual void Clone(Drug* clone) const;

    bool Load(XMLElement* node) override;

public:
    double GetRadius() const
    {
        return m_radius;
    }

protected:
    void _InitBehavior(XMLElement* node = nullptr) override
    {
    }

    double m_radius;
};

class HPDrug : public Drug
{
public:
    HPDrug(Scene* scene) : Drug(scene)
    {
    }

    ~HPDrug() override
    {
    }

    const char* Name() const override
    {
        return "HPDrug";
    }

    HPDrug* Clone() const override;

    virtual void Clone(HPDrug* clone) const
    {
    }

    bool Load(XMLElement* node) override;

protected:
    void _InitBehavior(XMLElement* node = nullptr) override;
};

class MPDrug : public Drug
{
public:
    MPDrug(Scene* scene) : Drug(scene)
    {
    }

    ~MPDrug() override
    {
    }

    const char* Name() const override
    {
        return "MPDrug";
    }

    MPDrug* Clone() const override;

    virtual void Clone(MPDrug* clone) const
    {
    }

    bool Load(XMLElement* node) override;

protected:
    void _InitBehavior(XMLElement* node = nullptr) override;
};

#endif
