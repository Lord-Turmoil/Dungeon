/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Application.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 17, 2022                             *
 *                                                                            *
 *                    Last Update : August 12, 2022                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Dungine's main loop is encapsulated in the interface mangager, which is  *
 *   also called Application.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/device/Timer.h"
#include "../../inc/device/Device.h"
#include "../../inc/device/Speaker.h"

#include "../../inc/ui/Application.h"
#include "../../inc/ui/AbstractInterface.h"
#include "../../inc/ui/Transitor.h"

#include "../../inc/utility/xml.h"


/******************************************************************************
 * Application::Application -- Constructor of the object.                     *
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
 *   2022/06/02 Tony : Created.                                               *
 *============================================================================*/
Application::Application() :
	m_pStartIntf(nullptr),
	m_pCurIntf(nullptr),
	m_isRunning(false),
	m_delay(DUNGINE_DELAY)
{
}


/******************************************************************************
 * Application::~Application -- Destructor of the object.                     *
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
 *   2022/06/02 Tony : Created.                                               *
 *============================================================================*/
Application::~Application()
{
	UnLoad();
}


/******************************************************************************
 * Application::Load -- Load all interfaces.                                  *
 *                                                                            *
 *    Well, it is much easier to load it from a xml file than manully. Hmm... *
 *    In this version, UI interfaces can be loaded in seperate files.         *
 *                                                                            *
 * INPUT:   filename -- xml file name.                                        *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/02 Tony : Created.                                               *
 *============================================================================*/
bool Application::Load(const char* filename)
{
	/*
	**	<UI>
	**		<AbstractInterface name="">...</AbstractInterface>
	**		<AbstractInterface name="">...</AbstractInterface>
	**		<AbstractInterface name="">...</AbstractInterface>
	**	</UI>
	*/
	XMLFile doc;

	if (!doc.Load(filename))
	{
		LOG_ERROR(FAILED_TO_LOAD_UI_AT, filename);
		return false;
	}

	XMLElement* node = doc.GetRoot()->FirstChildElement();
	while (node)
	{
		if (!AddInterface(LoadInterface(node)))
		{
			LOG_ERROR(FAILED_TO_LOAD_UI_AT, filename);
			return false;
		}

		node = node->NextSiblingElement();
	}

	return true;
}


/******************************************************************************
 * Application::UnLoad -- Unload all interfaces.                              *
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
 *   2022/07/30 Tony : Created.                                               *
 *============================================================================*/
void Application::UnLoad()
{
	for (auto it = m_interfaces.begin(); it != m_interfaces.end(); it++)
		delete it->second;
	m_interfaces.clear();

	/*
	** 2022/08/03 TS:
	** Ahh... :(
	*/
	m_pStartIntf = nullptr;
}


/******************************************************************************
 * Application::Initialize -- Initialize the manager.                         *
 *                                                                            *
 *    This must be called before Run.                                         *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/02 Tony : Created.                                               *
 *============================================================================*/
bool Application::Initialize()
{
	if (!m_pStartIntf)
	{
		LOG_ERROR(MISSING_STARTUP);
		return false;
	}

	Transitor::Initialize();

	m_pCurIntf = nullptr;
	Launch(m_pStartIntf->Name());

	return true;
}


/******************************************************************************
 * Application::Run -- The main loop of the whole game.                       *
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
 *   2022/06/02 Tony : Created.                                               *
 *   2022/08/12 Tony : Add update of speaker.                                 *
 *============================================================================*/
void Application::Run()
{
	Timer* timer = Timer::GetInstance();
	Device* device = Device::GetInstance();
	Event* evnt = Event::GetInstance();
	Speaker* speaker = Speaker::GetInstance();

	m_isRunning = true;
	while (m_isRunning)
	{
		timer->Update();
		evnt->PeekEvent();

		m_pCurIntf->Update(evnt);
		if (!m_pCurIntf)
		{
			m_isRunning = false;
			break;
		}
		m_pCurIntf->Draw();
		device->Render();
		
		if (Logger::Error())
			break;

		speaker->Update();

		timer->Delay(m_delay);
	}
}


/******************************************************************************
 * Application::Launch -- Launch a new interface.                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name -- Name of the interface.                                    *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/21 Tony : Created.                                               *
 *============================================================================*/
void Application::Launch(const std::string& name)
{
	AbstractInterface* intf = GetInterface(name);

	if (!intf)
	{
		LOG_ERROR(MISSING_INTERFACE, name.c_str());
		return;
	}

	if (m_pCurIntf)
		m_pCurIntf->Terminate();	// OnExit will be performed.
	m_pCurIntf = intf;
	m_pCurIntf->OnEnter();

	m_activeInterfaces.push(m_pCurIntf);
}


/******************************************************************************
 * Application::LaunchSubInterface -- Launch a sub interface.                 *
 *                                                                            *
 *    This will launch a new interface, but not terminate the current one.    *
 *                                                                            *
 * INPUT:   name -- Name of the interface.                                    *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void Application::Attatch(const std::string& name)
{
	AbstractInterface* intf = GetInterface(name);

	if (!intf)
	{
		LOG_ERROR(MISSING_INTERFACE, name.c_str());
		return;
	}

	if (m_pCurIntf)
		m_pCurIntf->OnExit();
	m_pCurIntf = intf;
	m_pCurIntf->OnEnter();

	m_activeInterfaces.push(m_pCurIntf);
}


/******************************************************************************
 * Application::Terminate -- Terminate current interface.                     *
 *                                                                            *
 *    This will terminate the current interface. It doesn't care about the    *
 *    next interface.                                                         *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void Application::Terminate()
{
	if (!m_pCurIntf)
		return;

	m_pCurIntf->OnExit();
	m_pCurIntf = nullptr;

	m_activeInterfaces.pop();
}


/******************************************************************************
 * Application::Detach -- Detach current interface.                           *
 *                                                                            *
 *    This will roll back to the previous interface and do all the transition.*
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void Application::Detach()
{
	Terminate();	// OnExit performed.
	if (m_activeInterfaces.empty())
	{
		// Oh? This shouldn't happen, but can be omitted?
		LOG_WARNING("Nothing to detach");
	}
	else
	{
		m_pCurIntf = m_activeInterfaces.top();
		m_pCurIntf->OnEnter();
	}
}


/******************************************************************************
 * Application::AddInterface -- Add interface to the manager.                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   intf                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/02 Tony : Created.                                               *
 *============================================================================*/
AbstractInterface* Application::AddInterface(AbstractInterface* intf)
{
	if (!intf)
		return nullptr;

	const std::string& name = intf->Name();
	auto it = m_interfaces.find(name);
	if (it == m_interfaces.end())
	{
		intf->SetManager(this);
		m_interfaces.emplace(name, intf);

		if (!m_pStartIntf)
			m_pStartIntf = intf;
	}
	else
	{
		LOG_ERROR(NAME_CONFLICT, "interface", name.c_str());
		delete it->second;
		it->second = intf;
	}

	return intf;
}


/******************************************************************************
 * Application::GetInterface -- Get an interface in the manager.              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name                                                              *
 *                                                                            *
 * OUTPUT:  Return the required interface. Return nullptr if doen't exist.    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/02 Tony : Created.                                               *
 *============================================================================*/
AbstractInterface* Application::GetInterface(const std::string& name)
{
	auto it = m_interfaces.find(name);

	if (it == m_interfaces.end())
		return nullptr;

	return it->second;
}