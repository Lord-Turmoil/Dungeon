/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : WidgetManager.cpp                         *
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

#include "../../inc/ui/Widget.h"
#include "../../inc/ui/WidgetManger.h"
#include "../../inc/ui/AbstractWidget.h"

#include "../../inc/utility/Parser.h"

 /******************************************************************************
  * WidgetManager::Constructor of the object.                                  *
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
WidgetManager::WidgetManager() {}


/******************************************************************************
 * WidgetManager::~WidgetManager -- Deconstructor of the object.              *
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
WidgetManager::~WidgetManager()
{
	for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
		delete it->second;
}


/******************************************************************************
 * WidgetManager::Update -- Update widget manager.                            *
 *                                                                            *
 *    When widget manager updates, it will update all widgets and widgets will*
 *    process input and update their transition at the same time.             *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
void WidgetManager::Update(Event* evnt)
{
	for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
		it->second->Update(evnt);
}


/******************************************************************************
 * WidgetManager::Draw -- Draw all widgets of the manager.                    *
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
void WidgetManager::Draw()
{
	for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
		it->second->Draw();
}


/******************************************************************************
 * WidgetManager::Load -- Load all widgets of a manager.                      *
 *                                                                            *
 *    Widget manager manages all widgets in an interface, so it will load all *
 *    widgets in an interface. It should be contained in <widget> tag. And    *
 *    when it adds widget, the widget loaded must have name attribute. If it  *
 *    is missing, globalState will be set to ST_ERROR, and this process will  *
 *    terminate.                                                              *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
bool WidgetManager::Load(XMLElement* node)
{
	if (!node)
		return false;

	const char* name = node->Name();
	XMLElement* tag;
	AbstractWidget* widget;

	StandardWidgetKit kit;
	tag = node->FirstChildElement();
	while (tag)
	{
		widget = kit.LoadWidget(tag);
		if (Logger::Error())
		{
			LOG_ERROR(CANNOT_LOAD, "Widget");
			return false;
		}
		AddWidget(widget);

		tag = tag->NextSiblingElement();
	}

	_RETURN_STATE();
}


/******************************************************************************
 * WidgetManager::AddWidget -- Add widget to the manager.                     *
 *                                                                            *
 *    Add a widget to the manager. Notice that the names of widgets MUST be   *
 *    unique, otherwise, the new widget will be deleted with a error logged.  *
 *                                                                            *
 * INPUT:   name   -- Name of the new widget.                                 *
 *          widget -- The widget itself.                                      *
 *                                                                            *
 * OUTPUT:  Return the newly added widget. If name conflict occurs, old widget*
 *          will be returned.                                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
AbstractWidget* WidgetManager::AddWidget(AbstractWidget* widget)
{
	if (!widget)
		return nullptr;

	std::string name = widget->Name();
	auto it = m_widgets.find(name);
	if (it == m_widgets.end())
	{
		widget->SetManager(this);
		m_widgets.emplace(name, widget);
	}
	else
	{
		LOG_ERROR(NAME_CONFLICT, "Widget", name.c_str());
		delete it->second;
		it->second = widget;
	}

	return widget;
}


/******************************************************************************
 * WidgetManager::GetWidget -- Get a widget in the manager.                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name -- The name of widget required.                              *
 *                                                                            *
 * OUTPUT:  Return the required widget. If doesn't exist, return nullptr.     *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
AbstractWidget* WidgetManager::GetWidget(const std::string& name)
{
	auto it = m_widgets.find(name);

	if (it == m_widgets.end())
		return nullptr;

	return it->second;
}


/******************************************************************************
 * WidgetManager::ResetTransition -- Reset transition of all widgets.         *
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
void WidgetManager::ResetTransition()
{
	for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
	{
		if (it->second->IsVisual())
			dynamic_cast<VisualWidget*>(it->second)->ResetTransition();
	}
}
