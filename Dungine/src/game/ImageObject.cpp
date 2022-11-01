/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : ImageObject.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 14, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Wrap image to an object linked with resource.                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Common.h"
#include "../../inc/game/ImageObject.h"


/******************************************************************************
 * ImageObject::~ImageObject -- Destructor of the object.                     *
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
 *   2022/07/14 Tony : Created.                                               *
 *============================================================================*/
ImageObject::~ImageObject()
{
	if (_IsPrototype())
	{
		if (m_pResource)
			m_pResource->Release();
	}
}


/******************************************************************************
 * ImageObject::Clone -- Clone image object.                                  *
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
 *   2022/07/14 Tony : Created.                                               *
 *============================================================================*/
ImageObject* ImageObject::Clone() const
{
	ImageObject* clone = new ImageObject();
	clone->_MakePrototype(false);

	AbstractObject::Clone(clone);

	clone->m_pImage = m_pImage;
	clone->m_pResource = m_pResource;

	return clone;
}


/******************************************************************************
 * ImageObject::Load -- Load image object.                                    *
 *                                                                            *
 *    Hmm... Since the name of image object is not certain, here makes no     *
 *    check for it.                                                           *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return result.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/14 Tony : Created.                                               *
 *============================================================================*/
bool ImageObject::Load(XMLElement* node)
{
/*
**	<TagName src=""></TagName>
*/
	if (!node)
		return false;

	if (m_pResource)
	{
		m_pResource->Release();
		m_pImage = nullptr;
		m_pResource = nullptr;
	}

	const char* name = node->Name();
	const char* src;

	src = node->Attribute("src");
	if (!src)
	{
		LOG_ERROR(MISSING_ATTRIBUTE, "src", name);
		return false;
	}

	m_pResource = LoadResource<ImageResource>(src);
	m_pImage = m_pResource->GetResource();

	return true;
}