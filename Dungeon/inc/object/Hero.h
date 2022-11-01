/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Hero.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 14, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Hero of the game.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _HERO_H_
#define _HERO_H_

#include "Figure.h"

class Hero : public Figure
{
public:
	Hero(Scene* pScene) : Figure(ObjectType::OBJ_HERO, pScene) {}
	virtual ~Hero() {}

	virtual Hero* Clone() const = 0;
	virtual void Clone(Hero* clone) const;

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);
	virtual void _InitState() = 0;
};


/********************************************************************
** Specific heroes just differ in skill states and some components'
** properties, so the logic of behavior is consistent. Gooooood. :P
** All hero class name begin with 'H'.
*/

/********************************************************************
** Well, Yony is the basic hero, it doesn's have fancy skills.
** And... Does 'Y' look like 'T'? :)
*/
class HYony final : public Hero
{
public:
	HYony(Scene* pScene) : Hero(pScene) {}
	virtual ~HYony() {}

	virtual HYony* Clone() const;
	virtual void Clone(HYony* clone) const {}

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitState();

private:

};


/********************************************************************
** Specter is an original character of this game. Just like its name,
** it can transform into a specter to avoid taking damage.
*/
class HSpecter final : public Hero
{
public:
	HSpecter(Scene* pScene) : Hero(pScene) {}
	virtual ~HSpecter() {}

	virtual HSpecter* Clone() const;
	virtual void Clone(HSpecter* clone) const {}

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitState();
};


/********************************************************************
** Hmm... Idea comes from Command & Conquer 3. Blackhand, the elite
** infantry unit of the Brotherhood of Nod. Just, features in fire.
*/
class HBlackHand final : public Hero
{
public:
	HBlackHand(Scene* pScene) : Hero(pScene) {}
	virtual ~HBlackHand() {}

	virtual HBlackHand* Clone() const;
	virtual void Clone(HBlackHand* clone) const {}

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitState();

private:

};

#endif