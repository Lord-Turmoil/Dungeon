/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyKit.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 22, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Factory of enemies.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENEMY_KIT_H_
#define _ENEMY_KIT_H_

#include <dungine.h>

class Enemy;

class EnemyKit : public AbstractKit<Enemy>
{
public:
    EnemyKit()
    {
    }

    ~EnemyKit() override
    {
    }

    Enemy* LoadObject(XMLElement* node) override;
};

#endif
