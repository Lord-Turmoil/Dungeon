/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Drawer.h                                  *
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

#ifndef _DRAWER_H_
#define _DRAWER_H_

#include "Cell.h"

#include "../../inc/device/Symbol.h"


/********************************************************************
** Drawer is the basic display element.
*/
class Drawer
{
public:
	Drawer() : m_pSubDrawer(nullptr), m_drawerChanged(true) {}
	virtual ~Drawer();

	Symbol* GetSymbol() { return &m_symbol; }

	Drawer* SetSubDrawer(Drawer* drawer);
	Drawer* GetSubDrawer() { return m_pSubDrawer; }

	Drawer* SetLayer(int layer);
	Drawer* SetAlpha(int alpha);
	Drawer* SetScale(double scale);

	/*
	** Update on every frame.
	*/
	virtual void Update() = 0;

	virtual void Draw();

	/*
	** Load draw drawer from certain xml node.
	*/
	virtual bool Load(XMLElement* node) = 0;

protected:
	void _LoadSubDrawer(XMLElement* node);

	/*
	** If some property changes, drawer should re-render
	** it self before start another draw.
	*/
	virtual void _Render() = 0;

	Symbol m_symbol;
	Drawer* m_pSubDrawer;
	bool m_drawerChanged;
};


/********************************************************************
** RawDrawer generate symbol by itself.
*/
struct RawStyleAttribute
{
	RawStyleAttribute()
		: isFilled(true), lineColor(WHITE), fillColor(WHITE) {}

	bool		isFilled;
	COLORREF	lineColor;
	COLORREF	fillColor;
	LINESTYLE	lineStyle;
};

class RawDrawer : public Drawer
{
public:
	RawDrawer() {}
	virtual ~RawDrawer() {}

	RawDrawer* Filled(bool isFilled)
	{
		m_attribute.isFilled = isFilled;
		m_drawerChanged = true;
		return this;
	}
	bool IsFilled() const { return m_attribute.isFilled; }

	RawDrawer* SetLineColor(COLORREF color)
	{
		m_attribute.lineColor = color;
		m_drawerChanged = true;
		return this;
	}
	COLORREF GetLineColor() const { return m_attribute.lineColor; };

	RawDrawer* SetFillColor(COLORREF color)
	{
		m_attribute.fillColor = color;
		m_drawerChanged = true;
		return this;
	}
	COLORREF GetFillColor() const { return m_attribute.fillColor; }

	RawDrawer* SetLineThickness(int thickness)
	{
		m_attribute.lineStyle.thickness = thickness;
		m_drawerChanged = true;
		return this;
	}
	int GetLineThickness() const { return m_attribute.lineStyle.thickness; }

	virtual void Update() = 0;

	virtual bool Load(XMLElement* node);

protected:
	virtual void _Render() = 0;

	void _ApplyAttribute();

	RawStyleAttribute m_attribute;
};


/********************************************************************
** Specific drawers, rectangle, roundrectangle, circle drawer derive
** from RawDrawer, and image drawer derives from Drawer.
*/
class RectDrawer : public RectCell, public RawDrawer
{
public:
	RectDrawer() {}
	RectDrawer(int width, int height) : RectCell(width, height) {}
	virtual ~RectDrawer() {}

	virtual void Update();
	virtual bool Load(XMLElement* node);

private:
	virtual void _Render();
};

class RoundRectDrawer : public RoundRectCell, public RawDrawer
{
public:
	RoundRectDrawer() {}
	RoundRectDrawer(int width, int height, int radius = 0) : RoundRectCell(width, height, radius) {}
	virtual ~RoundRectDrawer() {}

	virtual void Update();
	virtual bool Load(XMLElement* node);

private:
	virtual void _Render();
};

class CircleDrawer : public CircleCell, public RawDrawer
{
public:
	CircleDrawer() {}
	CircleDrawer(int radius) : CircleCell(radius) {}
	virtual ~CircleDrawer() {}

	virtual void Update();
	virtual bool Load(XMLElement* node);

private:
	virtual void _Render();
};

class TextDrawer : public RectCell, public Drawer
{
public:
	TextDrawer() : m_fontColor(WHITE), m_fontHeight(0), m_isJustified(true) {}
	TextDrawer(int width, int height) : RectCell(width, height),
		m_fontColor(WHITE), m_fontHeight(0), m_isJustified(true) {}
	virtual ~TextDrawer() {}

	TextDrawer* SetText(const std::string& text)
	{
		m_text = text;
		m_drawerChanged = true;
		return this;
	}
	TextDrawer* SetText(const char* text)
	{
		m_text = text;
		m_drawerChanged = true;
		return this;
	}
	const std::string& GetText() const { return m_text; }
	const char* GetCText() const { return m_text.c_str(); }

	TextDrawer* SetFontSize(int fontHeight)
	{
		m_fontHeight = fontHeight;
		m_drawerChanged = true;
		return this;
	}
	TextDrawer* SetFontColor(COLORREF fontColor)
	{
		m_fontColor = fontColor;
		m_drawerChanged = true;
		return this;
	}
	TextDrawer* SetFontFace(const std::string& font)
	{
		m_font = font;
		m_drawerChanged = true;
		return this;
	}
	TextDrawer* SetFontFace(const char* font)
	{
		m_font = font;
		m_drawerChanged = true;
		return this;
	}
	TextDrawer* Justified(bool isJustified)
	{
		m_isJustified = isJustified;
		m_drawerChanged = true;
		return this;
	}

	virtual void Update();
	virtual bool Load(XMLElement* node);

private:
	void _ApplyTextAttribute();
	virtual void _Render();

	std::string m_text;

	int m_fontHeight;
	COLORREF m_fontColor;
	std::string m_font;

	/*
	** Whether to center the text in the rectangle or not.
	*/
	bool m_isJustified;
};

class ImageDrawer : public RectCell, public Drawer
{
public:
	ImageDrawer() {}
	ImageDrawer(int width, int height) : RectCell(width, height) {}
	virtual ~ImageDrawer() {}

	virtual void Update();
	virtual bool Load(XMLElement* node);

private:
	virtual void _Render();
};


Drawer* LoadDrawer(XMLElement* node);

#endif