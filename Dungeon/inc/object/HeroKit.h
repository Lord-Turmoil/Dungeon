/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroKit.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 31, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Hero factory.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _HERO_KIT_H_
#define _HERO_KIT_H_

#include <dungine.h>

class Hero;

class HeroKit : public AbstractKit<Hero>
{
public:
    HeroKit()
    {
    }

    ~HeroKit() override
    {
    }

    Hero* LoadObject(XMLElement* node) override;
};

#endif
