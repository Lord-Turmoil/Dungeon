/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Event.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 15, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For user's mouse and keyboard events.                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/ui/Event.h"

/********************************************************************
** This array keeps all keys that need check.
*/
static int ACTIVE_KEY_LIST[VIRTUAL_KEY_NUM];
static int ACTIVE_KEY_NUM;

/******************************************************************************
 * Event::PeekEvent -- Peek all user events.                                  *
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
 *   2022/05/25 Tony : Created.                                               *
 *   2022/06/28	Tony : while should be used when peek mouse message.          *
 *============================================================================*/
void Event::PeekEvent()
{
    // Window message.
    ExMessage msg;
    while (peekmessage(&msg, EX_WINDOW | EX_MOUSE))
    {
        if (msg.message == WM_ACTIVATE)
        {
            if (LOWORD(msg.wParam) == WA_INACTIVE)
            {
                m_isFocus = false;
            }
            else
            {
                m_isFocus = true;
            }
            FlushEvent();
        }
    }

    if (!m_isFocus)
    {
        return;
    }

    // keyboard message
    for (int i = 0; i < ACTIVE_KEY_NUM; i++)
    {
        int key = ACTIVE_KEY_LIST[i];
        if (_KEY_DOWN(key)) // If the key is already down
        {
            if (m_instantKey[key])
            {
                m_sluggishKey[key] = false;
            }
            else
            {
                m_instantKey[key] = m_sluggishKey[key] = true;
            }
        }
        else
        {
            m_instantKey[key] = m_sluggishKey[key] = false;
        }
    }

    // Mouse message.
    if (_KEY_DOWN(VK_LBUTTON))
    {
        if (m_instantKey[VK_LBUTTON])
        {
            m_sluggishKey[VK_LBUTTON] = false;
            m_mouseDown = false;
        }
        else
        {
            m_instantKey[VK_LBUTTON] = m_sluggishKey[VK_LBUTTON] = true;
            m_mouseDown = true;
        }
        m_mouseUp = false;
    }
    else
    {
        if (m_instantKey[VK_LBUTTON] && !m_mouseUp)
        {
            m_mouseUp = true;
        }
        else
        {
            m_mouseUp = false;
        }
        m_mouseDown = false;
        m_instantKey[VK_LBUTTON] = m_sluggishKey[VK_LBUTTON] = false;
    }

    POINT point;
    GetCursorPos(&point);
    ScreenToClient(GetHWnd(), &point);
    m_mouse.x = static_cast<int>(point.x);
    m_mouse.y = static_cast<int>(point.y);
}

/******************************************************************************
 * Event::FlushEvent -- Clear all events.                                     *
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
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
void Event::FlushEvent()
{
    for (int i = 0; i < ACTIVE_KEY_NUM; i++)
    {
        m_instantKey[ACTIVE_KEY_LIST[i]] = m_sluggishKey[ACTIVE_KEY_LIST[i]] = false;
    }
    m_instantKey[VK_LBUTTON] = m_sluggishKey[VK_LBUTTON] = false;

    m_mouseDown = m_mouseUp = false;
}

/******************************************************************************
 * Event::Event -- Constructor of object.                                     *
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
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
Event::Event() : m_isFocus(true)
{
    ACTIVE_KEY_NUM = 0;
    ACTIVE_KEY_LIST[ACTIVE_KEY_NUM++] = VK_ESCAPE;
    ACTIVE_KEY_LIST[ACTIVE_KEY_NUM++] = VK_CONTROL;
    ACTIVE_KEY_LIST[ACTIVE_KEY_NUM++] = VK_SPACE;
    for (int key = VK_A; key <= VK_Z; key++)
    {
        ACTIVE_KEY_LIST[ACTIVE_KEY_NUM++] = key;
    }

    FlushEvent();
}
