/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Terrain.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 5, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The terrain of the game, in fact the map. It stores the arenas and the   *
 *   bridges, and handles path finding.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <vector>

#include <dungine.h>

#include "Space.h"
#include "../object/BrickKit.h"


/********************************************************************
** The standard size of game object is 64px * 64px square, and the
** sprite of it will have a height of 96px.
*/
const int UNIT_WIDTH = 64;
const int UNIT_HEIGHT = 96;
const Coordinate UNIT_OFFSET = { UNIT_WIDTH, UNIT_WIDTH };
const Coordinate UNIT_HALF_OFFSET = { UNIT_WIDTH >> 1, UNIT_WIDTH >> 1 };


/********************************************************************
** Some constants for terrain of campaign mode.
*/

// Map is a N * N square.
const int MAP_SIZE = 3;

const int MIN_SPACE_NUM = MAP_SIZE;
const int MAX_SPACE_NUM = MAP_SIZE * MAP_SIZE;

// sizes must be even!!!
const int SPACE_WRAPPER_SIZE = 40;
const int MIN_SPACE_SIZE = 20;
const int MAX_SPACE_SIZE = 30;
const int START_SPACE_SIZE = MIN_SPACE_SIZE;
const int END_SPACE_SIZE = MAX_SPACE_SIZE;
const int GATE_SIZE = 6;

const int LARGE_MAP_SIZE = MAP_SIZE * SPACE_WRAPPER_SIZE * UNIT_WIDTH;
const int MINI_MAP_SIZE = 128;
const float ASPECT_RATIO = (float)MINI_MAP_SIZE / (float)LARGE_MAP_SIZE;


struct PlainTerrain
{
	std::vector<PlainArena> arenas;
	std::vector<PlainBridge> bridges;

	PlainArena* GetArena(int id);
};

class Dungeon;

class Terrain
{
public:
	Terrain();
	~Terrain();

	void Update();
	void Draw();
	void Draw(Camera* camera);

	/*
	** To generate a map of certain properties.
	*/
	bool Load(XMLElement* node);
	void UnLoad();

	void Generate(int level);

public:
	void SetParent(Dungeon* dungeon) { m_pDungeon = dungeon; }
	Dungeon* GetParent() { return m_pDungeon; }

	Space* GetCurrentSpace() { return m_curSpace; }
	Arena* GetArena(int id);

	Symbol* GetMiniMap() { return &m_miniMap; }

	BrickKit* GetBrickKit() { return &m_brickKit; }

protected:
	void _Initialize();
	void _UpdateMiniMap();
	void _ClearMiniMap();

	void _ClearUp();
	
private:
	Dungeon* m_pDungeon;

	std::vector<Space*> m_spaces;
	Space* m_curSpace;

	Symbol m_miniMap;

	BrickKit m_brickKit;
};

#endif