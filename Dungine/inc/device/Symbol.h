/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Symbol.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 31, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For the basic image type and operations.                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include <string>
#include <easyx.h>

#include "../common/Common.h"


/********************************************************************
** Symbol attribute decides how the symbol is finally rendered on
** the screen.
*/
struct SymbolAttribute
{
	/*
	** rotationAngle is the radian the symbol should rotate.
	** Positive to rotate clockwise, and negative the other.
	*/
	double rotationAngle;
	
	/*
	** scale is the... scale :).
	*/
	double scale;

	/*
	** alpha is the alpha value of the image. 0 for invisible.
	*/
	int alpha;

	SymbolAttribute() : rotationAngle(0.0), scale(1.0), alpha(255) {}
	SymbolAttribute* SetRotationAngle(double _rotationAngle)
	{
		rotationAngle = _rotationAngle;
		return this;
	}
	SymbolAttribute* SetScale(double _scale)
	{
		scale = _scale;
		return this;
	}

	SymbolAttribute& operator=(const SymbolAttribute& obj)
	{
		this->rotationAngle = obj.rotationAngle;
		this->scale = obj.scale;
		this->alpha = obj.alpha;

		return *this;
	};
};

/********************************************************************
** Symbol is the basic drawing unit. All drawing items must be first
** convert to a symbol. However, every symbol should be a member of
** an instance of certain class, there mustn't be symbols which are
** newed individually outside of a class. It will cause memory leak!
*/
class Symbol
{
	friend struct CompareSymbol;
	friend class Renderer;
public:
	Symbol() : m_layer(LAYER_NORMAL),
		m_pSupSymbol(nullptr), m_pSubSymbol(nullptr), m_pAttribute(nullptr),
		m_pImage(nullptr), m_isStatic(false) {}
	~Symbol();

	void SetCoord(const Coordinate& coord) { m_coord = coord; }
	void SetCoord(int x, int y) { m_coord.Init(x, y); }
	Coordinate GetCoord() const { return m_coord; }

	Symbol* SetOffset(const Coordinate& offset)
	{
		m_offset = offset;
		return this;
	}
	Coordinate GetOffset() const { return m_offset; }

	void SetLayer(int layer) { m_layer = layer; }
	int GetLayer() const { return m_layer; }

	void SetStatic(bool isStatic) { m_isStatic = isStatic; }
	bool IsStatic() const { return m_isStatic; }

	/*
	** As for the attribute, a simpleton-like pattern is used.
	** It will not new the attribute until an attribute is
	** assigned.
	*/
	Symbol* SetAttribute(SymbolAttribute* attribute);
	SymbolAttribute* GetAttribute();
	Symbol* ClearAttribute();

	Symbol* SetSupSymbol(Symbol* pSupSymbol);
	Symbol* GetSupSymbol() const { return m_pSupSymbol; }
	Symbol* SetSubSymbol(Symbol* pSubSymbol);
	Symbol* GetSubSymbol() const { return m_pSubSymbol; }

	Symbol* SetImage(IMAGE* image);
	IMAGE* GetImage();

	void Translate(const Coordinate& offset);

public:
	/*
	** 2022/07/03 TS
	**   Warning, image of symbol may not be initialized yet!
	*/
	Symbol& operator=(const Symbol& sym)
	{
		m_pSubSymbol = sym.m_pSubSymbol;
		m_pSupSymbol = sym.m_pSupSymbol;
		
		if (sym.m_pAttribute)
			SetAttribute(sym.m_pAttribute);
		if (sym.m_isStatic)
			m_pImage = sym.m_pImage;
		else
		{
			if (m_pImage)
				delete m_pImage;
			m_pImage = (sym.m_pImage) ? new IMAGE(*(sym.m_pImage)) : nullptr;
		}
		m_coord = sym.m_coord;
		m_offset = sym.m_offset;
		m_layer = sym.m_layer;
		m_isStatic = sym.m_isStatic;
		
		return *this;
	};

protected:
	/*
	** These two are for the symbols that has strong connections,
	** such as the weapon of the figure. However, their draw order
	** with other symbols are decided by the current symbol alone.
	** Draw order: sup -> self -> sub.
	*/
	Symbol* m_pSupSymbol;
	Symbol* m_pSubSymbol;

	/*
	** The attribute should be applied individually.
	*/
	SymbolAttribute* m_pAttribute;

	IMAGE* m_pImage;

	/*
	** Coordinate of the upper left corner of the symbol.
	*/
	Coordinate m_coord;
	Coordinate m_offset;

	int m_layer;

	/*
	** If is static, it means that the image of the symbol is
	** pre-loaded at somewhere else, which is no need for the
	** symbol to delete it. Otherwise, the image is get from
	** new and need to be freed.
	*/
	bool m_isStatic;
};

/********************************************************************
** For the rendering order of the symbols.
*/
struct CompareSymbol
{
	bool operator() (const Symbol& sym1, const Symbol& sym2)
	{
		if (sym1.m_layer != sym2.m_layer)
			return sym1.m_layer > sym2.m_layer;
		else
			return sym1.m_coord.y > sym2.m_coord.y;
	};
	bool operator() (const Symbol* sym1, const Symbol* sym2)
	{
		if (sym1->m_layer != sym2->m_layer)
			return sym1->m_layer > sym2->m_layer;
		else
			return sym1->m_coord.y > sym2->m_coord.y;
	};
};


#if 0

/********************************************************************
** Convert text to symbol.
*/
struct TextStyle
{
	int height;
	int width;
	std::wstring face;
	COLORREF color;
};

Symbol* TextToSymbol(const std::wstring& text, int width, int height, TextStyle* style);
Symbol* TextToSymbol(Symbol* symbol, const std::wstring& text, int width, int height, TextStyle* style);

#endif // #if 0

#endif