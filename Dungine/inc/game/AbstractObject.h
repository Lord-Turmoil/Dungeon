/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : AbstractObject.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 4, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   This file provides the base class of all objects that have an existance  *
 *   on the battlefield. :P                                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ABSTRACT_OBJECT_H_
#define _ABSTRACT_OBJECT_H_

#include "../common/Common.h"
#include "../utility/tinyxml.h"
#include "../template/Prototype.h"


/********************************************************************
** AbstractObject provides basic info of all "actual" objects, it
** includes figures, walls, weapons, bullets even the components.
*/
class AbstractObject : public Prototype<AbstractObject>
{
public:
	AbstractObject(RTTIType RTTI) : m_RTTI(RTTI) {}
	virtual ~AbstractObject() {}

	// :P
	RTTIType WhatAmI() const { return m_RTTI; }
	
	/*
	** Load is only for the prototype, the others are get
	** from Clone().
	*/
	virtual AbstractObject* Clone() const = 0;
	virtual void Clone(AbstractObject* clone) const;

	virtual bool Load(XMLElement* node) = 0;

private:
	RTTIType m_RTTI;
};

#endif