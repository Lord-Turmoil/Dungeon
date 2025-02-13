/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Object.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 24, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For the base class of all game objects.                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <dungine.h>

#include "../common/Defines.h"

enum ObjectType : unsigned
{
    OBJ_BRICK = BIT(1), // wall and gate
    OBJ_BUFF = BIT(2),
    OBJ_BULLET = BIT(3),
    OBJ_CHEST = BIT(4),
    OBJ_CRATE = BIT(5),
    OBJ_ENEMY = BIT(6),
    OBJ_HERO = BIT(7),
    OBJ_WEAPON = BIT(8),
    OBJ_PORTAL = BIT(9),
    OBJ_STAND = BIT(10),

    OBJ_FIGURE = OBJ_ENEMY | OBJ_HERO,
};

/********************************************************************
** Object derives from GameObject, add custom type info.
*/
class Object : public GameObject
{
public:
    // Hmm... scene is a must.
    Object(ObjectType type, Scene* scene = nullptr) : GameObject(RTTIType::RTTI_OBJECT, scene), m_type(type)
    {
    }

    ~Object() override
    {
    }

    ObjectType Type() const
    {
        return m_type;
    }

    Object* Clone() const override = 0;
    virtual void Clone(Object* clone) const;

    bool Load(XMLElement* node) override;

    // virtual void Update(Event* evnt);
    // virtual void Draw();
    // virtual void Draw(IMAGE* pDestImage);

public:
    void SetCenter(const Coordinate& center) override
    {
        m_coord = center - m_centerOffset;
    }

    Coordinate GetCenter() const override
    {
        return m_coord + m_centerOffset;
    }

    void Translate(const Coordinate& offset) override;

protected:
    /*
    ** If the game object has behaviors, then they will be initailzed here.
    ** node is the game object's xml node.
    */
    virtual void _InitBehavior(XMLElement* node = nullptr)
    {
    }

    ObjectType m_type;

    Coordinate m_centerOffset;
};

#endif
