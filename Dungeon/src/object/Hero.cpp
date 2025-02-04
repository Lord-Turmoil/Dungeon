/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Hero.cpp                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 23, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Hero of the game.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Hero.h"
#include "../../inc/object/HeroBehavior.h"
#include "../../inc/object/HeroState.h"

/******************************************************************************
 * Hero::Clone -- Clone hero.                                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   clone                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/23 Tony : Created.                                               *
 *============================================================================*/
void Hero::Clone(Hero* clone) const
{
    Figure::Clone(clone);
}

/******************************************************************************
 * Hero::Load                                                                 *
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
 *   2022/07/23 Tony : Created.                                               *
 *============================================================================*/
bool Hero::Load(XMLElement* node)
{
    Figure::Load(node);

    _RETURN_STATE();
}

/******************************************************************************
 * Hero::_InitBehavior -- Initialize behavior of heroes.                      *
 *                                                                            *
 *    Generally, all heros share the same behaviors.                          *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/23 Tony : Created.                                               *
 *============================================================================*/
void Hero::_InitBehavior(XMLElement* node)
{
    auto parent = GetComponent<BehaviorComponent>();

    parent->AddBehavior(new HeroInit());
    parent->AddBehavior(new HeroMove());
    parent->AddBehavior(new HeroDead());
    parent->AddBehavior(new HeroPerish());

    parent->ChangeBehavior("Init");
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Yony
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HYony* HYony::Clone() const
{
    HYony* clone = new HYony(m_pScene);
    clone->_MakePrototype(false);

    Hero::Clone(clone);

    return clone;
}

bool HYony::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Yony");
    _RETURN_IF_ERROR();

    Hero::Load(node);

    _RETURN_STATE();
}

void HYony::_InitState()
{
    StateComponent* parent = GetComponent<StateComponent>();

    parent->AddState(new HeroNone());
    parent->AddState(new HeroHurt());
    parent->AddState(new YonySkill());

    parent->ChangeState("None");
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Specter
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HSpecter* HSpecter::Clone() const
{
    HSpecter* clone = new HSpecter(m_pScene);
    clone->_MakePrototype(false);

    Hero::Clone(clone);

    return clone;
}

bool HSpecter::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Specter");
    _RETURN_IF_ERROR();

    Hero::Load(node);

    _RETURN_STATE();
}

void HSpecter::_InitState()
{
    StateComponent* parent = GetComponent<StateComponent>();

    parent->AddState(new HeroNone());
    parent->AddState(new HeroHurt());
    parent->AddState(new SpecterSkill());

    parent->ChangeState("None");
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** BlackHand
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HBlackHand* HBlackHand::Clone() const
{
    HBlackHand* clone = new HBlackHand(m_pScene);
    clone->_MakePrototype(false);

    Hero::Clone(clone);

    return clone;
}

bool HBlackHand::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("BlackHand");
    _RETURN_IF_ERROR();

    Hero::Load(node);

    _RETURN_STATE();
}

void HBlackHand::_InitState()
{
    StateComponent* parent = GetComponent<StateComponent>();

    parent->AddState(new HeroNone());
    parent->AddState(new HeroHurt());
    parent->AddState(new BlackHandSkill());

    parent->ChangeState("None");
}
