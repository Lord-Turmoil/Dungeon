/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Interface.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 21, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For all game interfaces.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/ui/Interface.h"
#include "../../inc/ui/MenuInterface.h"
#include "../../inc/ui/GameInterface.h"
#include "../../inc/ui/CreditsInterface.h"
#include "../../inc/ui/VersionInterface.h"


/******************************************************************************
 * PlainInterface::Load -- Load plain interface.                              *
 *                                                                            *
 *    This is the basic property of interface.                                *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
bool PlainInterface::Load(XMLElement* node)
{
/*
**	<PlainInterface name="" in-time="" out-time="">
**		<Widget>...</Widget>
**	</PlainInterface>
*/
	const char* name = node->Name();
	const char* attr;

	AbstractInterface::Load(node);

	_PARSE("in-time", m_inTime, name, 0L);
	_PARSE("out-time", m_outTime, name, 0L);

	_RETURN_STATE();
}


/******************************************************************************
 * PlainInterface::Update -- Update plain interface.                          *
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
 *   2022/07/30 Tony : Created.                                               *
 *============================================================================*/
void PlainInterface::Update(Event* evnt)
{
	if (m_pSubIntf)
		m_pSubIntf->Update(evnt);
	else
	{
		m_pWidgetManager->Update(evnt);
		if (evnt)
			_ProcessInput(*evnt);
	}
}


/******************************************************************************
 * PlainInterface::OnEnter -- Default on enter effect.                        *
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
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
void PlainInterface::OnEnter()
{
	_TransitIn();
}


/******************************************************************************
 * PlainInterface::OnExit -- Default on exit effect.                          *
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
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
void PlainInterface::OnExit()
{
	_TransitOut();
}


/******************************************************************************
 * PlainInterface::_ProcessInput -- Process input.                            *
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
void PlainInterface::_ProcessInput(Event& evnt)
{
	if (evnt.SluggishCtrl(CMD_PRTSC))
		Device::GetInstance()->PrintScreen();
}


/******************************************************************************
 * PlainInterface::_TransitIn -- Transit in effect.                           *
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
 *   2022/07/18 Tony : Created.                                               *
 *============================================================================*/
void PlainInterface::_TransitIn()
{
	if (m_inTime > 0)
		Transitor::FadeIn(this, m_inTime);
}


/******************************************************************************
 * PlainInterface::_TransitOut -- Transit out effect.                         *
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
 *   2022/07/18 Tony : Created.                                               *
 *============================================================================*/
void PlainInterface::_TransitOut()
{
	if (m_outTime > 0)
		Transitor::FadeOut(this, m_outTime);
}



/******************************************************************************
 * TimeInterface::Load -- Load time interface.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/21 Tony : Created.                                               *
 *============================================================================*/
bool TimeInterface::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	PlainInterface::Load(node);

	_PARSE("duration", m_duration, name, 0L);

	_RETURN_STATE();
}


/******************************************************************************
 * TimeInterface::Update -- Update time interface.                            *
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
 *   2022/07/18 Tony : Created.                                               *
 *============================================================================*/
void TimeInterface::Update(Event* evnt)
{
	m_pWidgetManager->Update(evnt);
	
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > m_duration)
	{
		_SelfTerminate();
		return;
	}

	if (evnt)
		_ProcessInput(*evnt);
}


/******************************************************************************
 * TimeInterface::_Initialize -- Initialize time interface.                   *
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
 *   2022/08/12 Tony : Created.                                               *
 *============================================================================*/
void TimeInterface::_Initialize()
{
	m_elapsedTime = 0L;
}


/******************************************************************************
 * TimeInterface::_SelfTerminate -- Execute self temination.                  *
 *                                                                            *
 *    This is going to be overrided.                                          *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/18 Tony : Created.                                               *
 *============================================================================*/
void TimeInterface::_SelfTerminate()
{
	Detach();
}



/******************************************************************************
 * LoadInterface -- Load interface from xml.                                  *
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
 *   2022/06/22 Tony : Created.                                               *
 *============================================================================*/
AbstractInterface* LoadInterface(XMLElement* node)
{
	/*
	**	<IntfName name="">
	**		<Widget>
	**		</Widget>
	**	</IntfName>
	*/
	const char* name = node->Name();

	AbstractInterface* intf = nullptr;
	if (_STR_SAME(name, "Logo"))
		intf = new LogoInterface();
	else if (_STR_SAME(name, "Main"))
		intf = new MainInterface();
	else if (_STR_SAME(name, "Select"))
		intf = new SelectInterface();
	else if (_STR_SAME(name, "Setting"))
		intf = new SettingInterface();
	else if (_STR_SAME(name, "Help"))
		intf = new HelpInterface();
	else if (_STR_SAME(name, "Game"))
		intf = new GameInterface();
	else if (_STR_SAME(name, "Pause"))
		intf = new PauseInterface();
	else if (_STR_SAME(name, "Quit"))
		intf = new QuitInterface();
	else if (_STR_SAME(name, "About"))
		intf = new AboutInterface();
	else if (_STR_SAME(name, "Credits"))
		intf = new CreditsInterface();
	else if (_STR_SAME(name, "Version"))
		intf = new VersionInterface();
	else if (_STR_SAME(name, "Victory"))
		intf = new VictoryInterface();
	else if (_STR_SAME(name, "Time"))
		intf = new TimeInterface();
	else
		LOG_ERROR(INVALID_ATTRIBUTE, "type", name);

	if (intf)
	{
		intf->SetWidgetManager(new WidgetManager());
		if (!intf->Load(node))
		{
			delete intf;
			return nullptr;
		}
	}
	
	return intf;
}