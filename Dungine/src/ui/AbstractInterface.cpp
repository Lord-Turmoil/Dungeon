/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : AbstractInterface.cpp                     *
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

#include "../../inc/common/Common.h"

#include "../../inc/utility/xml.h"
#include "../../inc/utility/Parser.h"
#include "../../inc/utility/Auxilliary.h"

#include "../../inc/device/Explorer.h"
#include "../../inc/device/Device.h"
#include "../../inc/device/Timer.h"

#include "../../inc/ui/Application.h"
#include "../../inc/ui/AbstractInterface.h"
#include "../../inc/ui/WidgetManger.h"


/******************************************************************************
 * AbstractInterface::~AbstractInterface -- Destructor of the object.         *
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
AbstractInterface::~AbstractInterface()
{
	_DELETE(m_pWidgetManager);
}

/******************************************************************************
 * AbstractInterface::Launch -- Launch interface.                             *
 *                                                                            *
 *    This will handle operation to the manager.                              *
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
void AbstractInterface::Launch()
{
	_Initialize();
	m_pApp->Launch(m_name);
}


/******************************************************************************
 * AbstractInterface::Attach -- Attach interface to the current one.          *
 *                                                                            *
 *    This will handle operation to the manager.                              *
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
void AbstractInterface::Attach()
{
	_Initialize();
	m_pApp->Attatch(m_name);
}


/******************************************************************************
 * AbstractInterface::Terminate -- Terminate interface.                       *
 *                                                                            *
 *    This requires that the interface must be the current one, which means   *
 *    that it is on the top of the stack.                                     *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  Mustn't be used on a sub interface.                             *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void AbstractInterface::Terminate()
{
	m_pApp->Terminate();
	ClearSubInterface();
	_Destroy();
}


/******************************************************************************
 * AbstractInterface::Detach -- Detach interface.                             *
 *                                                                            *
 *    Detach current inteface.                                                *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Mustn't be used on a sub interface.                               *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void AbstractInterface::Detach()
{
	m_pApp->Detach();
	ClearSubInterface();
	_Destroy();
}


/******************************************************************************
 * AbstractInterface::Load -- Load the basic property of interface.           *
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
 *   2022/06/02 Tony : Created.                                               *
 *============================================================================*/
bool AbstractInterface::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;
	XMLElement* tag;

	_PARSE_ESSENTIAL("name", m_name, name, "");
	if (Logger::Error())
		return false;

	tag = node->FirstChildElement("Widgets");
	if (tag)
	{
		m_pWidgetManager = new WidgetManager();
		m_pWidgetManager->Load(tag);
	}

	m_isActive = false;

	_RETURN_STATE();
}


/******************************************************************************
 * AbstractInterface::Update -- Update on each frame.                         *
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
 *   2022/06/01 Tony : Created.                                               *
 *============================================================================*/
void AbstractInterface::Update(Event* evnt)
{
	if (m_pSubIntf)
		m_pSubIntf->Update(evnt);
	else
		m_pWidgetManager->Update(evnt);
}


/******************************************************************************
 * AbstractInterface::Draw -- Draw the interface.                             *
 *                                                                            *
 *    This only provide default draw.                                         *
 *                                                                            *
 * INPUT:   pDestImage -- If assigned, it will draw on there.                 *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/01 Tony : Created.                                               *
 *============================================================================*/
void AbstractInterface::Draw()
{
	m_pWidgetManager->Draw();
	if (m_pSubIntf)
		m_pSubIntf->Draw();
}

void AbstractInterface::Draw(IMAGE* pDestImage)
{
	m_pWidgetManager->Draw();
	if (m_pSubIntf)
		m_pSubIntf->Draw();

	Device::GetInstance()->Render(pDestImage);
}


/******************************************************************************
 * AbstractInterface::GetWidget -- Get a widget in the interface.             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name -- The name of the widget.                                   *
 *                                                                            *
 * OUTPUT:  Return the required widget. Return nullptr if doesn't exist.      *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/01 Tony : Created.                                               *
 *============================================================================*/
AbstractWidget* AbstractInterface::GetWidget(const std::string& name)
{
	return m_pWidgetManager->GetWidget(name);
}