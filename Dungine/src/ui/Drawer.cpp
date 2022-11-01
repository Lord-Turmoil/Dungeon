/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Drawer.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 9, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Drawer of the widget.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/ui/Drawer.h"

#include "../../inc/device/Color.h"
#include "../../inc/device/Device.h"
#include "../../inc/device/Explorer.h"

#include "../../inc/utility/Auxilliary.h"
#include "../../inc/utility/tinyxml.h"
#include "../../inc/utility/Parser.h"
#include "../../inc/utility/Straw.h"


/******************************************************************************
 * Drawer::~Drawer -- Destructor of the drawer.                               *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
Drawer::~Drawer()
{
	_DELETE(m_pSubDrawer);
}


/******************************************************************************
 * Drawer::SetSubDrawer -- Set sub-drawer of drawer.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   drawer -- New sub-drawer.                                         *
 *                                                                            *
 * OUTPUT:  Return sub drawer.                                                *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
Drawer* Drawer::SetSubDrawer(Drawer* drawer)
{
	_DELETE(m_pSubDrawer);

	m_pSubDrawer = drawer;

	return m_pSubDrawer;
}


/******************************************************************************
 * Drawer::SetLayer -- Set the layer of the drawer.                           *
 *                                                                            *
 *    Notice that sub-drawer has to be set a higher layer to display on top   *
 *    of the current layer.                                                   *
 *                                                                            *
 * INPUT:   layer                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
Drawer* Drawer::SetLayer(int layer)
{
	m_symbol.SetLayer(LAYER_UI + layer);
	if (m_pSubDrawer)
		m_pSubDrawer->SetLayer(layer + 1);

	return this;
}


/******************************************************************************
 * Drawer::SetAlpha -- Set the alpha of the drawer.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   alpha                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
Drawer* Drawer::SetAlpha(int alpha)
{
	m_symbol.GetAttribute()->alpha = alpha;
	if (m_pSubDrawer)
		m_pSubDrawer->SetAlpha(alpha);

	return this;
}


/******************************************************************************
 * Drawer::SetScale -- Set the scale of the drawer.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   scale                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
Drawer* Drawer::SetScale(double scale)
{
	m_symbol.GetAttribute()->scale = scale;
	if (m_pSubDrawer)
		m_pSubDrawer->SetScale(scale);

	return this;
}


/******************************************************************************
 * Drawer::Draw -- Just... draw.                                              *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void Drawer::Draw()
{
	Device::GetInstance()->AddSymbol(&m_symbol);
	if (m_pSubDrawer)
		m_pSubDrawer->Draw();
}


/******************************************************************************
 * Drawer::_LoadSubDrawer -- Load sub-drawer of a drawer.                     *
 *                                                                            *
 *    In theory, a drawer can only have one sub drawer, if multiple sub tags  *
 *    are assigned, it will only look at the first one. :)                    *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/29 Tony : Created.                                               *
 *============================================================================*/
void Drawer::_LoadSubDrawer(XMLElement* node)
{
	XMLElement* tag = node->FirstChildElement();

	if (tag)
		SetSubDrawer(LoadDrawer(tag));
}


/******************************************************************************
 * RawDrawer::Load -- Load drawer attributes.                                 *
 *                                                                            *
 *    This load drawing attributes for raw drawer to render symbol.           *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
bool RawDrawer::Load(XMLElement* node)
{
	/*
	** <... filled="" line-color="" fill-color="" border="" ...>
	*/
	const char* name = node->Name();
	const char* attr;

	_PARSE("filled", m_attribute.isFilled, name, true);
	_PARSE_PRIVATE("line-color", m_attribute.lineColor, name, ParseColor);
	_PARSE_PRIVATE("fill-color", m_attribute.fillColor, name, ParseColor);
	_PARSE("border", m_attribute.lineStyle.thickness, name, 1);

	m_drawerChanged = true;

	_RETURN_STATE();
}


/******************************************************************************
 * RawDrawer::_ApplyAttribute -- Apply drawing attribute.                     *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void RawDrawer::_ApplyAttribute()
{
	setlinecolor(m_attribute.lineColor);
	setlinestyle(&m_attribute.lineStyle);
	setfillcolor(m_attribute.fillColor);
}



/******************************************************************************
 * RectDrawer::Update -- Update rectangle drawer.                             *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void RectDrawer::Update()
{
	if (m_cellChanged || m_drawerChanged)
	{
		_Render();
		m_cellChanged = m_drawerChanged = false;
	}

	m_symbol.SetCoord(m_coord - m_focus);

	if (m_pSubDrawer)
		m_pSubDrawer->Update();
}


/******************************************************************************
 * RectDrawer::Load -- Load rectangle drawer from a xml node.                 *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
bool RectDrawer::Load(XMLElement* node)
{
	const char* name = node->Name();

	_CHECK_TAG("Rectangle");
	_RETURN_IF_ERROR();

	LoadProperty(node);
	RawDrawer::Load(node);

	m_drawerChanged = true;

	_LoadSubDrawer(node);

	_RETURN_STATE();
}


/******************************************************************************
 * RectDrawer::_Render -- Render rectangle image.                             *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void RectDrawer::_Render()
{
	int border = m_attribute.lineStyle.thickness & (~1u);	// turn odd to even
	int width = m_width + border;
	int height = m_height + border;
	Coordinate origin(border >> 1);

	m_symbol.GetImage()->Resize(width, height);

	Device::GetInstance()->SetTargetImage(m_symbol.GetImage());
	setbkcolor(TRANSPARENT_COLOR);
	cleardevice();

	_ApplyAttribute();

	if (m_attribute.isFilled)
	{
		if (m_attribute.lineStyle.thickness > 0)
			fillrectangle(origin.x, origin.y, origin.x + m_width, origin.y + m_height);
		else
			solidrectangle(origin.x, origin.y, origin.x + m_width, origin.y + m_height);
	}
	else if (m_attribute.lineStyle.thickness > 0)
		rectangle(origin.x, origin.y, origin.x + m_width, origin.y + m_height);

	Device::GetInstance()->SetTargetImage(NULL);
}


/******************************************************************************
 * RoundRectDrawer::Update -- Update roundrectangle drawer.                   *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void RoundRectDrawer::Update()
{
	if (m_cellChanged || m_drawerChanged)
	{
		_Render();
		m_cellChanged = m_drawerChanged = false;
	}

	m_symbol.SetCoord(m_coord - m_focus);

	if (m_pSubDrawer)
		m_pSubDrawer->Update();
}


/******************************************************************************
 * RoundRectDrawer::Load -- Load roundrectangle drawer from a xml node.       *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
bool RoundRectDrawer::Load(XMLElement* node)
{
	const char* name = node->Name();

	_CHECK_TAG("Roundrect");
	_RETURN_IF_ERROR();

	LoadProperty(node);
	RawDrawer::Load(node);

	m_drawerChanged = true;

	_LoadSubDrawer(node);

	_RETURN_STATE();
}


/******************************************************************************
 * RoundRectDrawer::_Render -- Render roundrectangle image.                   *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void RoundRectDrawer::_Render()
{
	int border = m_attribute.lineStyle.thickness & (~1u);	// turn odd to even
	int width = m_width + border;
	int height = m_height + border;
	Coordinate origin(border >> 1);

	m_symbol.GetImage()->Resize(width, height);

	Device::GetInstance()->SetTargetImage(m_symbol.GetImage());
	setbkcolor(TRANSPARENT_COLOR);
	cleardevice();

	_ApplyAttribute();

	if (m_attribute.isFilled)
	{
		if (m_attribute.lineStyle.thickness > 0)
			fillroundrect(origin.x, origin.y, origin.x + m_width, origin.y + m_height, m_radius, m_radius);
		else
			solidroundrect(origin.x, origin.y, origin.x + m_width, origin.y + m_height, m_radius, m_radius);
	}
	else if (m_attribute.lineStyle.thickness > 0)
		roundrect(origin.x, origin.y, origin.x + m_width, origin.y + m_height, m_radius, m_radius);

	Device::GetInstance()->SetTargetImage(NULL);
}


/******************************************************************************
 * CircleDrawer::Update -- Update circle drawer.                              *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void CircleDrawer::Update()
{
	if (m_cellChanged || m_drawerChanged)
	{
		_Render();
		m_cellChanged = m_drawerChanged = false;
	}

	m_symbol.SetCoord(m_coord - m_focus);

	if (m_pSubDrawer)
		m_pSubDrawer->Update();
}


/******************************************************************************
 * CircleDrawer::Load -- Load circle drawer.                                  *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
bool CircleDrawer::Load(XMLElement* node)
{
	const char* name = node->Name();

	_CHECK_TAG("Circle");
	_RETURN_IF_ERROR();

	LoadProperty(node);
	RawDrawer::Load(node);

	m_drawerChanged = true;

	_LoadSubDrawer(node);

	_RETURN_STATE();
}


/******************************************************************************
 * CircleDrawer::_Render -- Render circle drawer.                             *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void CircleDrawer::_Render()
{
	int border = m_attribute.lineStyle.thickness & (~1u);	// turn odd to even
	int width, height;
	Coordinate origin(m_radius + (border >> 1));
	width = height = m_radius * 2 + 1 + border;

	m_symbol.GetImage()->Resize(width, height);

	Device::GetInstance()->SetTargetImage(m_symbol.GetImage());
	setbkcolor(TRANSPARENT_COLOR);
	cleardevice();

	_ApplyAttribute();

	if (m_attribute.isFilled)
	{
		if (m_attribute.lineStyle.thickness > 0)
			fillcircle(origin.x, origin.y, m_radius);
		else
			solidcircle(origin.x, origin.y, m_radius);
	}
	else if (m_attribute.lineStyle.thickness > 0)
		circle(origin.x, origin.y, m_radius);

	Device::GetInstance()->SetTargetImage(NULL);
}


/******************************************************************************
 * TextDrawer::Update -- Update text drawer.                                  *
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
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
void TextDrawer::Update()
{
	if (m_cellChanged || m_drawerChanged)
	{
		_Render();
		m_cellChanged = m_drawerChanged = false;
	}

	m_symbol.SetCoord(m_coord - m_focus);

	if (m_pSubDrawer)
		m_pSubDrawer->Update();
}


/******************************************************************************
 * TextDrawer::Load -- Load text drawer.                                      *
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
 *   2022/05/18 Tony : Created.                                               *
 *   2022/06/01 Tony : Added fit for font size.                               *
 *============================================================================*/
bool TextDrawer::Load(XMLElement* node)
{
	/*
	**	<Text pos="" focus="" width="" height="" font="" font-size="" color="" justified="">
	**		Content
	**	</Text>
	*/
	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("Text");
	_RETURN_IF_ERROR();

	LoadProperty(node);

	_PARSE("font", m_font, name, "");
	_PARSE("font-size", m_fontHeight, name, 0);
	m_fontHeight = (int)(m_fontHeight * deviceInfo.aspectRatio);
	_PARSE_PRIVATE("color", m_fontColor, name, ParseColor);
	_PARSE("justified", m_isJustified, name, true);

	attr = node->GetText();
	if (!attr)
		LOG_EXTRA_WARNING(MISSING_TEXT);
	else
	{
		m_text = attr;
		strip(&m_text);
	}

	m_drawerChanged = true;

	_LoadSubDrawer(node);

	_RETURN_STATE();
}


/******************************************************************************
 * TextDrawer::_ApplyTextAttribute -- Apply text attribute.                   *
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
 *   2022/05/26 Tony : Created.                                               *
 *============================================================================*/
void TextDrawer::_ApplyTextAttribute()
{
	settextcolor(m_fontColor);
	settextstyle(m_fontHeight, 0, widen(m_font.c_str()));
}


/******************************************************************************
 * TextDrawer::_Render -- Render text drawer.                                 *
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
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
void TextDrawer::_Render()
{
	RECT rect = { 0, 0, m_width, m_height };

	m_symbol.GetImage()->Resize(m_width, m_height);

	Device::GetInstance()->SetTargetImage(m_symbol.GetImage());
	setbkcolor(TRANSPARENT_COLOR);
	cleardevice();

	_ApplyTextAttribute();

	if (m_isJustified)
		drawtext(widen(m_text.c_str()), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	else
		outtextxy(0, 0, widen(m_text.c_str()));

	Device::GetInstance()->SetTargetImage(NULL);
}


/******************************************************************************
 * ImageDrawer::Update -- Update image drawer.                                *
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
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void ImageDrawer::Update()
{
	if (m_cellChanged || m_drawerChanged)
		_Render();

	m_symbol.SetCoord(m_coord - m_focus);

	if (m_pSubDrawer)
		m_pSubDrawer->Update();
}


/******************************************************************************
 * ImageDrawer::Load -- Load image drawer.                                    *
 *                                                                            *
 *    For image drawer, it should specify another three attributes: origin,   *
 *    size and src. origin and size are pair, src is the filename of image.   *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
bool ImageDrawer::Load(XMLElement* node)
{
	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("Image");
	_RETURN_IF_ERROR();

	LoadProperty(node);

	Coordinate origin;
	Coordinate size;
	const char* src;

	_PARSE_PRIVATE("origin", origin, name, ParseCoord);
	_PARSE_PRIVATE("size", size, name, ParseCoord);

	src = node->Attribute("src");
	if (!src)
	{
		LOG_ERROR(MISSING_ATTRIBUTE, "src", name);
		return false;
	}

	ImageResource* res = LoadResource<ImageResource>(src);
	if (!res)
	{
		LOG_ERROR(INVALID_RESOURCE_ID, src);
		return false;
	}

	/*
	** A compromise, the background should fill the screen.
	*/
	if (size == COORD_ZERO)
		FetchImage(m_symbol.GetImage(), res->GetResource());
	else
		FetchImage(m_symbol.GetImage(), res->GetResource(), m_width, m_height, origin.x, origin.y, size.x, size.y);

	_LoadSubDrawer(node);

	_RETURN_STATE();
}


/******************************************************************************
 * ImageDrawer::_Render -- Render image drawer.                               *
 *                                                                            *
 *    For now, image drawer should not perform re-render. :(                  *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/14 Tony : Created.                                               *
 *============================================================================*/
void ImageDrawer::_Render()
{
	// On hold.
}



/******************************************************************************
 * LoadDrawer -- Load a drawer.                                               *
 *                                                                            *
 *    This uses a simple factory pattern. It will return nullptr and log an   *
 *    error if an invalid node is passed.                                     *
 *                                                                            *
 * INPUT:   node -- Any xml node.                                             *
 *                                                                            *
 * OUTPUT:  Return the new node loaded. nullptr if failed.                    *
 *                                                                            *
 * WARNINGS:  This may return nullptr!                                        *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/18 Tony : Created.                                               *
 *============================================================================*/
Drawer* LoadDrawer(XMLElement* node)
{
	if (!node)
		return nullptr;

	const char* name = node->Name();
	Drawer* rv = nullptr;

	if (_STR_SAME(name, "Rectangle"))
		rv = new RectDrawer();
	else if (_STR_SAME(name, "Roundrect"))
		rv = new RoundRectDrawer();
	else if (_STR_SAME(name, "Circle"))
		rv = new CircleDrawer();
	else if (_STR_SAME(name, "Text"))
		rv = new TextDrawer();
	else if (_STR_SAME(name, "Image"))
		rv = new ImageDrawer();

	if (rv)
		rv->Load(node);
	else
		LOG_ERROR(UNKNOWN_TAG, name);

	return rv;
}