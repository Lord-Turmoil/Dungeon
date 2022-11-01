/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : WeaponComponent.cpp                       *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 27, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Weapon slot.                                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Weapon.h"
#include "../../inc/object/WeaponComponent.h"
#include "../../inc/object/WeaponLibrary.h"

#include "../../inc/object/Figure.h"


/******************************************************************************
 * WeaponComponent::WeaponComponent -- Constructor of the object.             *
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
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
WeaponComponent::WeaponComponent(int updateOrder) :
	AbstractComponent(updateOrder), m_capacity(0), m_isArmed(false)
{
	m_current = m_weapons.end();
}


/******************************************************************************
 * WeaponComponent::~WeaponComponent -- Destructor of the object.             *
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
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
WeaponComponent::~WeaponComponent()
{
	/*
	** 2022/08/02: TS
	** Huh??
	*/
	UnEquip();
	for (auto it = m_weapons.begin(); it != m_weapons.end(); it++)
		delete (*it);
}


/******************************************************************************
 * WeaponComponent::Clone -- Clone.                                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return the clone.                                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/27 Tony : Created.                                               *
 *   2022/08/17 Tony : BUG when multiple weapon exsits Fixed.                 *
 *============================================================================*/
WeaponComponent* WeaponComponent::Clone() const
{
	WeaponComponent* clone = new WeaponComponent(m_updateOrder);
	clone->_MakePrototype(false);

	AbstractComponent::Clone(clone);

	clone->m_capacity = m_capacity;
	clone->m_offset = m_offset;

	/*
	** 2022/08/17 TS:
	** Here, we use vector... unlike map, vector may move its memory
	** if is full, which will cause clone->m_current invaild. So we
	** should first reserve the space. :(
	*/
	Weapon* weapon;

	// This line is ESSENTIAL!!! :(
	clone->m_weapons.reserve(m_weapons.size());

	for (auto it = m_weapons.begin(); it != m_weapons.end(); it++)
	{
		weapon = (*it)->Clone();
		weapon->SetSlot(clone);
		clone->m_weapons.push_back(weapon);
		if (it == m_current)
			clone->m_current = clone->m_weapons.end() - 1;
	}

	return clone;
}


/******************************************************************************
 * WeaponComponent::Load -- Load weapon component.                            *
 *                                                                            *
 *    Number of weapons should not be greater than capacity. Exceeded weapons *
 *    will be ignored.                                                        *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
bool WeaponComponent::Load(XMLElement* node)
{
/*
**	<WeaponSlot capacity="" offset="">
**		<Weapon></Weapon>
**		<Weapon></Weapon>
**		<Weapon></Weapon>
**	</WeaponSlot>
*/
	const char* name = node->Name();
	const char* attr;
	const char* text;
	Weapon* weapon;
	
	_CHECK_TAG(StaticName());
	_RETURN_IF_ERROR();

	AbstractComponent::Load(node);

	_PARSE("capacity", m_capacity, name, 0);
	_PARSE_PRIVATE("offset", m_offset, name, ParseCoord);

	std::vector<XMLElement*> weaponList = GetElementsByTagName(node, "Weapon");
	for (auto it = weaponList.begin(); it != weaponList.end(); it++)
	{
		text = (*it)->GetText();
		if (!text)
		{
			LOG_ERROR("Missing name of weapon");
			return false;
		}

		weapon = WeaponLibrary::GetInstance()->GetWeaponByName(text);
		if (!weapon)
		{
			LOG_ERROR("Missing weapon \"%s\"", text);
			return false;
		}

		if (m_weapons.size() < m_capacity)
		{
			weapon = weapon->Clone();
			weapon->SetSlot(this);
			m_weapons.push_back(weapon);
		}
	}

	m_current = m_weapons.begin();

	_RETURN_STATE();
}


/******************************************************************************
 * WeaponComponent::Update -- Update weapon component.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
void WeaponComponent::Update(Event* evnt)
{
	m_center = m_pGameObject->GetCoord() + m_offset;

	if (m_isArmed)
		(*m_current)->Update(evnt);
}


/******************************************************************************
 * WeaponComponent::Translate -- Translate weapon slot.                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   offset                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void WeaponComponent::Translate(const Coordinate& offset)
{
	/*
	m_center += offset;
	if (m_isArmed)
		(*m_current)->Translate(offset);
	*/
}


/******************************************************************************
 * WeaponComponent::Equip -- Equip the figure with the current weapon.        *
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
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
void WeaponComponent::Equip()
{
	if (IsEmpty())
		return;

	Figure* figure = static_cast<Figure*>(m_pGameObject);
	Weapon* weapon = *m_current;

	figure->GetSymbol()->SetSubSymbol(weapon->GetSymbol());
	weapon->SetScene(figure->GetScene());
	weapon->Equip();
	m_isArmed = true;
}


/******************************************************************************
 * WeaponComponent::UnEquip -- Unequip the weapon.                            *
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
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
void WeaponComponent::UnEquip()
{
	m_pGameObject->GetSymbol()->SetSubSymbol(nullptr);

	if (m_isArmed)
	{
		(*m_current)->UnEquip();
		m_isArmed = false;
	}
}


/******************************************************************************
 * WeaponComponent::PickUpWeapon -- Pick up weapon.                           *
 *                                                                            *
 *    Hmm... This is a little tricky. When picking up weapon, it should just  *
 *    remove the weapon from the object pool of the scene, then add it to the *
 *    figure. If the slot is full, it has to drop the current weapon and then *
 *    again, put it into the object pool.                                     *
 *                                                                            *
 * INPUT:   weapon                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
void WeaponComponent::PickUpWeapon(Weapon* weapon)
{
	UnEquip();

	if (IsFull())
	{
		(*m_current)->SetSlot(nullptr);
		(*m_current)->Drop();
		m_pGameObject->GetScene()->AddObject(*m_current);

		std::swap(*m_current, *(m_weapons.end() - 1));
		m_weapons.pop_back();
	}

	m_pGameObject->GetScene()->RemoveObject(weapon);

	weapon->SetSlot(this);
	m_weapons.push_back(weapon);
	m_current = m_weapons.end() - 1;

	Equip();
}


/******************************************************************************
 * WeaponComponent::SwitchWeapon -- Switch weapon.                            *
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
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
void WeaponComponent::SwitchWeapon()
{
	if (IsEmpty())
		return;

	UnEquip();
	m_current++;
	if (m_current == m_weapons.end())
		m_current = m_weapons.begin();
	Equip();
}


/******************************************************************************
 * WeaponComponent::TrigWeapon -- Trig current weapon.                        *
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
 *   2022/07/28 Tony : Created.                                               *
 *============================================================================*/
void WeaponComponent::TrigWeapon()
{
	if (m_isArmed)
		(*m_current)->Trig();
}

void WeaponComponent::UnTrigWeapon()
{
	if (m_isArmed)
		(*m_current)->UnTrig();
}


/******************************************************************************
 * WeaponComponent::GetCurrentWeapon -- Get current equipped waepon.          *
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
 *   2022/07/27 Tony : Created.                                               *
 *============================================================================*/
Weapon* WeaponComponent::GetCurrentWeapon()
{
	if (!m_isArmed)
		return nullptr;
	else
		return *m_current;
}
