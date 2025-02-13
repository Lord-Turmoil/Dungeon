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
 * Overview:                                                                 *
 *   Wrap image to an object linked with resource.                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _IMAGE_OBJECT_H_
#define _IMAGE_OBJECT_H_

#include "../device/Explorer.h"
#include "AbstractObject.h"

class ImageObject : public AbstractObject
{
public:
    ImageObject() : AbstractObject(RTTIType::RTTI_IMAGE), m_pImage(nullptr), m_pResource(nullptr)
    {
    }

    ~ImageObject() override;

    ImageObject* Clone() const override;

    virtual void Clone(ImageObject* clone) const
    {
    }

    bool Load(XMLElement* node) override;

    IMAGE* GetImage()
    {
        return m_pImage;
    }

private:
    IMAGE* m_pImage;
    ImageResource* m_pResource;
};

#endif
