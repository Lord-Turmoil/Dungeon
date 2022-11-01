/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : CrateBehavior.cpp                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 15, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Crate behavior.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Crate.h"
#include "../../inc/object/CrateBehavior.h"
#include "../../inc/object/Hero.h"
#include "../../inc/game/Dungeon.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CrateBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void CrateBehavior::Clone(CrateBehavior* clone) const
{
	Behavior::Clone(clone);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CrateInit
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
CrateInit* CrateInit::Clone() const
{
	CrateInit* clone = new CrateInit();
	clone->_MakePrototype(false);

	CrateBehavior::Clone(clone);

	return clone;
}

void CrateInit::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Idle");
	}
}

void CrateInit::OnEnter()
{
	Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(CRATE_ANIM_INIT);
	anim->SetDir(ANIM_LEFT);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CrateIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
CrateIdle* CrateIdle::Clone() const
{
	CrateIdle* clone = new CrateIdle();
	clone->_MakePrototype(false);

	CrateBehavior::Clone(clone);

	return clone;
}

void CrateIdle::Update(Event* evnt)
{
	Crate* crate = static_cast<Crate*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(crate->GetScene());
	Hero* hero = dungeon->GetHero();
	double dist = Distance(hero->GetCoord(), crate->GetCoord());

	if (dist < crate->GetRadius())
	{
		m_parent->GetGameObject()->GetComponent<AnimComponent>()
			->GetAnim()->SetDir(ANIM_RIGHT);
		if (evnt->Sluggish(CMD_ACTIVATE))
			m_parent->ChangeBehavior("Opening");
	}
	else
	{
		m_parent->GetGameObject()->GetComponent<AnimComponent>()
			->GetAnim()->SetDir(ANIM_LEFT);
	}
}

void CrateIdle::OnEnter()
{
	Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(CRATE_ANIM_IDLE);
	anim->SetDir(ANIM_LEFT);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CrateOpening
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
CrateOpening* CrateOpening::Clone() const
{
	CrateOpening* clone = new CrateOpening();
	clone->_MakePrototype(false);

	CrateBehavior::Clone(clone);

	return clone;
}

void CrateOpening::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Open");
	}
}

void CrateOpening::OnEnter()
{
	Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(CRATE_ANIM_OPENING);
	anim->SetDir(ANIM_LEFT);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CrateOpen
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
CrateOpen* CrateOpen::Clone() const
{
	CrateOpen* clone = new CrateOpen();
	clone->_MakePrototype(false);

	CrateBehavior::Clone(clone);

	return clone;
}

void CrateOpen::OnEnter()
{
	Crate* crate = static_cast<Crate*>(m_parent->GetGameObject());
	Animation* anim = crate->GetComponent<AnimComponent>()->GetAnim();

	crate->Open();

	anim->SetMotion(CRATE_ANIM_OPEN);
	anim->SetDir(ANIM_LEFT);
}