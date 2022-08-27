/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : AbstractWidget.cpp                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 17, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   A refactoring of previous widget module.                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/common/Common.h"

#include "../../inc/ui/AbstractWidget.h"
#include "../../inc/ui/Transition.h"

#include "../../inc/device/Device.h"

#include "../../inc/utility/Parser.h"


/******************************************************************************
 * AbstractWidget::Load -- Load primary info of widgets.                      *
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
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
bool AbstractWidget::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	_PARSE_ESSENTIAL("name", m_name, name, "INVALID");
	_RETURN_IF_ERROR();

	_RETURN_IF_ERROR();
	if (Logger::Error())
		return false;

	_PARSE("activated", m_isActivated, name, true);

	return true;
}


/******************************************************************************
 * VisualWidget::~VisualWidget -- Destructor of the object.                   *
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
 *   2022/08/18 Tony : Created.                                               *
 *============================================================================*/
VisualWidget::~VisualWidget()
{
	for (auto it = m_transitionList.begin(); it != m_transitionList.end(); it++)
		delete (*it);
}


/******************************************************************************
 * VisualWidget::AddTransition -- Add transition to widget.                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   transition -- New transition style.                               *
 *                                                                            *
 * OUTPUT:  Return newly added transition.                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Transition* VisualWidget::AddTransition(Transition* transition)
{
	transition->SetWidget(this);
	m_transitionList.push_back(transition);

	return transition;
}


/******************************************************************************
 * VisualWidget::ResetTransition -- Reset all transition of widget.           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
VisualWidget* VisualWidget::ResetTransition()
{
	for (auto it = m_transitionList.begin(); it != m_transitionList.end(); it++)
		(*it)->Reset();

	return this;
}


/******************************************************************************
 * VisualWidget::TerminateTransition -- Terminate all transition of widget.   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/04 Tony : Created.                                               *
 *============================================================================*/
VisualWidget* VisualWidget::TerminateTransition()
{
	for (auto it = m_transitionList.begin(); it != m_transitionList.end(); it++)
		(*it)->Terminate();

	return this;
}


/******************************************************************************
 * VisualWidget::StopTransition -- Stop all transition of widget.             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/04 Tony : Created.                                               *
 *============================================================================*/
VisualWidget* VisualWidget::StopTransition()
{
	for (auto it = m_transitionList.begin(); it != m_transitionList.end(); it++)
		(*it)->Stop();

	return this;
}




/******************************************************************************
 * VisualWidget::_UpdateTransition -- Update all transition of widgets.       *
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
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
void VisualWidget::_UpdateTransition()
{
	for (auto it = m_transitionList.begin(); it != m_transitionList.end(); it++)
		(*it)->Update();
}


/******************************************************************************
 * VisualWidget::Load -- Load basic properties of widget.                     *
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
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
bool VisualWidget::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;
	XMLElement* tag;

	AbstractWidget::Load(node);

	_PARSE_PRIVATE("pos", m_coord, name, ParseCoord);

	_PARSE("layer", m_layer, name, 0);
	SetLayer(m_layer);

	_AdjustProperty(node);

	tag = node->FirstChildElement("Transitions");
	if (tag)
	{
		tag = tag->FirstChildElement();
		while (tag)
		{
			AddTransition(LoadTransition(tag));
			tag = tag->NextSiblingElement();
		}
	}

	return true;
}


/******************************************************************************
 * VisualWidget::LoadAttribute -- Load widget attributes.                     *
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
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
bool VisualWidget::LoadAttribute(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	int alpha = 255;
	_PARSE("alpha", alpha, name, 255);
	if (alpha != 255)
		SetAlpha(alpha);

	double scale = 1.0;
	_PARSE("scale", scale, name, 1.0);
	if (scale != 1.0)
		SetScale(scale);

	return true;
}


/******************************************************************************
 * VisualWidget::_AdjustProperty -- Adjust coordinate to fit screen.          *
 *                                                                            *
 *    VisualWidget only needs to adjust its coordinate, others will be adjust by    *
 *    Cell.
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/01 Tony : Created.                                               *
 *============================================================================*/
void VisualWidget::_AdjustProperty(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;
	bool isFloat = false;

	attr = node->Attribute("float");
	if (attr)
	{
		if (!ParseAttribute(&isFloat, attr, false))
			LOG_ERROR(INVALID_ATTRIBUTE, "float", name);
	}

	if (!isFloat)
	{
		// adjust position with no float
		m_coord.x = (int)(m_coord.x * deviceInfo.aspectRatio + deviceInfo.padding.x);
		m_coord.y = (int)(m_coord.y * deviceInfo.aspectRatio + deviceInfo.padding.y);

		return;
	}

	// adjust with float
	bool toLeft = true;
	bool toTop = true;
	_PARSE("left", toLeft, name, true);
	_PARSE("top", toTop, name, true);

	attr = node->Attribute("pos");
	if (attr)
	{
		if (strchr(attr, '%'))
		{
			m_coord.x = deviceInfo.clientWidth * m_coord.x / 100;
			m_coord.y = deviceInfo.clientHeight * m_coord.y / 100;
		}
		else
		{
			m_coord.x = (int)(m_coord.x * deviceInfo.aspectRatio);
			m_coord.y = (int)(m_coord.y * deviceInfo.aspectRatio);
		}
	}

	if (!toLeft)
		m_coord.x = deviceInfo.clientWidth - m_coord.x;
	if (!toTop)
		m_coord.y = deviceInfo.clientHeight - m_coord.y;
}