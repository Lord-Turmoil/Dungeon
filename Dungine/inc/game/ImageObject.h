/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : ImageObject.h                             *
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

#ifndef _IMAGE_OBJECT_H_
#define _IMAGE_OBJECT_H_

#include "AbstractObject.h"
#include "../device/Explorer.h"

class ImageObject : public AbstractObject
{
public:
	ImageObject() : AbstractObject(RTTIType::RTTI_IMAGE),
		m_pImage(nullptr), m_pResource(nullptr) {}
	virtual ~ImageObject();

	virtual ImageObject* Clone() const;
	virtual void Clone(ImageObject* clone) const {}

	virtual bool Load(XMLElement* node);

	IMAGE* GetImage() { return m_pImage; }

private:
	IMAGE* m_pImage;
	ImageResource* m_pResource;
};

#endif