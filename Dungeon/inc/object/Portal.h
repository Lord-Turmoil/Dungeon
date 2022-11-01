/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Portal.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 4, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Portal.                                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _PORTAL_H_
#define _PORTAL_H_

#include "Object.h"

class Portal final : public Object
{
public:
	Portal(Scene* scene) : Object(ObjectType::OBJ_PORTAL, scene), m_radius(0.0)
	{
		m_symbol.SetLayer(LAYER_FIGURE);
	}
	virtual ~Portal() {}

	virtual Portal* Clone() const;
	virtual void Clone(Portal* clone) const {}

	virtual bool Load(XMLElement* node);

	void Initialize();

public:
	double GetRadius() const { return m_radius; }

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr);

	double m_radius;
};

#endif
