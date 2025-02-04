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
 * Overview:                                                                 *
 *   Hero library.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _HERO_LIBRARY_H_
#define _HERO_LIBRARY_H_

#include <dungine.h>
#include <vector>

class Hero;

class HeroLibrary : private AbstractLibrary<Hero>, public Singleton<HeroLibrary>
{
    friend class Singleton<HeroLibrary>;

public:
    bool Load(const char* filename);

    bool Load(XMLElement* node) override;
    void UnLoad() override;

    Hero* AddHero(Hero* enemy);
    Hero* GetHeroByName(const std::string& name);

private:
    HeroLibrary()
    {
    }

    ~HeroLibrary() override
    {
    }
};

#endif
