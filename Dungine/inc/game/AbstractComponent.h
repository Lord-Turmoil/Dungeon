/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : AbstractComponent.h                       *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 7, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Provide basic components. The components in this files are not complete, *
 *   they must be inherited.                                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _COMPONENT_BASE_H_
#define _COMPONENT_BASE_H_

#include <unordered_map>

#include "../common/Common.h"

#include "../utility/Animation.h"
#include "GameObject.h"

class Event;

/********************************************************************
** Base class of all components.
** WARNING, all components MUST have a corresponding game object!
*/
class AbstractComponent : public AbstractObject
{
public:
    AbstractComponent(int updateOrder = 0)
        : AbstractObject(RTTIType::RTTI_COMPONENT), m_updateOrder(updateOrder), m_pGameObject(nullptr)
    {
    }

    ~AbstractComponent() override
    {
    }

    static const char* StaticName()
    {
        return "";
    }

    virtual const char* Name()
    {
        return StaticName();
    }

    int UpdateOrder() const
    {
        return m_updateOrder;
    }

    AbstractComponent* Clone() const override = 0;
    virtual void Clone(AbstractComponent* clone) const;

    bool Load(XMLElement* node) override;

    virtual void Update(Event* evnt) = 0;

public:
    virtual void Translate(const Coordinate& offset)
    {
    }

    virtual void SetGameObject(GameObject* object)
    {
        m_pGameObject = object;
    }

    GameObject* GetGameObject()
    {
        return m_pGameObject;
    }

protected:
    int m_updateOrder;

    /*
    ** The game object that possess the component. For component
    ** interaction.
    */
    GameObject* m_pGameObject;
};

#endif
