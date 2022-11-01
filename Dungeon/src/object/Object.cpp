/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Object.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 24, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For the base class of all game objects.                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Object.h"
#include "../../inc/object/ComponentKit.h"


/******************************************************************************
 * Object::Clone -- Clone object.                                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   clone                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void Object::Clone(Object* clone) const
{
	GameObject::Clone(clone);
	clone->m_type = m_type;
	clone->m_centerOffset = m_centerOffset;
}


/******************************************************************************
 * Object::Load                                                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return state.                                                     *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/26 Tony : Created.                                               *
 *============================================================================*/
bool Object::Load(XMLElement* node)
{
/*
**	<...>					<- node
**		<Components>
**			<...></...>
**			<...></...>
**		</Components>
**	</...>
*/
	const char* name = node->Name();
	const char* attr;

	XMLElement* tag = node->FirstChildElement("Components");
	if (!tag)
		_RETURN_STATE();

	_PARSE_PRIVATE("center", m_centerOffset, name, ParseCoord);

	ComponentKit kit;
	AbstractComponent* cmpt;

	tag = tag->FirstChildElement();
	while (tag)
	{
		cmpt = kit.LoadComponent(tag);

#ifdef DUNGEON_DEBUG
		if (!cmpt)
		{
			LOG_ERROR(R"(Failed to load components of "%s")", name);
			return false;
		}
#endif

		AddComponent(cmpt);

		tag = tag->NextSiblingElement();
	}

	_RETURN_STATE();
}


/******************************************************************************
 * Object::Translate -- Translate object.                                     *
 *                                                                            *
 *    All related components have to translate along with the object.         *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void Object::Translate(const Coordinate& offset)
{
	m_coord += offset;
	for (auto it = m_components.begin(); it != m_components.end(); it++)
		it->second->Translate(offset);
}