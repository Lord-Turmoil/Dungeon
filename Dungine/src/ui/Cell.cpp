/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Cell.cpp                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 12, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Shape info of widgets.                                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/ui/Cell.h"
#include "../../inc/device/Device.h"
#include "../../inc/utility/Parser.h"

/********************************************************************
** Cell is the basic deviceInfo info of drawer and trigger. Since It
** is designed on a canvas of 640 * 480, its size and position are
** fixed. If float option is on, it's position will be relative to
** the client screen. float has four directions, and top-left is set
** by default, and can add percent to pos for more flexibility. And
** to simplify this, float only affect m_coord, not the actual distance
** with the border of the screen. However, there's a trick that you can
** use focus to make it. Here I didn't use strict grammar check, so
** as long as one percent exists, then both x and y are regarded as
** percentage... :( Notice that pos and focus should consider border
** since border doesn't auto adjust and won't change based on aspect
** ratio.
** e.g. (...) means default value is passed to the attribute, which can be
*omitted.
** <tag pos="30 20" width="128" height="64" focus="64 32" float="true"
*left="false" (top="true")>
** <tag (pos="0 0") width="256" height="64" (focus="0 0") float="true"
*left="false" border="4">
*/

/******************************************************************************
 * Cell::LoadProperty -- Load property of a cell and make adjustments.        *
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
 *   2022/05/12 Tony : Created.                                               *
 *============================================================================*/
void Cell::LoadProperty(XMLElement* node)
{
    _LoadProperty(node);
    _AdjustProperty(node);

    m_cellChanged = true;
}

/******************************************************************************
 * Cell::_LoadProperty -- Only load the property of Cell.                     *
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
 *   2022/05/12 Tony : Created.                                               *
 *============================================================================*/
void Cell::_LoadProperty(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;

    _PARSE("width", m_width, name, 0);
    _PARSE("height", m_height, name, 0);

    _PARSE_PRIVATE("pos", m_coord, name, ParseCoord);
    _PARSE_PRIVATE("focus", m_focus, name, ParseCoord);
}

/******************************************************************************
 * Cell::_AdjustProperty -- Only adjust properties of Cell.                   *
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
 *   2022/05/12 Tony : Created.                                               *
 *============================================================================*/
void Cell::_AdjustProperty(XMLElement* node)
{
    m_width = static_cast<int>(m_width * deviceInfo.aspectRatio);
    m_height = static_cast<int>(m_height * deviceInfo.aspectRatio);
    m_focus *= deviceInfo.aspectRatio;
}

/******************************************************************************
 * RectCell::_LoadProperty -- Load property of a rectangle.                   *
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
 *   2022/05/12 Tony : Created.                                               *
 *============================================================================*/
void RectCell::_LoadProperty(XMLElement* node)
{
    Cell::_LoadProperty(node);
}

/******************************************************************************
 * RectCell::_AdjustProperty -- Adjust to fit screen.                         *
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
 *   2022/05/12 Tony : Created.                                               *
 *============================================================================*/
void RectCell::_AdjustProperty(XMLElement* node)
{
    Cell::_AdjustProperty(node);
}

/******************************************************************************
 * RoundRectCell::_LoadProperty -- Load property.                             *
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
 *   2022/05/12 Tony : Created.                                               *
 *============================================================================*/
void RoundRectCell::_LoadProperty(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;

    RectCell::_LoadProperty(node);
    _PARSE("radius", m_radius, name, 0);
}

/******************************************************************************
 * RoundRectCell::_AdjustProperty -- Adjust to fit screen.                    *
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
 *   2022/05/12 Tony : Created.                                               *
 *============================================================================*/
void RoundRectCell::_AdjustProperty(XMLElement* node)
{
    RectCell::_AdjustProperty(node);
    m_radius = static_cast<int>(m_radius * deviceInfo.aspectRatio);
}

/******************************************************************************
 * CircleCell::_LoadProperty -- Load property.                                *
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
 *   2022/05/12 Tony : Created.                                               *
 *============================================================================*/
void CircleCell::_LoadProperty(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;

    _PARSE("radius", m_radius, name, m_radius);

    _PARSE_PRIVATE("pos", m_coord, name, ParseCoord);
    _PARSE_PRIVATE("focus", m_focus, name, ParseCoord);
}

/******************************************************************************
 * CircleCell::_AdjustProperty -- Adjust...                                   *
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
 *   2022/05/12 Tony : Created.                                               *
 *============================================================================*/
void CircleCell::_AdjustProperty(XMLElement* node)
{
    Cell::_AdjustProperty(node);
    m_radius = static_cast<int>(m_radius * deviceInfo.aspectRatio);
}
