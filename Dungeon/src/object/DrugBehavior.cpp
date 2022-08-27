/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : DrugBehavior.cpp                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Drug behavior.                                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/object/Buff.h"
#include "../../inc/object/DrugBehavior.h"
#include "../../inc/object/Hero.h"

#include "../../inc/game/Dungeon.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DrugBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void DrugBehavior::Clone(DrugBehavior* clone) const
{
	Behavior::Clone(clone);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DrugIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DrugIdle* DrugIdle::Clone() const
{
	DrugIdle* clone = new DrugIdle();
	clone->_MakePrototype(false);

	DrugBehavior::Clone(clone);

	return clone;
}

void DrugIdle::Update(Event* evnt)
{
	Drug* drug = static_cast<Drug*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(drug->GetScene());
	Hero* hero = dungeon->GetHero();
	double dist = Distance(hero->GetCenter(), drug->GetCenter());

	if (dist < drug->GetRadius())
	{
		m_parent->GetGameObject()->GetComponent<AnimComponent>()
			->GetAnim()->SetDir(ANIM_RIGHT);
		if (evnt->Sluggish(CMD_ACTIVATE))
			m_parent->ChangeBehavior("Active");
	}
	else
	{
		m_parent->GetGameObject()->GetComponent<AnimComponent>()
			->GetAnim()->SetDir(ANIM_LEFT);
	}
}

void DrugIdle::OnEnter()
{
	Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(DRUG_ANIM_IDLE);
	anim->SetDir(ANIM_LEFT);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DrugActive
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void DrugActive::Clone(DrugActive* clone) const
{
	DrugBehavior::Clone(clone);
}

void DrugActive::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Perish");
	}
}

void DrugActive::OnEnter()
{
	Animation* anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(DRUG_ANIM_ACTIVE);
	anim->SetDir(ANIM_LEFT);

	_Activate();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HPDrugActive
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HPDrugActive* HPDrugActive::Clone() const
{
	HPDrugActive* clone = new HPDrugActive();
	clone->_MakePrototype(false);

	DrugActive::Clone(clone);

	return clone;
}
	
void HPDrugActive::_Activate()
{
	HPDrug* drug = static_cast<HPDrug*>(m_parent->GetGameObject());

	static_cast<Dungeon*>(drug->GetScene())
		->GetHero()->HealHP(drug->GetValue());
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** MPDrugActive
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
MPDrugActive* MPDrugActive::Clone() const
{
	MPDrugActive* clone = new MPDrugActive();
	clone->_MakePrototype(false);

	DrugActive::Clone(clone);

	return clone;
}

void MPDrugActive::_Activate()
{
	HPDrug* drug = static_cast<HPDrug*>(m_parent->GetGameObject());

	static_cast<Dungeon*>(drug->GetScene())
		->GetHero()->HealMP(drug->GetValue());
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** DrugPerish
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
DrugPerish* DrugPerish::Clone() const
{
	DrugPerish* clone = new DrugPerish();
	clone->_MakePrototype(false);

	DrugBehavior::Clone(clone);

	return clone;
}

void DrugPerish::OnEnter()
{
	m_parent->GetGameObject()->SetValid(false);
}
