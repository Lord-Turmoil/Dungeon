/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Buff.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 13, 2022                           *
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
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENERGY_H_
#define _ENERGY_H_

#include "Object.h"

/********************************************************************
** Buff includes energy(restore mp), hp/mp-drug and coin.
*/
class Buff : public Object
{
public:
	Buff(Scene* scene) : Object(ObjectType::OBJ_BUFF, scene), m_value(0)
	{
		m_symbol.SetLayer(LAYER_BUFF);
	}
	virtual ~Buff() {}

	virtual const char* Name() const = 0;

	virtual Buff* Clone() const = 0;
	virtual void Clone(Buff* clone) const;

	virtual bool Load(XMLElement* node);

public:
	int GetValue() const { return m_value; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr) {}

private:
	int m_value;
};


/********************************************************************
** Energy flies towards the hero and restore mp.
*/
class Energy final : public Buff
{
public:
	Energy(Scene* scene) : Buff(scene), m_radius(0.0) {}
	virtual ~Energy() {}

	virtual const char* Name() const { return "Energy"; }

	virtual Energy* Clone() const;
	virtual void Clone(Energy* clone) const {}

	virtual bool Load(XMLElement* node);

	void Activate();

public:
	double GetRadius() const { return m_radius; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);

private:
	double m_radius;
};


/********************************************************************
** Coin is almost the same as Energy.
*/
class Coin final : public Buff
{
public:
	Coin(Scene* scene) : Buff(scene), m_radius(0.0) {}
	virtual ~Coin() {}

	virtual const char* Name() const { return "Coin"; }

	virtual Coin* Clone() const;
	virtual void Clone(Coin* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Activate();

public:
	double GetRadius() const { return m_radius; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);

private:
	double m_radius;
};


/********************************************************************
** Drug restores hero's mp or hp.
*/
class Drug : public Buff
{
public:
	Drug(Scene* scene) : Buff(scene) {}
	virtual ~Drug() {}

	virtual const char* Name() const = 0;

	virtual Drug* Clone() const = 0;
	virtual void Clone(Drug* clone) const;

	virtual bool Load(XMLElement* node);

public:
	double GetRadius() const { return m_radius; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr) {}

	double m_radius;
};

class HPDrug : public Drug
{
public:
	HPDrug(Scene* scene) : Drug(scene) {}
	virtual ~HPDrug() {}

	virtual const char* Name() const { return "HPDrug"; }

	virtual HPDrug* Clone() const;
	virtual void Clone(HPDrug* clone) const {}

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);
};

class MPDrug : public Drug
{
public:
	MPDrug(Scene* scene) : Drug(scene) {}
	virtual ~MPDrug() {}

	virtual const char* Name() const { return "MPDrug"; }

	virtual MPDrug* Clone() const;
	virtual void Clone(MPDrug* clone) const {}

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);
};

#endif