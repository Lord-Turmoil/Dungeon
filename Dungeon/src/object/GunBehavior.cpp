/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GunBehavior.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 25, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Gun behavior.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/GunBehavior.h"
#include "../../inc/object/Weapon.h"
#include "../../inc/object/Figure.h"
#include "../../inc/object/Component.h"
#include "../../inc/common/Math.h"


void GunBehavior::Clone(GunBehavior* clone) const
{
	WeaponBehavior::Clone(clone);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GunReady
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
GunReady* GunReady::Clone() const
{
	GunReady* clone = new GunReady();
	clone->_MakePrototype(false);

	GunBehavior::Clone(clone);

	return clone;
}

void GunReady::Update(Event* evnt)
{	
	Gun* gun = static_cast<Gun*>(m_parent->GetGameObject());
	
	gun->AdjustPosture();
	if (gun->IsTriggered())
	{
		Figure* figure = static_cast<Figure*>(gun->GetSlot()->GetGameObject());
		if (figure->GetMP() >= gun->GetCost())
		{
			figure->CostMP(gun->GetCost());
			m_parent->ChangeBehavior("Fire");
		}
	}
}

void GunReady::OnEnter()
{
	m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(WEAPON_ANIM_READY);
}