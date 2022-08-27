/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Space.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 11, 2022                             *
 *                                                                            *
 *                    Last Update : August 12, 2022                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The space of the terrain is where the event is currently taking place.   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/game/Dungeon.h"

#include "../../inc/object/Hero.h"

#include "../../inc/terrain/Space.h"
#include "../../inc/terrain/Terrain.h"
#include "../../inc/terrain/Graph.h"


/******************************************************************************
 * Space::Space -- Constructor of the object.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   type    -- Type of the space.                                     *
 *          terrain -- Parent terrain of the space.                           *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Space::Space(SpaceType type, Terrain* terrain) :
	m_type(type), m_pTerrain(terrain), m_id(-1)
{
	m_floor.SetLayer(LAYER_FLOOR);
}


/******************************************************************************
 * Space::~Space -- Destructor of the object.                                 *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Space::~Space()
{
	m_bricks.Destroy();
}


/******************************************************************************
 * Space::InRange -- Check if given coordinate in the space or not.           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   target                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
bool Space::InRange(const Coordinate& target)
{
	return InRectangle(m_rect, target);
}


/******************************************************************************
 * Space::Draw -- Draw space.                                                 *
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
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
void Space::Draw()
{
	Device::GetInstance()->AddSymbol(&m_floor);
	m_bricks.Draw();
}

void Space::Draw(Camera* camera)
{
	if (camera->InRange(GetBorder()))
	{
		camera->Capture(&m_floor);
		m_bricks.Draw(camera);
	}
}


/******************************************************************************
 * Space::DrawMiniMap -- Draw space to minimap.                               *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Space::DrawMiniMap()
{
	IMAGE* tarImage = m_pTerrain->GetMiniMap()->GetImage();

	// This image already set aspectratio.
	Device::GetInstance()->SetTargetImage(tarImage);

	Rect rect = m_rect * ASPECT_RATIO;
	if (m_attr.isDiscovered)
	{
		// background
		if (m_attr.isCurrent)
		{
			if (m_attr.isCompleted)
				setfillcolor(0x00C9EE);
			else
				setfillcolor(0x9EB7CD);
		}
		else if (m_attr.isCompleted)
			setfillcolor(0x00FC7C);
		else
			setfillcolor(0x6B7D8B);
		// solidrectangle(m_rect.x, m_rect.y, m_rect.x + m_rect.width, m_rect.y + m_rect.height);
		solidrectangle(rect.x, rect.y, rect.x + rect.width, rect.y + rect.height);

		// decoration
		if (m_attr.isEnd)
		{
			if (m_attr.hasBoss)
				setfillcolor(0x0000EE);
			else
				setfillcolor(0x00FFFF);
			// solidcircle(GetCenter().x, GetCenter().y, (int)(min(m_rect.width, m_rect.height) * 0.4));
			solidcircle(rect.GetCenter().x, rect.GetCenter().y, (int)(min(rect.width, rect.height) * 0.4));
		}
	}

	Device::GetInstance()->SetTargetImage();
}


/******************************************************************************
 * Space::Discover -- Discover current space.                                 *
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
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
void Space::Discover()
{
	m_attr.isDiscovered = true;
}


/******************************************************************************
 * Space::_CoordToSub -- Utility convertor.                                   *
 *                                                                            *
 *    Convert actual coordinate to subscription in graph's matrix.            *
 *                                                                            *
 * INPUT:   pos                                                               *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Coordinate Space::_CoordToSub(const Coordinate& coord)
{
	Coordinate sub;
	
	sub = coord - m_rect.pos;
	sub /= UNIT_WIDTH;

	return sub;
}


/******************************************************************************
 * Space::_SubToCoord -- Utility convertor.                                   *
 *                                                                            *
 *    Convert subscription of graph's matrix to actual coordinate.            *
 *                                                                            *
 * INPUT:   sub                                                               *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Coordinate Space::_SubToCoord(const Coordinate& sub)
{
	return m_rect.pos + sub * UNIT_WIDTH;
}


/******************************************************************************
 * Space::_SubToCoordCenter -- Utility convertor.                             *
 *                                                                            *
 *    Convert subscription of graph's matrix to actual coordinate.            *
 *                                                                            *
 * INPUT:   sub                                                               *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Coordinate Space::_SubToCoordCenter(const Coordinate& sub)
{
	return m_rect.pos + sub * UNIT_WIDTH + UNIT_HALF_OFFSET;
}


/******************************************************************************
 * Space::_Render -- Render space.                                            *
 *                                                                            *
 *    This load resource to the space.                                        *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Space::_Render(BrickMatrix& matrix)
{
	int width = (int)(matrix[0].size());
	int height = (int)matrix.size();

	Coordinate offset(UNIT_WIDTH >> 1);
	Coordinate base = m_rect.pos + offset;
	Coordinate pos = base;
	BrickKit* kit = m_pTerrain->GetBrickKit();

	Wall* wall;
	Gate* gate;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (matrix[i][j] == BrickType::BRICK_WALL)
			{
				wall = kit->GetWall()->Clone();
				wall->SetCoord(pos);
				wall->Update(nullptr);
				m_bricks.AddObject(wall);
			}
			else if (matrix[i][j] == BrickType::BRICK_GATE)
			{
				gate = kit->GetGate()->Clone();
				gate->SetCoord(pos);
				gate->Update(nullptr);
				m_bricks.AddObject(gate);
			}
			pos.x += UNIT_WIDTH;
		}
		pos.x = base.x;
		pos.y += UNIT_WIDTH;
	}

	// Generate floor
	m_floor.SetCoord(m_rect.pos);
	m_floor.GetImage()->Resize(m_rect.width, m_rect.height);
	Device::GetInstance()->SetTargetImage(m_floor.GetImage());

	pos.Init(COORD_ZERO);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			putimage(pos.x, pos.y, kit->GetFloorImage());
			pos.x += UNIT_WIDTH;
		}
		pos.x = 0;
		pos.y += UNIT_WIDTH;
	}

	Device::GetInstance()->SetTargetImage();
}


/******************************************************************************
 * Arena::Arena -- Constructor of the object.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   terrain -- See above.                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Arena::Arena(Terrain* terrain) : Space(SpaceType::SPC_ARENA, terrain),
	m_isActivated(false), m_isLocked(false)
{
}


/******************************************************************************
 * Arena::~Arena -- Destructor of the object.                                 *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Arena::~Arena() {}


/******************************************************************************
 * Arena::Generate -- Generate arena.                                         *
 *                                                                            *
 *    In this version, space is set to square.                                *
 *                                                                            *
 * INPUT:   node  -- Corresponding xml node.                                  *
 *          arena -- plain arena.                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Arena::Generate(PlainSpace* spc)
{
	PlainArena* arena = static_cast<PlainArena*>(spc);

	m_id = arena->id;
	m_attr.isStart = arena->isStart;
	m_attr.isEnd = arena->isEnd;

	if (m_attr.isStart)
		m_isActivated = true;

	// Generate size and position of the arena.
	int size;
	Coordinate sub;
	
	if (arena->isStart)
		size = START_SPACE_SIZE;
	else if (arena->isEnd)
		size = END_SPACE_SIZE;
	else
		size = to_even(Random(MIN_SPACE_SIZE, MAX_SPACE_SIZE));
	m_rect.width = m_rect.height = size * UNIT_WIDTH;
	
	sub.x = arena->id % MAP_SIZE;
	sub.y = arena->id / MAP_SIZE;
	m_rect.pos.x = (sub.x * SPACE_WRAPPER_SIZE + ((SPACE_WRAPPER_SIZE - size) >> 1)) * UNIT_WIDTH;
	m_rect.pos.y = (sub.y * SPACE_WRAPPER_SIZE + ((SPACE_WRAPPER_SIZE - size) >> 1)) * UNIT_WIDTH;

	// Generate a matrix, then turn the matrix to arena.
	BrickMatrix matrix(size, BrickRow(size));
	unsigned gate = arena->gate;
	int k;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if ((i == 0) || (i == size - 1) || (j == 0) || (j == size - 1))
				matrix[i][j] = BrickType::BRICK_WALL;
			else
				matrix[i][j] = BrickType::BRICK_FLOOR;
		}
	}
	if (gate & DIR_LEFT)
	{
		k = (size - GATE_SIZE) >> 1;
		for (int i = 0; i < GATE_SIZE; i++)
			matrix[k + i][0] = BrickType::BRICK_GATE;
	}
	if (gate & DIR_RIGHT)
	{
		k = (size - GATE_SIZE) >> 1;
		for (int i = 0; i < GATE_SIZE; i++)
			matrix[k + i][size - 1] = BrickType::BRICK_GATE;
	}
	if (gate & DIR_UP)
	{
		k = (size - GATE_SIZE) >> 1;
		for (int i = 0; i < GATE_SIZE; i++)
			matrix[0][k + i] = BrickType::BRICK_GATE;
	}
	if (gate & DIR_DOWN)
	{
		k = (size - GATE_SIZE) >> 1;
		for (int i = 0; i < GATE_SIZE; i++)
			matrix[size - 1][k + i] = BrickType::BRICK_GATE;
	}

 	_GenerateObstacle(matrix);
	_Render(matrix);

	// Initialize graph.
	m_graph.Initialize(size, size);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (matrix[i][j] != BrickType::BRICK_FLOOR)
				m_graph.EntryObstacle({ j, i });
		}
	}

	// Find all gates.
	auto& pool = m_bricks.GetPool();
	for (auto it = pool.begin(); it != pool.end(); it++)
	{
		if (static_cast<Brick*>(*it)->GetBrickType() == BrickType::BRICK_GATE)
			m_gates.push_back(static_cast<Gate*>(*it));
	}

	_UnLock();
}


/******************************************************************************
 * Arena::Update -- Update arena.                                             *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Arena::Update()
{
	if (!m_isActivated)
	{
		Hero* hero = static_cast<Hero*>(m_pTerrain->GetParent()->GetHero());
		if (_InTrap(hero->GetCoord()))
		{
			m_isActivated = true;
			_Lock();
		}
	}
	else
	{
		if (m_attr.isCompleted && m_isLocked)
			_UnLock();
	}

	m_bricks.Update(nullptr);
	m_floor.SetCoord(m_rect.pos);
}


/******************************************************************************
 * Arena::Discover -- Discover space and neighbors.                           *
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
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
void Arena::Discover()
{
	for (auto it = m_neighbors.begin(); it != m_neighbors.end(); it++)
	{
		(*it)->Attribute().isDiscovered = true;
	}
}


/******************************************************************************
 * Arena::Entry -- Entry a coordinate to the graph.                           *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Arena::Entry(const Coordinate& coord)
{
	m_graph.EntryObject(_CoordToSub(coord));
}


/******************************************************************************
 * Arena::Entry -- Entry an area to the graph.                                *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Arena::Entry(const Coordinate& topLeft, const Coordinate& bottomRight)
{
	m_graph.EntryObject(_CoordToSub(topLeft), _CoordToSub(bottomRight));
}


/******************************************************************************
 * Arena::DeEntry -- DeEntry a coordinate in the graph.                       *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Arena::DeEntry(const Coordinate& coord)
{
	m_graph.DeEntryObject(_CoordToSub(coord));
}


/******************************************************************************
 * Arena::DeEntry -- DeEntry an area in the graph.                            *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Arena::DeEntry(const Coordinate& topLeft, const Coordinate& bottomRight)
{
	m_graph.DeEntryObject(_CoordToSub(topLeft), _CoordToSub(bottomRight));
}


/******************************************************************************
 * Arena::Wander -- Wander with no aim.                                       *
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
 *   2022/07/29 Tony : Created.                                               *
 *============================================================================*/
Coordinate Arena::Wander(const Coordinate& src)
{
	if (m_graph.Wander(_CoordToSub(src)))
		return _SubToCoordCenter(m_graph.Next());
	else
		return src;
}


/******************************************************************************
 * Arena::Engage -- Engagee.                                                  *
 *                                                                            *
 *    Get the next move to advance.                                           *
 *                                                                            *
 * INPUT:   src                                                               *
 *          dest                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Coordinate Arena::Engage(const Coordinate& src, const Coordinate& dest)
{
	if (m_graph.Engage(_CoordToSub(src), _CoordToSub(dest)))
		return _SubToCoordCenter(m_graph.Next());
	else
		return src;
}


/******************************************************************************
 * Arena::Retreat -- Retreat.                                                 *
 *                                                                            *
 *    Get the next move to retreat.                                           *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Coordinate Arena::Retreat(const Coordinate& src, const Coordinate& dest)
{
	if (m_graph.Retreat(_CoordToSub(src), _CoordToSub(dest)))
		return _SubToCoordCenter(m_graph.Next());
	else
		return src;
}


/******************************************************************************
 * Arena::FindBlank -- Find a blank area.                                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   width                                                             *
 *          height                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Coordinate Arena::FindBlank(int width, int height)
{
	int w = width / UNIT_WIDTH + 2;
	int h = height / UNIT_WIDTH + 2;

	if (m_graph.FindBlank(w, h))
		return (_SubToCoord(m_graph.Blank()) + Coordinate(width >> 1, height >> 1));
	else
	{
		// No way!
		LOG_ERROR(INTERNAL_ERROR);
		return GetCoord();
	}
}

Coordinate Arena::FindBlank(const Coordinate& center, int width, int height)
{
	int w = width / UNIT_WIDTH + 2;
	int h = height / UNIT_WIDTH + 2;

	if (m_graph.FindBlank(_CoordToSub(center), w, h))
		return (_SubToCoord(m_graph.Blank()) + Coordinate(width >> 1, height >> 1));
	else
	{
		// No way!
		LOG_ERROR(INTERNAL_ERROR);
		return GetCoord();
	}
}


/******************************************************************************
 * Arena::InSight -- Check if dest is in sight of src.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   src                                                               *
 *          dest                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
bool Arena::InSight(const Coordinate& src, const Coordinate& dest)
{
	return m_graph.InSight(_CoordToSub(src), _CoordToSub(dest));
}


/******************************************************************************
 * Arena::Clear -- Clear added objects in the area.                           *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Arena::Clear()
{
	m_graph.Clear();
}


/******************************************************************************
 * Arena::AddNeighbor -- Add a neighbor to the arena.                         *
 *                                                                            *
 *    This will automatically check duplicate case.                           *
 *                                                                            *
 * INPUT:   spc                                                               *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Arena::AddNeighbor(Space* spc)
{
	for (auto it = m_neighbors.begin(); it != m_neighbors.end(); it++)
	{
		if ((*it) == spc)
			return;
	}

	m_neighbors.push_back(spc);
}


/******************************************************************************
 * Arena::_Lock -- Lock the arena.                                            *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Arena::_Lock()
{
	for (auto it = m_gates.begin(); it != m_gates.end(); it++)
		(*it)->Close();
	m_isLocked = true;
}


/******************************************************************************
 * Arena::_UnLock -- Unlock the arena.                                        *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Arena::_UnLock()
{
	for (auto it = m_gates.begin(); it != m_gates.end(); it++)
		(*it)->Open();
	m_isLocked = false;
}


/******************************************************************************
 * Arena::_InTrap -- Check if target is in trap or not.                       *
 *                                                                            *
 *    Check if the target is in range of activation.                          *
 *                                                                            *
 * INPUT:   target                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/-- Tony : Created.                                               *
 *============================================================================*/
bool Arena::_InTrap(const Coordinate& target)
{
	if (!InRectangle(m_rect, target))
		return false;

	int dist = dmin(
		ManhattanDist({ m_rect.pos.x, m_rect.pos.y + m_rect.height / 2 }, target),
		ManhattanDist({ m_rect.pos.x + m_rect.width, m_rect.pos.y + m_rect.height / 2 }, target),
		ManhattanDist({ m_rect.pos.x + m_rect.width / 2, m_rect.pos.y }, target),
		ManhattanDist({ m_rect.pos.x + m_rect.width / 2, m_rect.pos.y + m_rect.height }, target));

	if (dist > GATE_SIZE * UNIT_WIDTH)
		return true;
	else
		return false;
}


/******************************************************************************
 * Arena::_GenerateObstacle -- Generate obstacles in arena.                   *
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
 *   2022/07/-- Tony : Created.                                               *
 *============================================================================*/
void Arena::_GenerateObstacle(BrickMatrix& matrix)
{
	if (m_attr.isStart)
		return;
	else if (m_attr.isEnd)
		_PatternCorner(matrix);
	else
	{
		switch (Random(3))
		{
		case 0:
			_PatternLine(matrix);
			break;
		case 1:
			_PatternCorner(matrix);
			break;
		default:
			_PatternRandom(matrix);
			break;
		}
	}
}


/******************************************************************************
 * Arena::_PatternLine -- Line pattern of obstacle.                           *
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
 *   2022/08/12 Tony : Created.                                               *
 *============================================================================*/
void Arena::_PatternLine(BrickMatrix& matrix)
{
	int width = (int)matrix[0].size();
	int height = (int)matrix.size();
	int x, y;

	if (Random(2))	// vertical
	{
		x = Random(3, (width - GATE_SIZE) / 2);
		y = Random(3, height - 3);
		if (Random(2))
			x = width - x;
		if (Random(2))
		{
			for (int i = 0; i < y; i++)
				matrix[i][x] = BrickType::BRICK_WALL;
		}
		else
		{
			for (int i = y; i < height; i++)
				matrix[i][x] = BrickType::BRICK_WALL;
		}
	}
	else	// horizontal
	{
		x = Random(3, width - 2);
		y = Random(3, (height - GATE_SIZE) / 2 - 5);
		if (Random(2))
			y = height - y;
		if (Random(2))
		{
			for (int i = 0; i < x; i++)
				matrix[y][i] = BrickType::BRICK_WALL;
		}
		else
		{
			for (int i = x; i < width; i++)
				matrix[y][i] = BrickType::BRICK_WALL;
		}
	}
}


/******************************************************************************
 * Arena::_PatternCorner -- Corner pattern of obstacle.                       *
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
 *   2022/08/12 Tony : Created.                                               *
 *============================================================================*/
void Arena::_PatternCorner(BrickMatrix& matrix)
{
	int width = (int)matrix[0].size();
	int height = (int)matrix.size();
	int hMargin = (width - GATE_SIZE) / 2;
	int vMargin = (height - GATE_SIZE) / 2;
	int x, y;
	int length;

	// top left
	if (Random(4))
	{
		x = Random(3, hMargin);
		y = Random(3, vMargin);
		length = Random(1, hMargin);
		for (int i = 0; i < length; i++)
			matrix[y][x + i] = BrickType::BRICK_WALL;
		length = Random(1, vMargin);
		for (int i = 0; i < length; i++)
			matrix[y + i][x] = BrickType::BRICK_WALL;
	}
	// top right
	if (Random(4))
	{
		x = Random(hMargin + GATE_SIZE, width - 3);
		y = Random(3, vMargin);
		length = Random(1, hMargin);
		for (int i = 0; i < length; i++)
			matrix[y][x - i] = BrickType::BRICK_WALL;
		length = Random(1, vMargin);
		for (int i = 0; i < length; i++)
			matrix[y + i][x] = BrickType::BRICK_WALL;
	}
	// bottom left
	if (Random(4))
	{
		x = Random(3, hMargin);
		y = Random(vMargin + GATE_SIZE, height - 3);
		length = Random(1, hMargin);
		for (int i = 0; i < length; i++)
			matrix[y][x + i] = BrickType::BRICK_WALL;
		length = Random(1, vMargin);
		for (int i = 0; i < length; i++)
			matrix[y - i][x] = BrickType::BRICK_WALL;
	}
	// bottom right
	if (Random(4))
	{
		x = Random(hMargin + GATE_SIZE, width - 3);
		y = Random(vMargin + GATE_SIZE, height - 3);
		length = Random(1, hMargin);
		for (int i = 0; i < length; i++)
			matrix[y][x - i] = BrickType::BRICK_WALL;
		length = Random(1, vMargin);
		for (int i = 0; i < length; i++)
			matrix[y - i][x] = BrickType::BRICK_WALL;
	}
}


/******************************************************************************
 * Arena::_PatternRandom -- Random pattern of obstacle.                       *
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
 *   2022/08/12 Tony : Created.                                               *
 *============================================================================*/
void Arena::_PatternRandom(BrickMatrix& matrix)
{
	int width = (int)matrix[0].size();
	int height = (int)matrix.size();
	int centerX = width / 2;
	int centerY = height / 2;
	
	for (int i = height - 3; i > 1; i--)
	{
		for (int j = width - 3; j > 1; j--)
		{
			if (ManhattanDist(j, i, centerX, centerY) < GATE_SIZE)
				continue;

			if (Random(20) == 0)
				matrix[i][j] = BrickType::BRICK_WALL;
		}
	}
}


/******************************************************************************
 * Bidge::Bridge -- Constructor of the object.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   terrain -- See above.                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Bridge::Bridge(Terrain* terrain) : Space(SpaceType::SPC_BRIDGE, terrain)
{
}


/******************************************************************************
 * Bidge::~Bridge -- Destructor of the object.                                *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
Bridge::~Bridge()
{
}


/******************************************************************************
 * Bidge::Generate -- Generate bridge.                                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *          bridge                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Bridge::Generate(PlainSpace* spc)
{
	PlainBridge* bridge = static_cast<PlainBridge*>(spc);

	Arena* u = m_pTerrain->GetArena(bridge->u);
	Arena* v = m_pTerrain->GetArena(bridge->v);

	u->AddNeighbor(v);
	u->AddNeighbor(this);
	v->AddNeighbor(u);
	v->AddNeighbor(this);

	Rect uRect = u->GetBorder();
	Rect vRect = v->GetBorder();

	int x, y;
	int width, height;

	if (bridge->IsHorizontal())
	{
		// u is on the left of v
		x = uRect.pos.x + uRect.width;
		y = uRect.pos.y + (uRect.height / 2) - 4 * UNIT_WIDTH;
		width = vRect.pos.x - x;
		height = (GATE_SIZE + 2) * UNIT_WIDTH;
	}
	else
	{
		// u is on the top of v
		x = uRect.pos.x + (uRect.width / 2) - 4 * UNIT_WIDTH;
		y = uRect.pos.y + uRect.height;
		width = (GATE_SIZE + 2) * UNIT_WIDTH;
		height = vRect.pos.y - y;
	}

	m_rect.Reset({ x, y }, width, height);

	width /= UNIT_WIDTH;
	height /= UNIT_WIDTH;

	BrickMatrix matrix(height, BrickRow(width));

	if (bridge->IsHorizontal())
	{
		for (int i = 0; i < width; i++)
			matrix[0][i] = matrix[height - 1][i] = BrickType::BRICK_WALL;
	}
	else
	{
		for (int i = 0; i < height; i++)
			matrix[i][0] = matrix[i][width - 1] = BrickType::BRICK_WALL;
	}

	_Render(matrix);
}


/******************************************************************************
 * Bidge::Update -- Update bridge.                                            *
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
 *   2022/07/11 Tony : Created.                                               *
 *============================================================================*/
void Bridge::Update()
{
	m_bricks.Update(nullptr);
	m_floor.SetCoord(m_rect.pos);
}