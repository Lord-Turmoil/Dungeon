/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Transition.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 25, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For the transition effect of the widgets. Similar to the CSS transform   *
 *   style.                                                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/utility/Auxilliary.h"

#include "../../inc/device/Timer.h"
#include "../../inc/device/Device.h"

#include "../../inc/utility/xml.h"
#include "../../inc/utility/Parser.h"

#include "../../inc/ui/Widget.h"
#include "../../inc/ui/Transition.h"



/******************************************************************************
 * Transition::Reset -- Reset transition.                                     *
 *                                                                            *
 *    This will also reset m_isWaiting, to play transition all over again.    *
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
void Transition::Reset()
{
	m_elapsedTime = 0;
	m_isReversed = false;
	m_isWaiting = m_delay ? true : false;
	m_isOver = m_duration ? false : true;
	_Reset();
}


/******************************************************************************
 * Transition::Terminate -- Terminate transition right away.                  *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void Transition::Terminate()
{
	m_elapsedTime = m_duration;
	m_isReversed = false;
	m_isWaiting = false;
	m_isOver = true;
	_Terminate();
}


/******************************************************************************
 * Transition::Stop -- Stop transition.                                       *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void Transition::Stop()
{
	m_isOver = true;
}


/******************************************************************************
 * Transition::Update -- Update transition.                                   *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void Transition::Update()
{
	if (m_isOver || !m_pWidget)
		return;

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	if (m_isWaiting)
	{
		if (m_elapsedTime > m_delay)
		{
			m_elapsedTime = 0;
			m_isWaiting = false;
		}

		return;
	}

	if (m_elapsedTime > m_duration)	// one turn over
	{
		_Finalize();

		if (m_loop)
		{
			if (m_reverse)
				_Reverse();
		}
		else
			m_isOver = true;

		m_elapsedTime = 0;

		return;
	}

	_Update();
}


/******************************************************************************
 * Transition::Load -- Load transition.                                       *
 *                                                                            *
 *    Load a single transition.                                               *
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
bool Transition::Load(XMLElement* node)
{
/*
**	<Transition type="" duration="0" delay="0" loop="false" reverse="false" style="linear"...>
*/
	const char* name = node->Name();
	const char* attr;
	bool value = false;
	
	_PARSE("duration", m_duration, name, 0);
	_PARSE("delay", m_delay, name, 0);
	_PARSE("loop", value, name, false);
	m_loop = value;
	_PARSE("reverse", value, name, false);
	m_reverse = value;
	
	attr = node->Attribute("style");
	if (attr)
	{
		if (_STR_SAME(attr, "ease-out"))
			m_style = TransitionStyle::TRANS_STYLE_EASE_OUT;
		else if (_STR_SAME(attr, "ease-in"))
			m_style = TransitionStyle::TRANS_STYLE_EASE_IN;
		else if (_STR_SAME(attr, "linear"))
			m_style = TransitionStyle::TRANS_STYLE_LINEAR;
		else
			LOG_ERROR(INVALID_ATTRIBUTE, "style", name);
	}
	SetStyle(m_style);

	m_delay = max(m_delay, 0);
	m_duration = max(m_duration, 0);

	m_isWaiting = m_delay ? true : false;
	m_isOver = m_duration ? false : true;

	return true;
}

/******************************************************************************
 * Transition::_Reverse -- Reverse the transition process.                    *
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
void Transition::_Reverse()
{
	m_isReversed = !m_isReversed;
}


/******************************************************************************
 * CoordTransition::Load -- Load coord-transition.                            *
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
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
bool CoordTransition::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	Transition::Load(node);

	_PARSE_PRIVATE("begin", m_begin, name, ParseCoord);
	_PARSE_PRIVATE("end", m_end, name, ParseCoord);

	_AdjustProperty(node);

	return !Logger::Error();
}


/******************************************************************************
 * CoordTransition::SetStyle -- Set style.                                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   style                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/18 Tony : Created.                                               *
 *============================================================================*/
CoordTransition* CoordTransition::SetStyle(TransitionStyle style)
{
	m_style = style;

	switch (m_style)
	{
	case TransitionStyle::TRANS_STYLE_EASE_OUT:
		m_blender = EaseOutBlender<Coordinate>;
		break;
	case TransitionStyle::TRANS_STYLE_EASE_IN:
		m_blender = EaseInBlender<Coordinate>;
		break;
	case TransitionStyle::TRANS_STYLE_LINEAR:
		m_blender = LinearBlender<Coordinate>;
		break;
	default:
		m_blender = LinearBlender<Coordinate>;
		break;
	}

	return this;
}

/******************************************************************************
 * CoordTransition::_Update -- Update coordinate transition.                  *
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
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
void CoordTransition::_Update()
{
	if (m_isReversed)
		m_pWidget->SetCoord(BlendValue(m_end, m_begin, ((double)m_elapsedTime / (double)m_duration), m_blender));
	else
		m_pWidget->SetCoord(BlendValue(m_begin, m_end, ((double)m_elapsedTime / (double)m_duration), m_blender));
}


/******************************************************************************
 * CoordTransition::_Reset -- Reset coord transition.                         *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void CoordTransition::_Reset()
{
	m_pWidget->SetCoord(m_begin);
}


/******************************************************************************
 * CoordTransition::_Terminate -- Terminate coord transition.                 *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void CoordTransition::_Terminate()
{
	m_pWidget->SetCoord(m_end);
}


/******************************************************************************
 * CoordTransition::_Finalize -- Finalize transition.                         *
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
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
void CoordTransition::_Finalize()
{
	if (m_isReversed)
		m_pWidget->SetCoord(m_begin);
	else
		m_pWidget->SetCoord(m_end);
}


/******************************************************************************
 * CoordTransition::_AdjustProperty -- Adjust to fit screen.                  *
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
 *   2022/06/01 Tony : Created.                                               *
 *============================================================================*/
void CoordTransition::_AdjustProperty(XMLElement* node)
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
		m_begin.x = (int)(m_begin.x * deviceInfo.aspectRatio + deviceInfo.padding.x);
		m_begin.y = (int)(m_begin.y * deviceInfo.aspectRatio + deviceInfo.padding.y);

		m_end.x = (int)(m_end.x * deviceInfo.aspectRatio + deviceInfo.padding.x);
		m_end.y = (int)(m_end.y * deviceInfo.aspectRatio + deviceInfo.padding.y);

		return;
	}

	// adjust with float
	bool toLeft = true;
	bool toTop = true;
	_PARSE("left", toLeft, name, true);
	_PARSE("top", toTop, name, true);

	attr = node->Attribute("begin");
	if (attr)
	{
		if (strchr(attr, '%'))
		{
			m_begin.x = deviceInfo.clientWidth * m_begin.x / 100;
			m_begin.y = deviceInfo.clientHeight * m_begin.y / 100;
		}
	}
	attr = node->Attribute("end");
	if (attr)
	{
		if (strchr(attr, '%'))
		{
			m_end.x = deviceInfo.clientWidth * m_end.x / 100;
			m_end.y = deviceInfo.clientHeight * m_end.y / 100;
		}
	}

	if (!toLeft)
	{
		m_begin.x = deviceInfo.clientWidth - m_begin.x;
		m_end.x = deviceInfo.clientWidth - m_end.x;
	}
	if (!toTop)
	{
		m_begin.y = deviceInfo.clientHeight - m_begin.y;
		m_end.y = deviceInfo.clientHeight - m_end.y;
	}
}


/******************************************************************************
 * AlphaTransition::Load -- Load alpha transition.                            *
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
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
bool AlphaTransition::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	Transition::Load(node);

	_PARSE("begin", m_begin, name, 255);
	_PARSE("end", m_end, name, 255);

	_AdjustProperty(node);

	return !Logger::Error();
}


/******************************************************************************
 * AlphaTransition::SetStyle -- Set style.                                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   style                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/18 Tony : Created.                                               *
 *============================================================================*/
AlphaTransition* AlphaTransition::SetStyle(TransitionStyle style)
{
	m_style = style;

	switch (m_style)
	{
	case TransitionStyle::TRANS_STYLE_EASE_OUT:
		m_blender = EaseOutBlender<int>;
		break;
	case TransitionStyle::TRANS_STYLE_EASE_IN:
		m_blender = EaseInBlender<int>;
		break;
	case TransitionStyle::TRANS_STYLE_LINEAR:
		m_blender = LinearBlender<int>;
		break;
	default:
		m_blender = LinearBlender<int>;
		break;
	}

	return this;
}


/******************************************************************************
 * AlphaTransition::_Update -- Update alpha transition.                       *
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
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
void AlphaTransition::_Update()
{
	if (m_isReversed)
		m_pWidget->SetAlpha(BlendValue(m_end, m_begin, ((double)m_elapsedTime / (double)m_duration), m_blender));
	else
		m_pWidget->SetAlpha(BlendValue(m_begin, m_end, ((double)m_elapsedTime / (double)m_duration), m_blender));
}


/******************************************************************************
 * AlphaTransition::_Reset -- Reset alpha transition.                         *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void AlphaTransition::_Reset()
{
	m_pWidget->SetAlpha(m_begin);
}


/******************************************************************************
 * AlphaTransition::_Terminate -- Terminate alpha transition.                 *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void AlphaTransition::_Terminate()
{
	m_pWidget->SetAlpha(m_end);
}


/******************************************************************************
 * AlphaTransition::_Finalize -- Finalize transition.                         *
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
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
void AlphaTransition::_Finalize()
{
	if (m_isReversed)
		m_pWidget->SetAlpha(m_begin);
	else
		m_pWidget->SetAlpha(m_end);
}


/******************************************************************************
 * ScaleTransition::Load -- Load scale transition.                            *
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
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
bool ScaleTransition::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	Transition::Load(node);

	_PARSE("begin", m_begin, name, 1.0);
	_PARSE("end", m_end, name, 1.0);

	_AdjustProperty(node);

	return !Logger::Error();
}


/******************************************************************************
 * ScaleTransition::SetStyle -- Set style.                                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   style                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/18 Tony : Created.                                               *
 *============================================================================*/
ScaleTransition* ScaleTransition::SetStyle(TransitionStyle style)
{
	m_style = style;

	switch (m_style)
	{
	case TransitionStyle::TRANS_STYLE_EASE_OUT:
		m_blender = EaseOutBlender<double>;
		break;
	case TransitionStyle::TRANS_STYLE_EASE_IN:
		m_blender = EaseInBlender<double>;
		break;
	case TransitionStyle::TRANS_STYLE_LINEAR:
		m_blender = LinearBlender<double>;
		break;
	default:
		m_blender = LinearBlender<double>;
		break;
	}

	return this;
}

/******************************************************************************
 * ScaleTransition::_Update -- Update scale transition.                       *
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
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
void ScaleTransition::_Update()
{
	if (m_isReversed)
		m_pWidget->SetScale(BlendValue(m_end, m_begin, ((double)m_elapsedTime / (double)m_duration), m_blender));
	else
		m_pWidget->SetScale(BlendValue(m_begin, m_end, ((double)m_elapsedTime / (double)m_duration), m_blender));
}


/******************************************************************************
 * ScaleTransition::_Reset -- Reset scale transition.                         *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void ScaleTransition::_Reset()
{
	m_pWidget->SetScale(m_begin);
}


/******************************************************************************
 * ScaleTransition::_Terminate -- Terminate scale transition.                 *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void ScaleTransition::_Terminate()
{
	m_pWidget->SetScale(m_end);
}


/******************************************************************************
 * ScaleTransition::_Finalize -- Finalize transition.                         *
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
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
void ScaleTransition::_Finalize()
{
	if (m_isReversed)
		m_pWidget->SetScale(m_begin);
	else
		m_pWidget->SetScale(m_end);
}


/******************************************************************************
 * RotationTransition::Load -- Load rotation transition.                      *
 *                                                                            *
 *    For convenience, here we read in degree, and then convert it to rad.    *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
bool RotationTransition::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	Transition::Load(node);

	_PARSE("begin", m_begin, name, 0.0);
	_PARSE("end", m_end, name, 0.0);

	m_begin *= DEG_TO_RAD;
	m_end *= DEG_TO_RAD;

	_AdjustProperty(node);

	return !Logger::Error();
}


/******************************************************************************
 * RotationTransition::SetStyle -- Set transition style.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   style                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
RotationTransition* RotationTransition::SetStyle(TransitionStyle style)
{
	m_style = style;

	switch (m_style)
	{
	case TransitionStyle::TRANS_STYLE_EASE_OUT:
		m_blender = EaseOutBlender<double>;
		break;
	case TransitionStyle::TRANS_STYLE_EASE_IN:
		m_blender = EaseInBlender<double>;
		break;
	case TransitionStyle::TRANS_STYLE_LINEAR:
		m_blender = LinearBlender<double>;
		break;
	default:
		m_blender = LinearBlender<double>;
		break;
	}

	return this;
}


/******************************************************************************
 * RotationTransition::_Update -- Update rotation transition.                 *
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
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
void RotationTransition::_Update()
{
	if (m_isReversed)
		m_pWidget->SetRotationAngle(BlendValue(m_end, m_begin, ((double)m_elapsedTime / (double)m_duration), m_blender));
	else
		m_pWidget->SetRotationAngle(BlendValue(m_begin, m_end, ((double)m_elapsedTime / (double)m_duration), m_blender));
}


/******************************************************************************
 * RotationTransition::_Reset -- Reset rotation transition.                   *
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
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
void RotationTransition::_Reset()
{
	m_pWidget->SetRotationAngle(m_begin);
}


/******************************************************************************
 * RotationTransition::_Terminate -- Terminate rotation transition.           *
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
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
void RotationTransition::_Terminate()
{
	m_pWidget->SetRotationAngle(m_end);
}


/******************************************************************************
 * RotationTransition::_Finalize -- Terminate rotation transition.            *
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
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
void RotationTransition::_Finalize()
{
	if (m_isReversed)
		m_pWidget->SetRotationAngle(m_begin);
	else
		m_pWidget->SetRotationAngle(m_end);
}


/******************************************************************************
 * LoadTransition -- Load transition from xml node.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return loaded transition.                                         *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/25 Tony : Created.                                               *
 *============================================================================*/
Transition* LoadTransition(XMLElement* node)
{
	if (!node)
		return nullptr;

	const char* name = node->Name();
	Transition* rv = nullptr;
	if (_STR_SAME(name, "CoordTransition"))
		rv = new CoordTransition();
	else if (_STR_SAME(name, "AlphaTransition"))
		rv = new AlphaTransition();
	else if (_STR_SAME(name, "ScaleTransition"))
		rv = new ScaleTransition();
	else if (_STR_SAME(name, "RotationTransition"))
		rv = new RotationTransition();

	if (rv)
		rv->Load(node);
	else
		LOG_ERROR(UNKNOWN_TAG, name);

	return rv;
}