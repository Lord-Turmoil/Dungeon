/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Camera.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 28, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   The camera in game.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/game/Camera.h"
#include "../../inc/device/Device.h"
#include "../../inc/utility/Auxilliary.h"

/******************************************************************************
 * Camera::Initialize -- Initialize camera.                                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   rect -- The target area camera is going to draw.                  *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/28 Tony : Created.                                               *
 *============================================================================*/
void Camera::Initialize(const Rect& rect)
{
    m_filmArea = m_area = rect;
    m_device = Device::GetInstance();
}

/******************************************************************************
 * Camera::Update -- Update camera's position.                                *
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
 *   2022/06/28 Tony : Created.                                               *
 *============================================================================*/
void Camera::Update()
{
    if (m_target != m_center)
    {
        Coordinate offset = BlendValue(m_target, m_center, m_lag) - m_center;

        m_center += offset;
        m_area.pos += offset;
        m_offset -= offset;
    }

    m_updated = true;
}

/******************************************************************************
 * Camera::Follow -- Set the target to follow.                                *
 *                                                                            *
 *    This has ease effect.                                                   *
 *                                                                            *
 * INPUT:   target -- The target center of the camera.                        *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/28 Tony : Created.                                               *
 *============================================================================*/
void Camera::Follow(const Coordinate& target)
{
    m_target = target;
}

/******************************************************************************
 * Camera::Focus -- To fucus the camera immediately.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   target                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/28 Tony : Created.                                               *
 *============================================================================*/
void Camera::Focus(const Coordinate& target)
{
    m_target = m_center = target;
    m_area.pos.x = target.x - m_area.width / 2;
    m_area.pos.y = target.y - m_area.height / 2;
    m_offset = -m_area.pos;
}

/******************************************************************************
 * Camera::Capture -- Capture an object.                                      *
 *                                                                            *
 *    It seems that before actually adding an object to the vector, some extra*
 *    check can be done to prevent out-sighted ones. Since there are not many *
 *    objects, this is ignored.                                               *
 *                                                                            *
 * INPUT:   obj -- The object to capture.                                     *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/28 Tony : Created.                                               *
 *============================================================================*/
void Camera::Capture(GameObject* obj)
{
    if (obj->IsToDisplay())
    {
        Symbol* sym = obj->GetSymbol();

        if (m_updated)
        {
            sym->Translate(m_offset);
        }

        m_device->AddSymbol(sym);
    }
}

void Camera::Capture(Symbol* symbol)
{
    if (m_updated)
    {
        symbol->Translate(m_offset);
    }

    m_device->AddSymbol(symbol);
}

/******************************************************************************
 * Camera::InRange -- To check if an area is in sight.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   rect                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/28 Tony : Created.                                               *
 *============================================================================*/
bool Camera::InRange(const Rect& rect)
{
    return HasIntersect(m_area, rect);
}

/******************************************************************************
 * Camera::AbsoluteToRelative -- Convert coordinate.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   coord                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *============================================================================*/
Coordinate Camera::AbsoluteToRelative(const Coordinate& coord)
{
    return coord - m_offset;
}

/******************************************************************************
 * Camera::RelativeToAbsolute -- Convert coordinate.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   coord                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *============================================================================*/
Coordinate Camera::RelativeToAbsolute(const Coordinate& coord)
{
    return coord + m_offset;
}
