/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Portal.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 4, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Portal.                                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Portal.h"
#include "../../inc/object/PortalBehavior.h"
#include "../../inc/object/SoundComponent.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Portal
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
Portal::Portal(Scene* scene) : Object(OBJ_PORTAL, scene), m_radius(0.0), m_pFlashback(nullptr)
{
    m_symbol.SetLayer(LAYER_FIGURE);
}

Portal* Portal::Clone() const
{
    Portal* clone = new Portal(m_pScene);
    clone->_MakePrototype(false);

    Object::Clone(clone);

    clone->m_radius = m_radius;

    return clone;
}

bool Portal::Load(XMLElement* node)
{
    /*
    **	<Portal	radius="">
    **		<Components>
    **		</Components>
    **	</Portal>
    */
    const char* name = node->Name();
    const char* attr;

    _CHECK_TAG("Portal");
    _RETURN_IF_ERROR();

    Object::Load(node);

    _PARSE_ESSENTIAL("radius", m_radius, name, 0.0);

    _InitBehavior();

    _RETURN_STATE();
}

void Portal::Initialize()
{
    GetComponent<BehaviorComponent>()->ChangeBehavior("Init");
    GetComponent<SoundComponent>()->Play("init");
}

void Portal::_InitBehavior(XMLElement* node)
{
    auto parent = GetComponent<BehaviorComponent>();

    parent->AddBehavior(new PortalIdle());
    parent->AddBehavior(new PortalInit());
    parent->AddBehavior(new PortalReady());

    parent->ChangeBehavior("Idle");
}
