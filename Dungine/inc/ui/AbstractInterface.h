/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : AbstractInterface.h                       *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 1, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   This file provides base class of interface.                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _ABSTRACT_INTERFACE_H_
#define _ABSTRACT_INTERFACE_H_

#include <stack>
#include <string>
#include <functional>
#include <unordered_map>

#include "../common/Common.h"

#include "../template/Singleton.h"

#include "../device/Sound.h"

#include "Event.h"


class XMLElement;
class Application;
class AbstractWidget;
class WidgetManager;

/********************************************************************
** Base class of all interfaces. All interfaces must link to the
** manager, or transition between then will fail.
** Notice that this is only a base class, which only provide basic
** methods as default. They should be overrided.
*/
class AbstractInterface
{
public:
	AbstractInterface() : m_pApp(nullptr), m_isActive(false), m_pWidgetManager(nullptr),
		m_pSupIntf(nullptr), m_pSubIntf(nullptr) {}
	virtual ~AbstractInterface();

	const std::string& Name() const { return m_name; }

	/*
	** Well, it is tricky that sometimes, when we launch a new interface, we
	** do not want the current interface to be terminated. So... Launch will
	** terminate the current one, while Attatch will keep the current one. :)
	** Notice that terminate doesn't care about the next interface, however,
	** sometimes the next interface may not be assigned, which means detatch
	** is needed.
	**
	** Excution order:
	**     New: Launch -> _Initialize                                    -> OnEnter
	**     Cur:                       -> Terminate -> OnExit -> _Destroy
	** Manager:                              ^          ^                      ^
	*/
	virtual void Launch();
	virtual void Attach();
	virtual void Terminate();
	virtual void Detach();

	virtual bool Load(XMLElement* node);

	virtual void OnEnter() {}
	virtual void OnExit() {}

	virtual void Update(Event* evnt);
	virtual void Draw();
	virtual void Draw(IMAGE* pDestImage);

	void SetSupInterface(AbstractInterface* intf) { m_pSupIntf = intf; }
	void ClearSupInterface() { m_pSupIntf = nullptr; }
	void SetSubInterface(AbstractInterface* intf)
	{
		intf->SetSupInterface(this);
		m_pSubIntf = intf;
	}
	void ClearSubInterface()
	{
		if (m_pSubIntf)
		{
			m_pSubIntf->ClearSupInterface();
			m_pSubIntf = nullptr;
		}
	}
	AbstractInterface* GetSupInterface() { return m_pSupIntf; }
	AbstractInterface* GetSubInterface() { return m_pSubIntf; }

protected:
	virtual void _TransitIn() {}
	virtual void _TransitOut() {}

	virtual void _Initialize() {}
	virtual void _Destroy() {}

public:
	bool IsActive() const { return m_isActive; }

	void SetManager(Application* pApp) { m_pApp = pApp; }
	Application* GetManager() { return m_pApp; }

	void SetWidgetManager(WidgetManager* pWidgetManager) { m_pWidgetManager = pWidgetManager; }
	WidgetManager* GetWidgetManager() { return m_pWidgetManager; }
	AbstractWidget* GetWidget(const std::string& name);

protected:
	std::string m_name;
	Application* m_pApp;
	
	WidgetManager* m_pWidgetManager;

	/*
	** This sub intf won't be added to active interface pool
	** in application.
	*/
	AbstractInterface* m_pSupIntf;
	AbstractInterface* m_pSubIntf;

	/*
	** Whether the interface is active or not. Parent interface
	** will remain active if sub-interface activated.
	*/
	bool m_isActive;
};

template<typename IntfType>
std::function<void(void)> GetLauncher(IntfType* intf)
{
	return std::bind(&IntfType::Launch, intf);
}

template<typename IntfType>
std::function<void(void)> GetAttacher(IntfType* intf)
{
	return std::bind(&IntfType::Attatch, intf);
}

template<typename IntfType>
std::function<void(void)> GetTerminator(IntfType* intf)
{
	return std::bind(&IntfType::Terminate, intf);
}

template<typename IntfType>
std::function<void(void)> GetDetacher(IntfType* intf)
{
	return std::bind(&IntfType::Detach, intf);
}


AbstractInterface* LoadInterface(XMLElement* node);

#endif