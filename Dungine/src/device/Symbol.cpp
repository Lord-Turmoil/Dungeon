/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Symbol.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For the basic image type and operations.                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/device/Symbol.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Symbol Region
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/******************************************************************************
 * Symbol::~Symbol -- Destructor of the object.                               *
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
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
Symbol::~Symbol()
{
    if (m_pAttribute)
    {
        delete m_pAttribute;
    }

    /*
    if (m_pSupSymbol)
            delete m_pSupSymbol;
    */

    /*
    if (m_pSubSymbol)
            delete m_pSubSymbol;
    */

    if ((!m_isStatic) && m_pImage)
    {
        delete m_pImage;
    }
}

/******************************************************************************
 * Symbol::Translate -- Translate symbol.                                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:  offset                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/28 Tony : Created.                                               *
 *============================================================================*/
void Symbol::Translate(const Coordinate& offset)
{
    m_coord += offset;

    if (m_pSupSymbol)
    {
        m_pSupSymbol->Translate(offset);
    }
    if (m_pSubSymbol)
    {
        m_pSubSymbol->Translate(offset);
    }
}

/******************************************************************************
 * Symbol::SetSupSymbol -- Set the sup symbol of the symbol.                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   pSupSymbol                                                        *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  Since symbols are all freed by there own parent class, so they  *
 *            mustn't be freed here!!!.                                       *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *   2022/05/21 Tony : Removed unneccessary delete.                           *
 *============================================================================*/
Symbol* Symbol::SetSupSymbol(Symbol* pSupSymbol)
{
    m_pSupSymbol = pSupSymbol;

    return this;
}

/******************************************************************************
 * Symbol::SetSubSymbol -- Set the sub symbol of the symbol.                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   pSubSymbol                                                        *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  The same as SetSupSymbol.                                       *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *   2022/05/21 Tony : Removed unneccessary delete.                           *
 *============================================================================*/
Symbol* Symbol::SetSubSymbol(Symbol* pSubSymbol)
{
    m_pSubSymbol = pSubSymbol;

    return this;
}

/******************************************************************************
 * Symbol::SetAttribute -- Set the attribute of the object.                   *
 *                                                                            *
 *    Here I used a simpleton pattern. The symbol will new an attribute at    *
 *    the first assignment. It will make a copy of the symbol.                *
 *                                                                            *
 * INPUT:   attribute -- The attribute to assign. This should better not come *
 *                       from new.                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
Symbol* Symbol::SetAttribute(SymbolAttribute* attribute)
{
    if (!m_pAttribute)
    {
        m_pAttribute = new SymbolAttribute();
    }

    *m_pAttribute = *attribute;

    return this;
}

/******************************************************************************
 * Symbol::GetAttribute -- Get the attribute of the symbol.                   *
 *                                                                            *
 *    Here a simpleton pattern is used. If the symbol doesn't have one, then  *
 *    it will new an instance.                                                *
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
SymbolAttribute* Symbol::GetAttribute()
{
    if (!m_pAttribute)
    {
        m_pAttribute = new SymbolAttribute();
    }

    return m_pAttribute;
}

/******************************************************************************
 * Symbol::ClearAttribute -- Clear the attribute.                             *
 *                                                                            *
 *    Well, clear the attribute. :?                                           *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Added delete. :(                                       *
 *============================================================================*/
Symbol* Symbol::ClearAttribute()
{
    if (m_pAttribute)
    {
        delete m_pAttribute;
        m_pAttribute = nullptr;
    }

    return this;
}

/******************************************************************************
 * Symbol::SetImage -- Set the image of the symbol.                           *
 *                                                                            *
 *    Well, this will remove the previous image.                              *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Created.                                               *
 *============================================================================*/
Symbol* Symbol::SetImage(IMAGE* image)
{
    if (m_pImage && !m_isStatic)
    {
        delete m_pImage;
    }

    m_pImage = image;

    return this;
}

/******************************************************************************
 * Symbol::GetImage -- Get the image of the symbol.                           *
 *                                                                            *
 *    Well, here a simple singleton is used.                                  *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return the image.                                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Created.                                               *
 *============================================================================*/
IMAGE* Symbol::GetImage()
{
    if (!m_pImage)
    {
        m_pImage = new IMAGE();
        m_isStatic = false;
    }

    return m_pImage;
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Miscellaneous Region
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

#if 0
/******************************************************************************
 * TextToSymbol -- Convert text to symbol.                                    *
 *                                                                            *
 *    If no symbol assigned, then it will new a symbol. It will only create   *
 *    a plain symbol.                                                         *
 *                                                                            *
 * INPUT:   symbol -- The target symbol.                                      *
 *          text   -- The text to set.                                        *
 *          width  -- The width of the symbol image.                          *
 *          height -- The height of the symbol image.                         *
 *          style  -- The text style.                                         *
 *                                                                            *
 * OUTPUT:  Return the result symbol.                                         *
 *                                                                            *
 * WARNINGS:  Pay great attention to the new symbol. If original image exists,*
 *            it should not be static.                                        *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
Symbol* TextToSymbol(const std::wstring& text, int width, int height, TextStyle* style)
{
	return TextToSymbol(new Symbol(), text, width, height, style);
}

Symbol* TextToSymbol(Symbol* symbol, const std::wstring& text, int width, int height, TextStyle* style)
{
	symbol->GetImage()->Resize(width, height);
	
	SetWorkingImage(symbol->GetImage());
	setbkcolor(TRANSPARENT_COLOR);
	cleardevice();

	if (style)
	{
		settextstyle(style->height, style->width, style->face.c_str());
		settextcolor(style->color);
	}

	RECT rect{ 0, 0, width, height };
	drawtext(text.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_NOCLIP);

	SetWorkingImage(NULL);

	return symbol;
}
#endif // #if 0
