/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : VersionInterface.h                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 26, 2022                           *
 *                                                                            *
 *                    Last Update : November 26, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Version interface.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _VERSION_INTERFACE_H_
#define _VERSION_INTERFACE_H_

#include <vector>

#include "Interface.h"


/********************************************************************
** Version interface have to support multiple version pages.
*/
class VersionInterface final : public PlainInterface
{
public:
	VersionInterface();
	virtual ~VersionInterface();

	virtual bool Load(XMLElement* node);
	virtual void Update(Event* evnt);
	virtual void Draw();
	virtual void Draw(IMAGE* pDestImage);

	virtual void AddEvents();

protected:
	virtual void _Initialize();

private:
	void _UpdatePage(Event* evnt);
	void _DrawPage();
	void _PageUp();
	void _PageDown();
	void _UpdateControl();

	/*
	** 2022/11/26 TS:
	** Show latest version when opened.
	*/
	void _OnRewind();
	static void _OnCredits();

	std::vector<WidgetManager*> m_pages;
	std::vector<WidgetManager*>::iterator m_curPage;
};

#endif