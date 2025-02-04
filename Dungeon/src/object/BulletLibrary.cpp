/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BulletLibrary.cpp                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 21, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Bullet library.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/BulletLibrary.h"
#include "../../inc/object/Bullet.h"
#include "../../inc/object/BulletKit.h"

/******************************************************************************
 * BulletLibrary::Load -- Load bullet library.                                *
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
bool BulletLibrary::Load(const char* filename)
{
    XMLFile file;

    file.Load(filename);

    Load(file.GetRoot());

    file.UnLoad();

    _RETURN_STATE();
}

/******************************************************************************
 * BulletLibrary::Load -- Load bullet library.                                *
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
bool BulletLibrary::Load(XMLElement* node)
{
    /*
    **	<BulletLibrary>
    **		<BulletName ...>...</BuffName>
    **		<BulletName ...>...</BuffName>
    **		<BulletName ...>...</BuffName>
    **	</BulletLibrary>
    */
    const char* name = node->Name();

    _CHECK_TAG("BulletLibrary");
    _RETURN_IF_ERROR();

    BulletKit kit;
    XMLElement* elem = node->FirstChildElement();
    while (elem)
    {
        AddBullet(kit.LoadObject(elem));
        elem = elem->NextSiblingElement();
    }

    _RETURN_STATE();
}

/******************************************************************************
 * BulletLibrary::UnLoad -- Unload bullet library.                            *
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
void BulletLibrary::UnLoad()
{
    for (auto it = m_pool.begin(); it != m_pool.end(); it++)
    {
        delete it->second;
    }
    m_pool.clear();
}

/******************************************************************************
 * BulletLibrary::AddBullet -- AddBullet                                      *
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
Bullet* BulletLibrary::AddBullet(Bullet* bullet)
{
    return AddItem(bullet->Name(), bullet);
}

/******************************************************************************
 * BulletLibrary::GetBulletByName -- Get bullet by name.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
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
Bullet* BulletLibrary::GetBulletByName(const std::string& name)
{
    return GetItem(name);
}
