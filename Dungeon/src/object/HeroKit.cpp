/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroKit.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 31, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Hero factory.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Hero.h"
#include "../../inc/object/HeroKit.h"

/******************************************************************************
 * HeroKit::LoadObject -- Load hero.                                          *
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
Hero* HeroKit::LoadObject(XMLElement* node)
{
	const char* name = node->Name();
	Hero* hero = nullptr;

	if (_STR_SAME(name, "Yony"))
		hero = new HYony(nullptr);
	else if (_STR_SAME(name, "Specter"))
		hero = new HSpecter(nullptr);
	else if (_STR_SAME(name, "BlackHand"))
		hero = new HBlackHand(nullptr);

	if (hero)
		hero->Load(node);
	else
		LOG_ERROR(UNKNOWN_TAG, name);

	return hero;
}
