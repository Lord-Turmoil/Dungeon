/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Cell.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 9, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Shape info of widgets.                                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _CELL_H_
#define _CELL_H_

#include "../common/Common.h"
#include "../utility/tinyxml.h"


/********************************************************************
** Cell is the base class of Drawer and Trigger. It contains the
** basic info of shapes.
*/
class Cell
{
public:
	Cell() : m_width(0), m_height(0), m_cellChanged(true) {}
	Cell(int width, int height)
		: m_width(width), m_height(height), m_cellChanged(true) {}
	virtual ~Cell() {}

	Cell* SetCoord(const Coordinate& coord)
	{
		m_coord = coord;
		return this;
	}
	Cell* SetCoord(int x, int y)
	{
		m_coord.Init(x, y);
		return this;
	}
	Coordinate GetCoord() const { return m_coord; }

	Cell* SetSize(int width, int height)
	{
		m_width = width;
		m_height = height;
		m_cellChanged = true;
		return this;
	}
	Cell* SetWidth(int width)
	{
		m_width = width;
		m_cellChanged = true;
		return this;
	}
	Cell* SetHeight(int height)
	{
		m_height = height;
		m_cellChanged = true;
		return this;
	}
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	Cell* SetFocus(const Coordinate& focus)
	{
		m_focus = focus;
		return this;
	}
	Cell* SetFocus(int x, int y)
	{
		m_focus.Init(x, y);
		return this;
	}
	Coordinate GetFocus() const { return m_focus; }

	Cell* Translate(const Coordinate& offset)
	{
		m_coord += offset;
		return this;
	}

	void LoadProperty(XMLElement* node);

protected:
	virtual void _LoadProperty(XMLElement* node);
	virtual void _AdjustProperty(XMLElement* node);

	Coordinate m_coord;
	int m_width;
	int m_height;

	/*
	** m_focus is the coordinate inside the drawer, whose global
	** coordinate is m_coord.
	*/
	Coordinate m_focus;

	/*
	** This is for derived classes. If is changed, then the upper
	** level should do some thing.
	*/
	bool m_cellChanged;
};


/********************************************************************
** Cell with a rectangle shape.
*/
class RectCell : public Cell
{
public:
	RectCell() {}
	RectCell(int width, int height) : Cell(width, height) {}
	virtual ~RectCell() {}

protected:
	virtual void _LoadProperty(XMLElement* node);
	virtual void _AdjustProperty(XMLElement* node);
};


/********************************************************************
** Cell with a roundrectangel shape.
*/
class RoundRectCell : public RectCell
{
public:
	RoundRectCell() : m_radius(0) {}
	RoundRectCell(int width, int height, int radius = 0) : RectCell(width, height), m_radius(radius) {}
	virtual ~RoundRectCell() {}

	RoundRectCell* SetRadius(int radius)
	{
		m_radius = radius;
		m_cellChanged = true;
		return this;
	}
	int GetRadius() const { return m_radius; }

protected:
	virtual void _LoadProperty(XMLElement* node);
	virtual void _AdjustProperty(XMLElement* node);

	// Four border-radius are the same.
	int m_radius;
};


/********************************************************************
** Cell with a circle shape. For circle cell, m_width and m_height
** are invalid.
*/
class CircleCell : public Cell
{
public:
	CircleCell() : m_radius(0) {}
	CircleCell(int radius) : Cell()
	{
		m_radius = radius;
	}
	virtual ~CircleCell() {}

	CircleCell* SetRadius(int radius)
	{
		m_radius = radius;
		m_cellChanged = true;
		return this;
	}
	int GetRadius() const { return m_radius; }

protected:
	virtual void _LoadProperty(XMLElement* node);
	virtual void _AdjustProperty(XMLElement* node);

	int m_radius;
};

#endif