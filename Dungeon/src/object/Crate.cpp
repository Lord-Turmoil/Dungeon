/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Crate.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 15, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Crate contains some buff or weapon.                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/object/Buff.h"
#include "../../inc/object/Crate.h"
#include "../../inc/object/CrateBehavior.h"
#include "../../inc/object/Weapon.h"
#include "../../inc/object/Hero.h"
#include "../../inc/object/Library.h"

#include "../../inc/game/Dungeon.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Crate
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Crate::~Crate()
{
	for (auto it = m_buffs.begin(); it != m_buffs.end(); it++)
		delete (*it);
	_DELETE(m_weapon);
}

/********************************************************************
** Clone content? I think not.
*/
Crate* Crate::Clone() const
{
	Crate* clone = new Crate(m_pScene);
	clone->_MakePrototype(false);

	Object::Clone(clone);
	
	clone->m_radius = m_radius;

	return clone;
}

bool Crate::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("Crate");
	_RETURN_IF_ERROR();

	Object::Load(node);

	_PARSE_ESSENTIAL("radius", m_radius, name, 0.0);

	_InitBehavior();

	_RETURN_STATE();
}

/********************************************************************
** Energy And Coin -- 60%
** Drug            -- 20%
** Weapon          -- 20%
*/
void Crate::Generate()
{
	Dungeon* dungeon = static_cast<Dungeon*>(m_pScene);
	Hero* hero = dungeon->GetHero();

	if ((hero->GetHPPercent() < 0.6) && (Random(100) < 40))
		_GenerateDrug(80);
	else if ((hero->GetMPPercent() < 0.3) && (Random(100) < 30))
		_GenerateDrug(20);
	else
	{
		int decision = Random(100);
		if (decision < 20)
		{
			if (!_GenerateWeapon())
			{
#ifdef DUNGEON_DEBUG
				LOG_MESSAGE("Run out of weapons");
#endif
				_GenerateEnergyAndCoin();
			}
		}
		else if (decision < 40)
			_GenerateDrug(30);
		else
			_GenerateEnergyAndCoin();
	}
}

void Crate::Open()
{
	if (m_weapon)
	{
		m_weapon->SetActive(true);
		m_pScene->AddObject(m_weapon);
		m_weapon = nullptr;
	}
	else
	{
		for (auto it = m_buffs.begin(); it != m_buffs.end(); it++)
		{
			(*it)->SetActive(true);
			m_pScene->AddObject(*it);
		}
		m_buffs.clear();
	}

	m_isOpen = true;
}

void Crate::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new CrateInit());
	parent->AddBehavior(new CrateIdle());
	parent->AddBehavior(new CrateOpening());
	parent->AddBehavior(new CrateOpen());

	parent->ChangeBehavior("Init");
}

void Crate::_GenerateDrug(int hpPercent)
{
	Drug* drug;

	if (Random(100) < hpPercent)
		drug = MiscLibrary::GetInstance()->GetMiscObject<HPDrug>("HPDrug")->Clone();
	else
		drug = MiscLibrary::GetInstance()->GetMiscObject<MPDrug>("MPDrug")->Clone();

	drug->SetCoord(GetCenter());
	drug->SetActive(false);
	m_buffs.push_back(drug);
}

bool Crate::_GenerateWeapon()
{
	Coordinate coord = GetCoord();
	coord.y++;

	Weapon* weapon = WeaponLibrary::GetInstance()->GetWeapon();
	if (!weapon)
		return false;

	m_weapon = weapon->Clone();
	m_weapon->SetCoord(coord);
	m_weapon->SetActive(false);

	return true;
}


/********************************************************************
** Be aware that if too many objects gather in one coordinate, it may
** cause failure of QuadTree.
*/
void Crate::_GenerateEnergyAndCoin()
{
	int num;
	Coordinate center;

	// energy
	{
		center = GetCenter();
		center.x--;
		num = Random(20, 40);
		Energy* proto = MiscLibrary::GetInstance()->GetMiscObject<Energy>("Energy");
		Energy* energy;
		for (int i = 0; i < num; i++)
		{
			if (i == 20)
				center.x += 2;
			energy = proto->Clone();
			energy->SetCoord(center);
			energy->Activate();
			energy->SetActive(false);
			m_buffs.push_back(energy);
		}
	}

	// coin
	{
		center = GetCenter();
		center.y += 2;
		center.x--;
		num = Random(10, 30);
		Coin* proto = MiscLibrary::GetInstance()->GetMiscObject<Coin>("Coin");
		Coin* coin;
		for (int i = 0; i < num; i++)
		{
			if (i == 15)
				center.x++;
			coin = proto->Clone();
			coin->SetCoord(center);
			coin->Activate();
			coin->SetActive(false);
			m_buffs.push_back(coin);
		}
	}
}