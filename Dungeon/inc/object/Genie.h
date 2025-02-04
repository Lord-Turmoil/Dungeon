/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Genie.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 19, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Genie is enemy that holds weapon.                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _GENIE_H_
#define _GENIE_H_

#include "Enemy.h"

class Genie : public Enemy
{
public:
    Genie(Scene* pScene) : Enemy(pScene)
    {
    }

    ~Genie() override
    {
    }

    Genie* Clone() const override;
    virtual void Clone(Genie* clone) const;

    bool Load(XMLElement* node) override;

public:
    double GetScareRadius() const
    {
        return m_scareRadius;
    }

public:
    void _InitBehavior(XMLElement* node = nullptr) override;

    double m_scareRadius;
};

#endif
