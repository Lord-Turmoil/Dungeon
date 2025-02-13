/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Boss.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 25, 2022                           *
 *                                                                            *
 *                    Last Update : December 13, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For boss.                                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _BOSS_H_
#define _BOSS_H_

#include "Genie.h"

/********************************************************************
** Boss has its specific behavior and state. And it won't be removed
** after defeated.
*/
class Boss : public Genie
{
public:
    Boss(Scene* pScene) : Genie(pScene), m_isUsingSkill(false)
    {
    }

    ~Boss() override
    {
    }

    Boss* Clone() const override = 0;
    virtual void Clone(Boss* clone) const;

    bool Load(XMLElement* node) override;

    /*
    ** 2022/12/13 TS:
    ** Used to enhance boss in Infinite mode.
    */
    virtual void Enhance(float rate);

public:
    void SetUsingSkill(bool isUsingSkill)
    {
        m_isUsingSkill = isUsingSkill;
    }

    bool IsUsingSkill() const
    {
        return m_isUsingSkill;
    }

protected:
    void _InitBehavior(XMLElement* node = nullptr) override
    {
    }

    void _InitState() override
    {
    }

    bool m_isUsingSkill;
};

/********************************************************************
** Rock is a giant stone, it can summon small stones. :)
*/
class ERock final : public Boss
{
public:
    ERock(Scene* pScene) : Boss(pScene)
    {
    }

    ~ERock() override
    {
    }

    ERock* Clone() const override;

    virtual void Clone(ERock* clone) const
    {
    }

    bool Load(XMLElement* node) override;

protected:
    void _InitBehavior(XMLElement* node = nullptr) override;
    void _InitState() override;
};

/********************************************************************
** Well, this is Dark Black Hand, the same as the hero Black Hand.
** abbr. DBK
*/
class EBlackHand final : public Boss
{
public:
    EBlackHand(Scene* pScene) : Boss(pScene)
    {
    }

    ~EBlackHand() override
    {
    }

    EBlackHand* Clone() const override;

    virtual void Clone(EBlackHand* clone) const
    {
    }

    bool Load(XMLElement* node) override;

protected:
    void _InitBehavior(XMLElement* node = nullptr) override;
    void _InitState() override;
};

#endif
