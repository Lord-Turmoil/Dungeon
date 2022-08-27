/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : WidgetManager.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 17, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Well, the widget manager.                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _WIDGET_MANAGER_H_
#define _WIDGET_MANAGER_H_

#include <map>
#include <string>

#include "../utility/tinyxml.h"

class Event;
class AbstractWidget;

/********************************************************************
** The manager of all widgets in an interface. Each interface has a
** different widget manager.
*/
class WidgetManager
{
public:
	WidgetManager();
	virtual ~WidgetManager();

	void Update(Event* evnt = nullptr);
	void Draw();
	bool Load(XMLElement* node);

	/*
	** Here, we use a map to store all widgets. So we should
	** provide a key and a value.
	*/
	AbstractWidget* AddWidget(AbstractWidget* widget);

	/*
	** Be aware that GetWidget will check if the widget exists
	** or not, but [] won't.
	*/
	AbstractWidget* GetWidget(const std::string& name);
	AbstractWidget* operator[](const std::string& name)
	{
		return m_widgets[name];
	}

	/*
	** Common methods for transition.
	*/
	void ResetTransition();

private:
	/*
	** widgtePool stores all widgets, and since widgets hold
	** their own layer and is handed to Device to draw, there
	** is no need for a widget queue to store sorted widgets.
	*/
	std::map<std::string, AbstractWidget*> m_widgets;
};

#endif