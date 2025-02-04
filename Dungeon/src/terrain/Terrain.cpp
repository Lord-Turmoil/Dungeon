/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Terrain.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 13 , 2022                            *
 *                                                                            *
 *                    Last Update : December 16, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   The terrain of the game, in fact the map. It stores the arenas and the   *
 *   bridges, and handles path finding.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/terrain/Terrain.h"
#include "../../inc/terrain/Prim.h"

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Settings.h"

#include "../../inc/object/Hero.h"

/******************************************************************************
 * PlainTerrain::GetArena -- Get arena by id in plain terrain.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   id                                                                *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/14 Tony : Created.                                               *
 *============================================================================*/
PlainArena* PlainTerrain::GetArena(int id)
{
    for (auto it = arenas.begin(); it != arenas.end(); it++)
    {
        if (it->id == id)
        {
            return &(*it);
        }
    }

    LOG_ERROR(INTERNAL_ERROR);

    return nullptr;
}

/******************************************************************************
 * Terrain::Terrain -- Constructor of the object.                             *
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
Terrain::Terrain() : m_pDungeon(nullptr), m_curSpace(nullptr)
{
    m_miniMap.GetImage()->Resize(MINI_MAP_SIZE, MINI_MAP_SIZE);
    m_miniMap.SetCoord(deviceInfo.clientWidth - static_cast<int>(MINI_MAP_SIZE * 1.5),
                       static_cast<int>(MINI_MAP_SIZE * 0.5));
    m_miniMap.GetAttribute()->alpha = 220;
    m_miniMap.SetLayer(LAYER_MINI_MAP);

    Device::GetInstance()->SetTargetImage(m_miniMap.GetImage());
    setbkcolor(TRANSPARENT_COLOR);
    // setaspectratio(ASPECT_RATIO, ASPECT_RATIO);
    cleardevice();
    Device::GetInstance()->SetTargetImage();
}

/******************************************************************************
 * Terrain::~Terrain -- Destructor of the object.                             *
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
Terrain::~Terrain()
{
    _ClearUp();
}

/******************************************************************************
 * Terrain::Update -- Update terrain.                                         *
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
void Terrain::Update()
{
    Coordinate target = m_pDungeon->GetHero()->GetCoord();

    if (!m_curSpace->InRange(target))
    {
        m_curSpace->Attribute().isCurrent = false;
        for (auto it = m_spaces.begin(); it != m_spaces.end(); it++)
        {
            if ((*it)->InRange(target))
            {
                m_curSpace = *it;
                m_curSpace->Attribute().isCurrent = true;
                m_curSpace->Discover();

                m_pDungeon->GetQuadTree()->Initialize(m_curSpace->GetBorder());

                break;
            }
        }
    }

    for (auto it = m_spaces.begin(); it != m_spaces.end(); it++)
    {
        (*it)->Update();
    }

    _UpdateMiniMap();
}

/******************************************************************************
 * Terrain::Draw -- Draw terrain.                                             *
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
 *   2022/12/16 Tony : If boss exists, mini map won't be drawn.               *
 *============================================================================*/
void Terrain::Draw()
{
    for (auto it = m_spaces.begin(); it != m_spaces.end(); it++)
    {
        (*it)->Draw();
    }

    if (!m_pDungeon->GetBoss())
    {
        Device::GetInstance()->AddSymbol(&m_miniMap);
    }
}

void Terrain::Draw(Camera* camera)
{
    for (auto it = m_spaces.begin(); it != m_spaces.end(); it++)
    {
        (*it)->Draw(camera);
    }

    if (!m_pDungeon->GetBoss())
    {
        Device::GetInstance()->AddSymbol(&m_miniMap);
    }
}

/******************************************************************************
 * Terrain::Load -- Load terrrain.                                            *
 *                                                                            *
 *    Load resources in terrain.                                              *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return result.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
bool Terrain::Load(XMLElement* node)
{
    /*
    **	<Terrain>
    **		<Wall>...</Wall>
    **		<Wall>...</Wall>
    **		<Gate>...</Gate>
    **		<Gate>...</Gate>
    **		<Floor></Floor>
    **		<Floor></Floor>
    *		...
    **	</Terrain>
    */
    if (!node)
    {
        return false;
    }

    return m_brickKit.Load(node);
}

/******************************************************************************
 * Terrain::UnLoad -- Unload terrrain.                                        *
 *                                                                            *
 *    Load resources in terrain.                                              *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return result.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Terrain::UnLoad()
{
    m_brickKit.UnLoad();
}

/******************************************************************************
 * Terrain::Generate -- Generate terrain.                                     *
 *                                                                            *
 *    The terrain is based on level and difficulty, difficulty is pre-loaded  *
 *    in Settings singleton.                                                  *
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
void Terrain::Generate(int level)
{
    int difficulty = Settings::GetInstance()->Difficulty();
    int arenaNum;
    bool isChapterEnd;

    _Initialize();

    isChapterEnd = (level == LEVEL_NUM - 1);
    if (isChapterEnd)
    {
        arenaNum = MAX_SPACE_NUM;
    }
    else
    {
        int lower = dmin(MIN_SPACE_NUM + level / 2 + difficulty / 2, MAX_SPACE_NUM);
        int upper = dmin(MIN_SPACE_NUM + level + difficulty / 2, MAX_SPACE_NUM);
        arenaNum = Random(lower, upper + 1);
    }

    // Really? Two lines of core algo? :P
    Prim::Generate(arenaNum);
    PlainTerrain* plain = Prim::GetPlainTerrain();

    // Generate arenas.
    auto& plainArenas = plain->arenas;
    for (auto it = plainArenas.begin(); it != plainArenas.end(); it++)
    {
        Arena* arena = new Arena(this);
        arena->Generate(&(*it));

        // Initialize enemy info.
        if (isChapterEnd && arena->Attribute().isEnd)
        {
            arena->Attribute().hasBoss = true;
            arena->Attribute().rounds = MAX_ENEMY_ROUNDS;
        }
        else if (arena->Attribute().isStart)
        {
            arena->Attribute().rounds = 0;
            m_curSpace = arena;
        }
        else
        {
            arena->Attribute().rounds = Random(MIN_ENENY_ROUNDS, MAX_ENEMY_ROUNDS);
        }

        m_spaces.push_back(arena);
    }

    // Generate bridges.
    auto& plainBridges = plain->bridges;
    for (auto it = plainBridges.begin(); it != plainBridges.end(); it++)
    {
        Bridge* bridge = new Bridge(this);
        bridge->Generate(&(*it));
        m_spaces.push_back(bridge);
    }

    m_curSpace->Attribute().isCompleted = true;
    m_curSpace->Attribute().isDiscovered = true;
    m_curSpace->Attribute().isCurrent = true;
    m_curSpace->Discover();

    _ClearMiniMap();
}

/******************************************************************************
 * Terrain::GetArena -- Get arena by id.                                      *
 *                                                                            *
 *    This id is of the matrix.                                               *
 *                                                                            *
 * INPUT:   id                                                                *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
Arena* Terrain::GetArena(int id)
{
    for (auto it = m_spaces.begin(); it != m_spaces.end(); it++)
    {
        if ((*it)->Type() == SpaceType::SPC_ARENA)
        {
            if (static_cast<Arena*>(*it)->ID() == id)
            {
                return static_cast<Arena*>(*it);
            }
        }
    }

    LOG_ERROR(INTERNAL_ERROR);

    return nullptr;
}

/******************************************************************************
 * Terrain::_Initialize -- Initialize terrain.                                *
 *                                                                            *
 *    Initialize terrain before each generation.                              *
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
void Terrain::_Initialize()
{
    for (auto it = m_spaces.begin(); it != m_spaces.end(); it++)
    {
        delete (*it);
    }
    m_spaces.clear();

    m_curSpace = nullptr;
}

/******************************************************************************
 * Terrain::_UpdateMiniMap -- Update and re-generate mini map.                *
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
void Terrain::_UpdateMiniMap()
{
    _ClearMiniMap();

    for (auto it = m_spaces.begin(); it != m_spaces.end(); it++)
    {
        if ((*it)->Type() == SpaceType::SPC_BRIDGE)
        {
            (*it)->DrawMiniMap();
        }
    }
    for (auto it = m_spaces.begin(); it != m_spaces.end(); it++)
    {
        if ((*it)->Type() == SpaceType::SPC_ARENA)
        {
            (*it)->DrawMiniMap();
        }
    }
}

/******************************************************************************
 * Terrain::_ClearMiniMap -- Clear mini map.                                  *
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
void Terrain::_ClearMiniMap()
{
    Device* device = Device::GetInstance();

    device->SetTargetImage(m_miniMap.GetImage());
    device->Clear();
    device->SetTargetImage();
}

/******************************************************************************
 * Terrain::ClearUp -- Release all resources and pointers.                    *
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
void Terrain::_ClearUp()
{
    for (auto it = m_spaces.begin(); it != m_spaces.end(); it++)
    {
        delete (*it);
    }
    m_spaces.clear();

    m_brickKit.UnLoad();
}
