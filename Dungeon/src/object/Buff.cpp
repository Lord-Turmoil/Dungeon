/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Buff.cpp                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Buff is some accessaries to the game.                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/object/Buff.h"
#include "../../inc/object/EnergyBehavior.h"
#include "../../inc/object/CoinBehavior.h"
#include "../../inc/object/DrugBehavior.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Buff
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void Buff::Clone(Buff* clone) const
{
	Object::Clone(clone);

	clone->m_value = m_value;
}

bool Buff::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	Object::Load(node);

	_PARSE("value", m_value, name, 0);

	_InitBehavior();

	_RETURN_STATE();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Energy
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Energy* Energy::Clone() const
{
	Energy* clone = new Energy(m_pScene);
	clone->_MakePrototype(false);

	Buff::Clone(clone);

	clone->m_radius = m_radius;

	return clone;
}

bool Energy::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("Energy");
	_RETURN_IF_ERROR();

	Buff::Load(node);

	_PARSE_ESSENTIAL("radius", m_radius, name, 0.0);

	_RETURN_STATE();
}

void Energy::Activate()
{
	GetComponent<RigidBodyComponent>()->Scatter();
}

void Energy::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new EnergyFly());
	parent->AddBehavior(new EnergyActive());
	parent->AddBehavior(new EnergyPerish());

	parent->ChangeBehavior("Fly");
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Coin
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Coin* Coin::Clone() const
{
	Coin* clone = new Coin(m_pScene);
	clone->_MakePrototype(false);

	Buff::Clone(clone);

	clone->m_radius = m_radius;

	return clone;
}

bool Coin::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("Coin");
	_RETURN_IF_ERROR();

	Buff::Load(node);

	_PARSE_ESSENTIAL("radius", m_radius, name, 0.0);

	_RETURN_STATE();
}

void Coin::Activate()
{
	GetComponent<RigidBodyComponent>()->Scatter();
}

void Coin::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new CoinFly());
	parent->AddBehavior(new CoinActive());
	parent->AddBehavior(new CoinPerish());

	parent->ChangeBehavior("Fly");
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Drug
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
void Drug::Clone(Drug* clone) const
{
	Buff::Clone(clone);

	clone->m_radius = m_radius;
}

bool Drug::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	Buff::Load(node);

	_PARSE_ESSENTIAL("radius", m_radius, name, 0.0);

	_RETURN_STATE();
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HPDrug
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
HPDrug* HPDrug::Clone() const
{
	HPDrug* clone = new HPDrug(m_pScene);
	clone->_MakePrototype(false);

	Drug::Clone(clone);

	return clone;
}

bool HPDrug::Load(XMLElement* node)
{
	const char* name = node->Name();

	_CHECK_TAG("HPDrug");
	_RETURN_IF_ERROR();

	Drug::Load(node);

	_RETURN_STATE();
}

void HPDrug::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new DrugIdle());
	parent->AddBehavior(new HPDrugActive());
	parent->AddBehavior(new DrugPerish());

	parent->ChangeBehavior("Idle");
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** MPDrug
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
MPDrug* MPDrug::Clone() const
{
	MPDrug* clone = new MPDrug(m_pScene);
	clone->_MakePrototype(false);

	Drug::Clone(clone);

	return clone;
}

bool MPDrug::Load(XMLElement* node)
{
	const char* name = node->Name();

	_CHECK_TAG("MPDrug");
	_RETURN_IF_ERROR();

	Drug::Load(node);

	_RETURN_STATE();
}

void MPDrug::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new DrugIdle());
	parent->AddBehavior(new MPDrugActive());
	parent->AddBehavior(new DrugPerish());

	parent->ChangeBehavior("Idle");
}