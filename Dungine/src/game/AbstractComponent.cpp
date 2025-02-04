/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : AbstractComponent.cpp                     *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 7, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Provide basic components. The components in this files are not complete, *
 *   they must be inherited.                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/device/Explorer.h"
#include "../../inc/device/Timer.h"

#include "../../inc/game/AbstractComponent.h"

#include "../../inc/utility/Parser.h"

/******************************************************************************
 * AbstractComponent::Clone                                                   *
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
void AbstractComponent::Clone(AbstractComponent* clone) const
{
    // clone->m_updateOrder = m_updateOrder;
    // clone->m_pGameObject = m_pGameObject;
}

/******************************************************************************
 * AbstractComponent::Load -- Load component's basic info.                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/26 Tony : Created.                                               *
 *============================================================================*/
bool AbstractComponent::Load(XMLElement* node)
{
    /*
    **	<... order="">
    **		...
    **	</...>
    */
    const char* name = node->Name();
    const char* attr;

    _PARSE("order", m_updateOrder, name, m_updateOrder);

    return true;
}
