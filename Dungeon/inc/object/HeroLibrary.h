/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroLibrary.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 31, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Hero library.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _HERO_LIBRARY_H_
#define _HERO_LIBRARY_H_

#include <vector>
#include <dungine.h>

class Hero;

class HeroLibrary : private AbstractLibrary<Hero>, public Singleton<HeroLibrary>
{
	friend class Singleton<HeroLibrary>;
public:
	bool Load(const char* filename);

	virtual bool Load(XMLElement* node);
	virtual void UnLoad();

	Hero* AddHero(Hero* enemy);
	Hero* GetHeroByName(const std::string& name);

private:
	HeroLibrary() {}
	virtual ~HeroLibrary() {}
};

#endif