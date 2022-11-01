/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : PortalBehavior.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 4, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Portal behavior.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/game/Dungeon.h"

#include "../../inc/object/Hero.h"
#include "../../inc/object/Portal.h"
#include "../../inc/object/PortalBehavior.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** PortalBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void PortalBehavior::Clone(PortalBehavior* clone) const
{
	Behavior::Clone(clone);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** PortalIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
PortalIdle* PortalIdle::Clone() const
{
	PortalIdle* clone = new PortalIdle();
	clone->_MakePrototype(false);

	PortalBehavior::Clone(clone);

	return clone;
}

void PortalIdle::OnEnter()
{
	m_parent->GetGameObject()->SetDisplay(false);
}

void PortalIdle::OnExit()
{
	m_parent->GetGameObject()->SetDisplay(true);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** PoratlInit
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
PortalInit* PortalInit::Clone() const
{
	PortalInit* clone = new PortalInit();
	clone->_MakePrototype(false);

	PortalBehavior::Clone(clone);

	return clone;
}

void PortalInit::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Ready");
	}
}

void PortalInit::OnEnter()
{
	Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim();

	anim->SetMotion(PORTAL_ANIM_INIT);
	anim->SetDir(ANIM_LEFT);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** PortalReady
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
PortalReady* PortalReady::Clone() const
{
	PortalReady* clone = new PortalReady();
	clone->_MakePrototype(false);

	PortalBehavior::Clone(clone);

	return clone;
}

void PortalReady::Update(Event* evnt)
{
	Portal* portal = static_cast<Portal*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(portal->GetScene());
	Hero* hero = dungeon->GetHero();
	double dist = Distance(hero->GetCoord(), portal->GetCoord());

	if (dist < portal->GetRadius())
	{

		m_parent->GetGameObject()->GetComponent<AnimComponent>()
			->GetAnim()->SetDir(ANIM_RIGHT);
		if (evnt->Sluggish(CMD_ACTIVATE))
		{
			dungeon->SetLevelUp();
			m_parent->ChangeBehavior("Idle");
		}
	}
	else
	{
		m_parent->GetGameObject()->GetComponent<AnimComponent>()
			->GetAnim()->SetDir(ANIM_LEFT);
	}
}

void PortalReady::OnEnter()
{
	Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim();
	
	anim->SetMotion(PORTAL_ANIM_READY);
	anim->SetDir(ANIM_LEFT);
}
