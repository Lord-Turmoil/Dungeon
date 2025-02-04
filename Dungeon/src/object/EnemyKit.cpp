/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyKit.cpp                              *
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

#include "../../inc/object/EnemyKit.h"
#include "../../inc/object/Boss.h"
#include "../../inc/object/Enemy.h"
#include "../../inc/object/Genie.h"

/******************************************************************************
 * EnemyKit::LoadObject -- Load enemy.                                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/22 Tony : Created.                                               *
 *============================================================================*/
Enemy* EnemyKit::LoadObject(XMLElement* node)
{
    const char* name = node->Name();
    Enemy* enemy = nullptr;

    if (_STR_SAME(name, "Enemy"))
    {
        enemy = new Enemy(nullptr);
    }
    else if (_STR_SAME(name, "Genie"))
    {
        enemy = new Genie(nullptr);
    }
    else if (_STR_SAME(name, "Rock"))
    {
        enemy = new ERock(nullptr);
    }
    else if (_STR_SAME(name, "DBlackHand"))
    {
        enemy = new EBlackHand(nullptr);
    }

    if (enemy)
    {
        enemy->Load(node);
    }
    else
    {
        LOG_ERROR(UNKNOWN_TAG, name);
    }

    return enemy;
}
