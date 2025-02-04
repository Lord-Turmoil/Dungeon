/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Trigger.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 9, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Trigger of widgets.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/ui/Trigger.h"
#include "../../inc/ui/Event.h"

#include "../../inc/device/Explorer.h"

#include "../../inc/utility/Auxilliary.h"
#include "../../inc/utility/Parser.h"
#include "../../inc/utility/Straw.h"

/******************************************************************************
 * Trigger::Load -- Load trigger's basic property.                            *
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
 *   20--/--/-- Tony : Created.                                               *
 *============================================================================*/
bool Trigger::Load(XMLElement* node)
{
    /*
    **	<trigger type="" vkey="0x41" ...>
    */

    const char* name = node->Name();

    _CHECK_TAG("Trigger");
    if (Logger::Error())
    {
        return false;
    }

    const char* attr;

    m_vKey = VK_NONE;
    attr = node->Attribute("vkey");
    if (attr)
    {
        if (sscanf_s(attr, "%x", &m_vKey) != 1)
        {
            LOG_ERROR(INVALID_ATTRIBUTE, "vkey", name);
        }
    }
    else
    {
        LOG_EXTRA_WARNING(MISSING_ATTRIBUTE, "vkey", name);
    }

    _RETURN_STATE();
}

/******************************************************************************
 * KeyboardTrigger::Detect -- Detect keyboard event.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  Return whether triggered.                                         *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/15 Tony : Created.                                               *
 *============================================================================*/
TriggerValue KeyboardTrigger::Detect(Event& evnt)
{
    if ((m_vKey != VK_NONE) && evnt.Sluggish(m_vKey))
    {
        return TriggerValue::TV_KEYDOWN;
    }
    else
    {
        return TriggerValue::TV_NONE;
    }
}

/******************************************************************************
 * KeyboardTrigger::Load -- Load keyboard trigger.                            *
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
 *   2022/05/15 Tony : Created.                                               *
 *============================================================================*/
bool KeyboardTrigger::Load(XMLElement* node)
{
    /*
    **	<trigger type="" vkey="0x41" ...>
    */
    const char* name = node->Name();
    const char* attr;

    _CHECK_TYPE("key");
    _RETURN_IF_ERROR();

    Trigger::Load(node);

    _RETURN_STATE();
}

/******************************************************************************
 * RectTrigger::Detect -- Detect event.                                       *
 *                                                                            *
 *    Every trigger can response to one keyboard event as a substitution for  *
 *    mouse event.                                                            *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  Return whether triggered.                                         *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
TriggerValue RectTrigger::Detect(Event& evnt)
{
    if (m_vKey != VK_NONE)
    {
        if (evnt.Sluggish(m_vKey))
        {
            return TriggerValue::TV_KEYDOWN;
        }
    }

    if (InRectangle(m_range, evnt.Mouse()))
    {
        if (evnt.MouseUp())
        {
            return TriggerValue::TV_CLICK;
        }
        else if (evnt.MouseDown())
        {
            return TriggerValue::TV_PRESS;
        }
        else
        {
            return TriggerValue::TV_HOVER;
        }
    }
    if (evnt.MouseUp())
    {
        return TriggerValue::TV_RELEASE;
    }
    else if (evnt.MouseDown())
    {
        return TriggerValue::TV_HOLD;
    }

    return TriggerValue::TV_MOVE;
}

/******************************************************************************
 * RectTrigger::Update -- Update rectangle trigger.                           *
 *                                                                            *
 *    Mainly update the range of the trigger. It can be further optimized     *
 *    since there's no need to update if only the coordinate or the focus of  *
 *    the cell is changed.                                                    *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
void RectTrigger::Update()
{
    m_range.pos = m_coord - m_focus;
    if (m_cellChanged)
    {
        m_range.width = m_width;
        m_range.height = m_height;
        m_cellChanged = false;
    }
}

/******************************************************************************
 * RectTrigger::Load -- Load a rectangle trigger.                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
bool RectTrigger::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;

    _CHECK_TYPE("rect");
    _RETURN_IF_ERROR();

    LoadProperty(node);
    Trigger::Load(node);

    m_range.width = m_width;
    m_range.height = m_height;
    Update();

    _RETURN_STATE();
}

/******************************************************************************
 * RoundRectTrigger::Detect -- Detect event as round rectangle trigger.       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  Return whether triggered.                                         *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
TriggerValue RoundRectTrigger::Detect(Event& evnt)
{
    if (m_vKey != VK_NONE)
    {
        if (evnt.Sluggish(m_vKey))
        {
            return TriggerValue::TV_KEYDOWN;
        }
    }

    if (InRoundRect(m_range, m_radius, evnt.Mouse()))
    {
        if (evnt.MouseUp())
        {
            return TriggerValue::TV_CLICK;
        }
        else if (evnt.MouseDown())
        {
            return TriggerValue::TV_PRESS;
        }
        else
        {
            return TriggerValue::TV_HOVER;
        }
    }
    if (evnt.MouseUp())
    {
        return TriggerValue::TV_RELEASE;
    }
    else if (evnt.MouseDown())
    {
        return TriggerValue::TV_HOLD;
    }

    return TriggerValue::TV_MOVE;
}

/******************************************************************************
 * RoundRectTrigger::Update -- update round rectangle trigger.                *
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
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
void RoundRectTrigger::Update()
{
    m_range.pos = m_coord - m_focus;
    if (m_cellChanged)
    {
        m_range.width = m_width;
        m_range.height = m_height;
        m_cellChanged = false;
    }
}

/******************************************************************************
 * RoundRectTrigger::Load -- Load round rectangle trigger.                    *
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
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
bool RoundRectTrigger::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;

    _CHECK_TYPE("roundrect");
    _RETURN_IF_ERROR();

    LoadProperty(node);
    Trigger::Load(node);

    m_range.width = m_width;
    m_range.height = m_height;
    Update();

    _RETURN_STATE();
}

/******************************************************************************
 * CircleTrigger::Detect -- Detect event as circle trigger.                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  Return whether triggered.                                         *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
TriggerValue CircleTrigger::Detect(Event& evnt)
{
    if (m_vKey != VK_NONE)
    {
        if (evnt.Sluggish(m_vKey))
        {
            return TriggerValue::TV_KEYDOWN;
        }
    }

    if (InCircle(m_center, m_radius, evnt.Mouse()))
    {
        if (evnt.MouseUp())
        {
            return TriggerValue::TV_CLICK;
        }
        else if (evnt.MouseDown())
        {
            return TriggerValue::TV_PRESS;
        }
        else
        {
            return TriggerValue::TV_HOVER;
        }
    }
    if (evnt.MouseUp())
    {
        return TriggerValue::TV_RELEASE;
    }
    else if (evnt.MouseDown())
    {
        return TriggerValue::TV_HOLD;
    }

    return TriggerValue::TV_MOVE;
}

/******************************************************************************
 * CircleTrigger::Update -- Update circle tirgger.                            *
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
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
void CircleTrigger::Update()
{
    m_center = m_coord - m_focus;
    m_center.x += m_radius;
    m_center.y += m_radius;
}

/******************************************************************************
 * CircleTrigger::Load -- Load circle trigger.                                *
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
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
bool CircleTrigger::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;

    _CHECK_TYPE("circle");
    _RETURN_IF_ERROR();

    LoadProperty(node);
    Trigger::Load(node);

    Update();

    _RETURN_STATE();
}

/******************************************************************************
 * MaskTrigger::Detect -- Detect event as mask trigger.                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  Return whether triggered or not.                                  *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
TriggerValue MaskTrigger::Detect(Event& evnt)
{
    if (m_vKey != VK_NONE)
    {
        if (evnt.Sluggish(m_vKey))
        {
            return TriggerValue::TV_KEYDOWN;
        }
    }

    if (InRectangle(m_range, evnt.Mouse()))
    {
        DWORD* buffer = GetImageBuffer(&m_mask);

        if (buffer[(evnt.MouseY() - m_range.y) * m_width + (evnt.MouseX() - m_range.x)] != TRANSPARENT_COLOR)
        {
            if (evnt.MouseUp())
            {
                return TriggerValue::TV_CLICK;
            }
            else if (evnt.MouseDown())
            {
                return TriggerValue::TV_PRESS;
            }
            else
            {
                return TriggerValue::TV_HOVER;
            }
        }
    }
    if (evnt.MouseUp())
    {
        return TriggerValue::TV_RELEASE;
    }
    else if (evnt.MouseDown())
    {
        return TriggerValue::TV_HOLD;
    }

    return TriggerValue::TV_MOVE;
}

/******************************************************************************
 * MaskTrigger::Update -- Update mask trigger.                                *
 *                                                                            *
 *    For now, mask trigger has nothing to update.                            *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
void MaskTrigger::Update()
{
    m_range.pos = m_coord - m_focus;
    if (m_cellChanged)
    {
        m_range.width = m_width;
        m_range.height = m_height;
        m_cellChanged = false;
    }
}

/******************************************************************************
 * MaskTrigger::Load -- Load mask trigger.                                    *
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
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
bool MaskTrigger::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;

    _CHECK_TYPE("mask");
    _RETURN_IF_ERROR();

    LoadProperty(node);
    Trigger::Load(node);

    Update();

    // Load mask image.
    Coordinate origin;
    Coordinate size;
    const char* src;

    _PARSE_PRIVATE("origin", origin, name, ParseCoord);
    _PARSE_PRIVATE("size", size, name, ParseCoord);

    src = node->Attribute("src");
    if (!src)
    {
        LOG_ERROR(MISSING_ATTRIBUTE, "src", name);
        return false;
    }

    ImageResource* res = LoadResource<ImageResource>(src);
    if (!res)
    {
        LOG_ERROR(INVALID_RESOURCE_ID, src);
        return false;
    }

    FetchImage(&m_mask, res->GetResource(), m_width, m_height, origin.x, origin.y, size.x, size.y);

    _RETURN_STATE();
}

/******************************************************************************
 * LoadTrigger -- Load a trigger.                                             *
 *                                                                            *
 *    This uses a simple factory pattern.                                     *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
Trigger* LoadTrigger(XMLElement* node)
{
    if (!node)
    {
        return nullptr;
    }

    const char* name = node->Name();
    if (_STR_DIFF(name, "Trigger"))
    {
        LOG_ERROR(TAG_MISMATCH, name, "Trigger");
        return nullptr;
    }

    const char* type;
    type = node->Attribute("type");
    if (!type)
    {
        LOG_ERROR(MISSING_ATTRIBUTE, "type", name);
        return nullptr;
    }

    Trigger* rv = nullptr;
    if (_STR_SAME(type, "rect"))
    {
        rv = new RectTrigger();
    }
    else if (_STR_SAME(type, "roundrect"))
    {
        rv = new RoundRectTrigger();
    }
    else if (_STR_SAME(type, "circle"))
    {
        rv = new CircleTrigger();
    }
    else if (_STR_SAME(type, "mask"))
    {
        rv = new MaskTrigger();
    }
    else if (_STR_SAME(type, "key"))
    {
        rv = new KeyboardTrigger();
    }

    if (rv)
    {
        rv->Load(node);
    }
    else
    {
        LOG_ERROR(UNKNOWN_TYPE, type);
    }

    return rv;
}
