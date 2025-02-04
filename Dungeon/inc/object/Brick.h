/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Brick.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 24, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For base class of brick in game.                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _BRICK_H_
#define _BRICK_H_

#include "Object.h"

enum class BrickType
{
    BRICK_FLOOR = 0,
    BRICK_WALL,
    BRICK_GATE
};

class Brick : public Object
{
public:
    Brick(Scene* scene) : Object(OBJ_BRICK, scene)
    {
        m_symbol.SetLayer(LAYER_BRICK);
    }

    ~Brick() override
    {
    }

    virtual BrickType GetBrickType() const = 0;

    Brick* Clone() const override = 0;
    virtual void Clone(Brick* clone) const;

    bool Load(XMLElement* node) override;

protected:
    void _InitBehavior(XMLElement* node = nullptr) override
    {
    }
};

class Wall : public Brick
{
public:
    Wall(Scene* scene) : Brick(scene)
    {
    }

    ~Wall() override
    {
    }

    BrickType GetBrickType() const override
    {
        return BrickType::BRICK_WALL;
    }

    Wall* Clone() const override;

    virtual void Clone(Wall* clone) const
    {
    }

    bool Load(XMLElement* node) override;

protected:
    void _InitBehavior(XMLElement* node = nullptr) override
    {
    }
};

class Gate : public Brick
{
public:
    Gate(Scene* scene) : Brick(scene), m_isOpen(false), m_isClosed(true)
    {
    }

    ~Gate() override
    {
    }

    BrickType GetBrickType() const override
    {
        return BrickType::BRICK_GATE;
    }

    Gate* Clone() const override;

    virtual void Clone(Gate* clone) const
    {
    }

    bool Load(XMLElement* node) override;

public:
    void Open();
    void Close();

    void SetOpen(bool isOpen)
    {
        m_isOpen = isOpen;
    }

    void SetClosed(bool isClosed)
    {
        m_isClosed = isClosed;
    }

    bool IsOpen() const
    {
        return m_isOpen;
    }

    bool IsClosed() const
    {
        return m_isClosed;
    }

private:
    void _InitBehavior(XMLElement* node = nullptr) override;

    bool m_isOpen;
    bool m_isClosed;
};

#endif
