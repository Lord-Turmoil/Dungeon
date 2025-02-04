/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroLibrary.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 31, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Hero library.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/HeroLibrary.h"
#include "../../inc/object/Hero.h"
#include "../../inc/object/HeroKit.h"

/******************************************************************************
 * HeroLibrary::Load -- Load the entire library.                              *
 *                                                                            *
 *    Since heroes are fixed, they are placed in a seperated xml file.        *
 *                                                                            *
 * INPUT:   filename -- XML file name.                                        *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
bool HeroLibrary::Load(const char* filename)
{
    XMLFile file;

    file.Load(filename);

    Load(file.GetRoot());

    file.UnLoad();

    _RETURN_STATE();
}

/******************************************************************************
 * HeroLibrary::Load -- Load the entire library.                              *
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
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
bool HeroLibrary::Load(XMLElement* node)
{
    /*
    **	<HeroLibrary>
    **		<HeroName ...>...</HeroName>
    **		<HeroName ...>...</HeroName>
    **		<HeroName ...>...</HeroName>
    **	</HeroLibrary>
    */
    const char* name = node->Name();

    _CHECK_TAG("HeroLibrary");
    _RETURN_IF_ERROR();

    HeroKit kit;
    XMLElement* elem = node->FirstChildElement();
    while (elem)
    {
        AddHero(kit.LoadObject(elem));
        elem = elem->NextSiblingElement();
    }

    _RETURN_STATE();
}

/******************************************************************************
 * HeroLibrary::UnLoad -- Unload the library.                                 *
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
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
void HeroLibrary::UnLoad()
{
    for (auto it = m_pool.begin(); it != m_pool.end(); it++)
    {
        delete it->second;
    }
    m_pool.clear();
}

/******************************************************************************
 * HeroLibrary::AddHero - Add hero to the library.                            *
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
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
Hero* HeroLibrary::AddHero(Hero* hero)
{
    return AddItem(hero->Name(), hero);
}

/******************************************************************************
 * HeroLibrary::GetHeroByName                                                 *
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
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
Hero* HeroLibrary::GetHeroByName(const std::string& name)
{
    return GetItem(name);
}
