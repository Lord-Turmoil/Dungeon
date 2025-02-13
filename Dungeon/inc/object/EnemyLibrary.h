/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyLibrary.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Library of enemy.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENEMY_LIBRARY_H_
#define _ENEMY_LIBRARY_H_

#include <vector>

#include <dungine.h>

class Enemy;

class EnemyLibrary : private AbstractLibrary<Enemy>, public Singleton<EnemyLibrary>
{
    friend class Singleton<EnemyLibrary>;

public:
    bool Load(XMLElement* node) override;
    void UnLoad() override;

    Enemy* AddEnemy(Enemy* enemy);
    Enemy* GetEnemyByName(const std::string& name);
    const std::vector<Enemy*>& GetEnemyByLevel(int level);

private:
    EnemyLibrary()
    {
    }

    ~EnemyLibrary() override
    {
    }

    std::map<int, std::vector<Enemy*>> m_categoryPool;
};

#endif
