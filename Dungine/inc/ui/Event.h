/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Event.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 15, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For user's mouse and keyboard events.                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _EVENT_H_
#define _EVENT_H_

#include <Windows.h>

#include "../common/Common.h"
#include "../template/Singleton.h"


// Maximum virtual key is 256. Here only check part of them.
const int VIRTUAL_KEY_NUM = 128;


/********************************************************************
** Event receives all user's events. However, it only receives A-Z
** and left mouse button event. :)
*/
class Event : public SingletonHungry<Event>
{
	friend class SingletonHungry<Event>;
public:
	void PeekEvent();

	// This is deprecated since it won't peek mouse event.
	void PeekEvent(void (*handler)(bool*, bool*))
	{
		handler(m_instantKey, m_sluggishKey);
	}

	void FlushEvent();
	
	/*
	** Warning: These two doen't check for validation of vKey!
	*/
	bool Instant(int vKey) const { return m_instantKey[vKey]; }
	bool Sluggish(int vKey) const { return m_sluggishKey[vKey]; }
	bool InstantCtrl(int vKey) const { return m_instantKey[VK_CONTROL] && m_instantKey[vKey]; }
	bool SluggishCtrl(int vKey) const { return m_instantKey[VK_CONTROL] && m_sluggishKey[vKey]; }

	const Coordinate& Mouse() const { return m_mouse; }
	int MouseX() const { return m_mouse.x; }
	int MouseY() const { return m_mouse.y; }

	bool MouseDown() const { return m_mouseDown; }
	bool MouseUp() const { return m_mouseUp; }

	bool IsFocus() const { return m_isFocus; }

private:
	Event();
	~Event() {}

	bool m_instantKey[VIRTUAL_KEY_NUM];
	bool m_sluggishKey[VIRTUAL_KEY_NUM];
	Coordinate m_mouse;
	bool m_mouseDown : 1;
	bool m_mouseUp : 1;
	bool m_isFocus : 1;
};

#if 0
// Peek custom event, these are deprecated.
void PeekPlayerEvent(bool* instant, bool* sluggish);
void PeekGameEvent(bool* instant, bool* sluggish);
void PeekInterfaceEvent(bool* instant, bool* sluggish);
#endif	// #if 0


/********************************************************************
** Some additional defines to windows virtual keys.
*/
#ifndef VK_NONE
#define VK_NONE	-1
#endif

#define VK_A	0x41	// CMD_LEFT
#define VK_B	0x42	//
#define VK_C	0x43	// CMD_PRINTSCREEN
#define VK_D	0x44	// CMD_RIGHT
#define VK_E	0x45	// CMD_ACTIVE
#define VK_F	0x46	//
#define VK_G	0x47	//
#define VK_H	0x48	//
#define VK_I	0x49	//
#define VK_J	0x4A	//
#define VK_K	0x4B	//
#define VK_L	0x4C	//
#define VK_M	0x4D	//
#define VK_N	0x4E	//
#define VK_O	0x4F	// CMD_FULLSCREEN
#define VK_P	0x50	// CMD_PAUSE
#define VK_Q	0x51	// CMD_SWITCH
#define VK_R	0x52	// CMD_RESTART
#define VK_S	0x53	// CMD_DOWN
#define VK_T	0x54	//
#define VK_U	0x55	//
#define VK_V	0x56	//
#define VK_W	0x57	// CMD_UP
#define VK_X	0x58	//
#define VK_Y	0x59	//
#define VK_Z	0x5A	//

#define _KEY_DOWN(KEY) (GetAsyncKeyState(KEY) & 0x8000)

/********************************************************************
** User commands.
*/
#define CMD_NONE		VK_NONE	// No use?

// player commands
#define CMD_LEFT		VK_A
#define CMD_RIGHT		VK_D
#define CMD_UP			VK_W
#define CMD_DOWN		VK_S
#define CMD_FIRE		VK_LBUTTON
#define CMD_SWITCH		VK_Q	// switch weapon
#define CMD_SKILL		VK_SPACE
#define CMD_ACTIVATE	VK_E	// pick up weapon or open create

// game commands
#define CMD_RESTART		VK_R
#define CMD_PAUSE		VK_P

// interface commands
#define CMD_PRTSC		VK_C		// with ctrl
#define CMD_QUIT		VK_ESCAPE

#endif