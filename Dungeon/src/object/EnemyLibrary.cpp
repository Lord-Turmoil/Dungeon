/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyLibrary.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 21, 2022                             *
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

#include "../../inc/object/EnemyLibrary.h"
#include "../../inc/object/Enemy.h"
#include "../../inc/object/EnemyKit.h"

/******************************************************************************
 * EnemyLibrary::Load -- Load enemy library.                                  *
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
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
bool EnemyLibrary::Load(XMLElement* node)
{
    /*
    **	<EnemyLibrary>
    **		<EnemyName ...>...</EnemyName>
    **		<EnemyName ...>...</EnemyName>
    **		<EnemyName ...>...</EnemyName>
    **	</EnemyLibrary>
    */
    UnLoad();

    const char* name = node->Name();

    _CHECK_TAG("EnemyLibrary");
    _RETURN_IF_ERROR();

    EnemyKit kit;
    XMLElement* elem = node->FirstChildElement();
    while (elem)
    {
        AddEnemy(kit.LoadObject(elem));
        elem = elem->NextSiblingElement();
    }

    _RETURN_STATE();
}

/******************************************************************************
 * EnemyLibrary::UnLoad -- Unload enemy library.                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
void EnemyLibrary::UnLoad()
{
    for (auto it = m_pool.begin(); it != m_pool.end(); it++)
    {
        delete it->second;
    }
    m_pool.clear();
    m_categoryPool.clear();
}

/******************************************************************************
 * EnemyLibrary::AddEnemy -- Add enemy to the library.                        *
 *                                                                            *
 *    This add two info, name and level.                                      *
 *                                                                            *
 * INPUT:   enemy                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
Enemy* EnemyLibrary::AddEnemy(Enemy* enemy)
{
    auto it = m_pool.find(enemy->Name());
    if (it != m_pool.end())
    {
        LOG_ERROR(NAME_CONFLICT, "Enemy Library", enemy->Name().c_str());
        return nullptr;
    }

    m_pool.emplace(enemy->Name(), enemy);
    m_categoryPool[enemy->Level()].push_back(enemy);

    return enemy;
}

/******************************************************************************
 * EnemyLibrary::GetEnemyByName -- Get enemy by name.                         *
 *                                                                            *
 *    Notice that, some special enemies have negative level, which is prefferd*
 *    to only be got by name.
 *                                                                            *
 * INPUT:   name                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
Enemy* EnemyLibrary::GetEnemyByName(const std::string& name)
{
    return GetItem(name);
}

/******************************************************************************
 * EnemyLibrary::GetEnemyByLevel -- Get enemy by level.                       *
 *                                                                            *
 *    This will get many enemies.                                             *
 *                                                                            *
 * INPUT:   level.                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
const std::vector<Enemy*>& EnemyLibrary::GetEnemyByLevel(int level)
{
    auto it = m_categoryPool.find(level);

    if (it != m_categoryPool.end())
    {
        return it->second;
    }

    /*
    ** This shouldn't happen, but as it will. :)
    */
    LOG_EXTRA_WARNING("Missing enemy of level %d", level);

    static std::vector<Enemy*> dummy;

    return dummy;
}
