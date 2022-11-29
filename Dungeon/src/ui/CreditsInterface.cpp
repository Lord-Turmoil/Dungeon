/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : CreditsInterface.cpp                      *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 17, 2022                           *
 *                                                                            *
 *                    Last Update : November 29, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Credits interface.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/ui/CreditsInterface.h"


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** CreditsInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool CreditsInterface::Load(XMLElement* node)
{
	const char* name = node->Name();

	_CHECK_TAG("Credits");
	_RETURN_IF_ERROR();

	TimeInterface::Load(node);
	_LoadCredits(node);

	_RETURN_STATE();
}

void CreditsInterface::AddEvents()
{
	static_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("quit"))
		->OnTriggered(GetDetacher(this));
}

/********************************************************************
** This is a little clumsy... :(
** 2022/11/29 TS:
** Oh... Added Mega... :(
*/
void CreditsInterface::_Initialize()
{
	m_elapsedTime = 0L;
	static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("credits"))
		->ResetTransition();
	static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("prompt"))
		->ResetTransition();
	static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("mega"))
		->ResetTransition();
	Update(nullptr);	// Make sure coordinate reset is applied.

	m_elapsedTime = 0L;
	static_cast<AudioPlayer*>(m_pWidgetManager->GetWidget("bgm"))
		->Play();
}

void CreditsInterface::_Destroy()
{
	static_cast<AudioPlayer*>(m_pWidgetManager->GetWidget("bgm"))
		->Stop();
}

bool CreditsInterface::_LoadCredits(XMLElement* node)
{
	const char* name = node->Name();
	const char* itemName;
	XMLElement* item;
	CreditsType type;

	node = node->FirstChildElement("Texts");
	if (!node)
	{
		LOG_ERROR(MISSING_CHILD_ELEMENT, name);
		return false;
	}

	_InitializeProperty(node);
	
	item = node->FirstChildElement();
	while (item)
	{
		itemName = item->Name();
		if (_STR_SAME(itemName, "Item"))
			type = CRE_ITEM;
		else if (_STR_SAME(itemName, "Section"))
			type = CRE_SECTION;
		else if (_STR_SAME(itemName, "Chapter"))
			type = CRE_CHAPTER;
		else if (_STR_SAME(itemName, "Title"))
			type = CRE_TITLE;
		else if (_STR_SAME(itemName, "Copy"))
			type = CRE_COPYRIGHT;
		else
		{
			LOG_ERROR(UNKNOWN_TAG, itemName);
			return false;
		}

		_LoadItem(item, type);

		item = item->NextSiblingElement();
	}

	_InitializeWidget();

	_RETURN_STATE();
}

void CreditsInterface::_LoadItem(XMLElement* node, CreditsType type)
{
	const char* text = node->GetText();
	TextDrawer* drawer = new TextDrawer(m_size[type].x, m_size[type].y);

	if (type == CRE_COPYRIGHT)
		m_endPos = m_curPos;

	m_curPos.y += m_marginTop[type];
	drawer->SetText(text)
		->SetFontSize(m_fontSize[type])
		->SetFontColor(m_fontColor[type])
		->SetFontFace(m_fontFace[type]);
	drawer->SetCoord(m_curPos)
		->SetFocus(m_size[type] / 2);
	
	m_curPos.y += m_lineHeight[type];
	if (m_curDrawer)
	{
		m_curDrawer->SetSubDrawer(drawer);
		m_curDrawer = drawer;
	}
	else
		m_drawer = m_curDrawer = drawer;
}

void CreditsInterface::_InitializeProperty(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	_PARSE("duration", m_creditsDuration, name, 0L);

	XMLElement* item;
	item = node->FirstChildElement("Title");
	_InitializeProperty(item, CRE_TITLE);
	item = node->FirstChildElement("Chapter");
	_InitializeProperty(item, CRE_CHAPTER);
	item = node->FirstChildElement("Section");
	_InitializeProperty(item, CRE_SECTION);
	item = node->FirstChildElement("Item");
	_InitializeProperty(item, CRE_ITEM);
	item = node->FirstChildElement("Copy");
	_InitializeProperty(item, CRE_COPYRIGHT);

	for (int i = 0; i < CRE_NUM; i++)
	{
		m_marginTop[i] = (int)(m_marginTop[i] * deviceInfo.aspectRatio);
		m_lineHeight[i] = (int)(m_lineHeight[i] * deviceInfo.aspectRatio);
		m_fontSize[i] = (int)(m_fontSize[i] * deviceInfo.aspectRatio);
		m_size[i] *= deviceInfo.aspectRatio;

		m_lineHeight[i] = dmax(m_lineHeight[i], m_size[i].y);
	}

	m_beginPos.Init(deviceInfo.clientWidth / 2, deviceInfo.clientHeight + m_size[0].y / 2);
	m_curPos.Init(COORD_ZERO);
}

void CreditsInterface::_InitializeProperty(XMLElement* node, CreditsType type)
{
	const char* name = node->Name();
	const char* attr;

	_PARSE("margin-top", m_marginTop[type], name, 0);
	_PARSE_ESSENTIAL("line-height", m_lineHeight[type], name, 0);
	_PARSE_ESSENTIAL("font-size", m_fontSize[type], name, 0);
	_PARSE_ESSENTIAL("font-face", m_fontFace[type], name, "");
	_PARSE_PRIVATE_ESSENTIAL("font-color", m_fontColor[type], name, ParseColor);
	_PARSE_PRIVATE_ESSENTIAL("size", m_size[type], name, ParseCoord);
}

void CreditsInterface::_InitializeWidget()
{
	StaticWidget* widget = new StaticWidget();
	CoordTransition* trans = new CoordTransition();

	trans->SetDuration(m_creditsDuration);
	trans->SetBeginCoord(m_beginPos);
	trans->SetEndCoord({ m_beginPos.x, -m_endPos.y });
	trans->SetStyle(TransitionStyle::TRANS_STYLE_LINEAR);

	widget->SetName("credits");
	widget->SetDrawer(m_drawer);
	widget->SetCoord(m_beginPos);
	widget->AddTransition(trans);
	widget->SetLayer(10);
	widget->ResetTransition();

	m_pWidgetManager->AddWidget(widget);
}