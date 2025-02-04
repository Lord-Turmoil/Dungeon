/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BrickKit.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 18, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Use factory pattern to generate bricks.                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/BrickKit.h"
#include "../../inc/game/Dungeon.h"

/******************************************************************************
 * BrickKit::~BrickKit -- Destructor of the object.                           *
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
 *   2022/07/18 Tony : Created.                                               *
 *============================================================================*/
BrickKit::~BrickKit()
{
    UnLoad();
}

/******************************************************************************
 * BrickKit::Load -- Load brick resource in a chapter.                        *
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
 *   20--/--/-- Tony : Created.                                               *
 *============================================================================*/
bool BrickKit::Load(XMLElement* node)
{
    /*
    **	<Terrain>
    **		<Wall src="">...</Wall>
    **		<Wall src="">...</Wall>
    **		<Gate src="">...</Gate>
    **		<Gate src="">...</Gate>
    **		<Floor src="">..</Floor>
    *		...
    **	</Terrain>
    */
    UnLoad();

    if (!_LoadWall(node) || !_LoadGate(node) || !_LoadFloorImage(node))
    {
        UnLoad();
        return false;
    }

    return true;
}

bool BrickKit::_LoadWall(XMLElement* node)
{
    std::vector<XMLElement*> wallList = GetElementsByTagName(node, "Wall");
    Wall* wall;

    for (auto it = wallList.begin(); it != wallList.end(); it++)
    {
        wall = new Wall(nullptr);
        if (!wall->Load(*it))
        {
            return false;
        }
        m_walls.push_back(wall);
    }

    return true;
}

bool BrickKit::_LoadGate(XMLElement* node)
{
    std::vector<XMLElement*> gateList = GetElementsByTagName(node, "Gate");
    Gate* gate;

    for (auto it = gateList.begin(); it != gateList.end(); it++)
    {
        gate = new Gate(nullptr);
        if (!gate->Load(*it))
        {
            return false;
        }
        m_gates.push_back(gate);
    }

    return true;
}

bool BrickKit::_LoadFloorImage(XMLElement* node)
{
    std::vector<XMLElement*> floorList = GetElementsByTagName(node, "Floor");
    ImageObject* floor;

    for (auto it = floorList.begin(); it != floorList.end(); it++)
    {
        floor = new ImageObject();
        if (!floor->Load(*it))
        {
            return false;
        }
        m_floors.push_back(floor);
    }

    return true;
}

/******************************************************************************
 * BrickKit::UnLoad -- Unload all resources.                                  *
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
 *   2022/07/18 Tony : Created.                                               *
 *============================================================================*/
void BrickKit::UnLoad()
{
    for (auto it = m_walls.begin(); it != m_walls.end(); it++)
    {
        delete (*it);
    }
    m_walls.clear();

    for (auto it = m_gates.begin(); it != m_gates.end(); it++)
    {
        delete (*it);
    }
    m_gates.clear();

    for (auto it = m_floors.begin(); it != m_floors.end(); it++)
    {
        delete (*it);
    }
    m_floors.clear();
}

/******************************************************************************
 * BrickKit::GetWall -- Get cloned wall.                                      *
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
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
Wall* BrickKit::GetWall()
{
    return m_walls[Random(static_cast<int>(m_walls.size()))];
}

/******************************************************************************
 * BrickKit::GetGate -- Get a cloned gate.                                    *
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
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
Gate* BrickKit::GetGate()
{
    return m_gates[Random(static_cast<int>(m_gates.size()))];
}

/******************************************************************************
 * BrickKit::GetFloorImage -- Get a single image of floor.                    *
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
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
IMAGE* BrickKit::GetFloorImage()
{
    return m_floors[Random(static_cast<int>(m_floors.size()))]->GetImage();
}
