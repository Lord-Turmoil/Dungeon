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
 * Over View:                                                                 *
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
	Brick(Scene* scene) : Object(ObjectType::OBJ_BRICK, scene)
	{
		m_symbol.SetLayer(LAYER_BRICK);
	}
	virtual ~Brick() {}
	
	virtual BrickType GetBrickType() const = 0;

	virtual Brick* Clone() const = 0;
	virtual void Clone(Brick* clone) const;

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr) {}
};

class Wall : public Brick
{
public:
	Wall(Scene* scene) : Brick(scene) {}
	virtual ~Wall() {}

	virtual BrickType GetBrickType() const { return BrickType::BRICK_WALL; }

	virtual Wall* Clone() const ;
	virtual void Clone(Wall* clone) const {}

	virtual bool Load(XMLElement* node);

protected:
	virtual void _InitBehavior(XMLElement* node = nullptr) {}
};

class Gate : public Brick
{
public:
	Gate(Scene* scene) : Brick(scene), m_isOpen(false), m_isClosed(true) {}
	virtual ~Gate() {}

	virtual BrickType GetBrickType() const { return BrickType::BRICK_GATE; }

	virtual Gate* Clone() const;
	virtual void Clone(Gate* clone) const {}

	virtual bool Load(XMLElement* node);

public:
	void Open();
	void Close();

	void SetOpen(bool isOpen) { m_isOpen = isOpen; }
	void SetClosed(bool isClosed) { m_isClosed = isClosed; }
	bool IsOpen() const { return m_isOpen; }
	bool IsClosed() const { return m_isClosed; }

private:
	virtual void _InitBehavior(XMLElement* node = nullptr);
	
	bool m_isOpen;
	bool m_isClosed;
};

#endif