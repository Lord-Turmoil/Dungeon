/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Stand.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : December 8, 2022                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The stand...                                                             *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/game/Flashback.h"

#include "../../inc/object/Stand.h"
#include "../../inc/object/StandBehavior.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Stand
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Stand::Stand(Scene* scene) : Object(ObjectType::OBJ_STAND, scene),
	m_isActive(false), m_radius(0.0)
{
	m_symbol.SetLayer(LAYER_FIGURE);
}

Stand::~Stand()
{
	if (m_pFlashback)
		delete m_pFlashback;
}

Stand* Stand::Clone() const
{
	Stand* clone = new Stand(m_pScene);
	clone->_MakePrototype(false);

	Object::Clone(clone);

	clone->m_isActive = m_isActive;
	clone->m_radius = m_radius;

	return clone;
}

bool Stand::Load(XMLElement* node)
{
	/*
	**	<Stand radius="">
	**		<Components>
	**		</Components>
	**	</Stand>
	*/
	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("Stand");
	_RETURN_IF_ERROR();

	Object::Load(node);

	_PARSE_ESSENTIAL("radius", m_radius, name, 0.0);

	_InitBehavior();

	_RETURN_STATE();
}

Flashback* Stand::GetFlashback()
{
	if (!m_pFlashback)
		m_pFlashback = new Flashback();
	return m_pFlashback;
}

void Stand::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new StandIdle());
	parent->AddBehavior(new StandChoose());
	parent->AddBehavior(new StandGreet());
	parent->AddBehavior(new StandSave());
	parent->AddBehavior(new StandFlash());
	parent->AddBehavior(new StandSaved());
	parent->AddBehavior(new StandFlashed());
	parent->AddBehavior(new StandInsufficient());
	parent->AddBehavior(new StandComplete());

	parent->ChangeBehavior("Idle");
}