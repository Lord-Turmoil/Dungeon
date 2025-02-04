/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Prim.h                                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 22, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Provide terrain generation algorithm.                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <algorithm>
#include <queue>

#include "../../inc/terrain/Prim.h"

PlainTerrain Prim::m_plain;

/******************************************************************************
 * PlainBridge::PlainBridge -- Constructor of the object.                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   _u                                                                *
 *          _v                                                                *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
PlainBridge::PlainBridge(int _u, int _v)
{
    u = _u;
    v = _v;

    if (u > v)
    {
        std::swap(u, v);
    }

    if (IsHorizontal())
    {
        gate = DIR_LEFT | DIR_RIGHT;
    }
    else if (IsVertical())
    {
        gate = DIR_UP | DIR_DOWN;
    }
    else
    {
        gate = DIR_NONE; // it should never reach here!
    }

    // u and v will be automatically adjust to u < v
}

/******************************************************************************
 * PlainBridge::IsHorizontal -- Check if the bridge is horizontal or not.     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return bool...                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
bool PlainBridge::IsHorizontal()
{
    return ((v % MAP_SIZE != 0) && (v == u + 1));
}

/******************************************************************************
 * PlainBridge::IsVertical -- Check if the bridge is vertical or not.         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return bool...                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
bool PlainBridge::IsVertical()
{
    return (v == u + MAP_SIZE);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Prim Region
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

static PrimEdge edge[EDGE_NUM * 2 + 1];
static int head[VERTEX_NUM];
static int edgestamp;

/******************************************************************************
 * Prim::Generate -- Generate plain terrain.                                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   vertexNum -- How many vertexes needed.                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/22 Tony : Created.                                               *
 *============================================================================*/
void Prim::Generate(int vertexNum)
{
    int dist[VERTEX_NUM];   // distance to each vertex from src
    int degree[VERTEX_NUM]; // degree of vertex
    bool visited[VERTEX_NUM];
    bool edgeFlag[EDGE_NUM + 1]; // which edge id is selected
    std::priority_queue<PrimToken, std::vector<PrimToken>, ComparePrimToken> frontier;

    _Initialize();
    memset(dist, 0x3f, sizeof(dist));
    memset(degree, 0, sizeof(degree));
    memset(visited, 0, sizeof(visited));
    memset(edgeFlag, 0, sizeof(edgeFlag));

    int src = Random(0, VERTEX_NUM);
    int u, v, w, id;
    int cnt;
    PrimToken token;

    token.u = -1;
    token.v = src;
    token.weight = dist[src] = 0;
    token.id = -1;

    frontier.push(token);
    cnt = 0;
    while (!frontier.empty())
    {
        token = frontier.top();
        u = token.u;
        v = token.v;
        id = token.id;
        frontier.pop();

        if (visited[v])
        {
            continue;
        }
        visited[v] = true;

        // Add arena and bridge.
        m_plain.arenas.push_back(PlainArena(v));
        cnt++;
        if ((id != -1) && (!edgeFlag[id]))
        {
            degree[u]++;
            degree[v]++;
            m_plain.bridges.push_back(PlainBridge(u, v));
            edgeFlag[id] = true;
        }
        if (cnt == vertexNum)
        {
            break;
        }

        for (int i = head[v]; i != 0; i = edge[i].next)
        {
            u = edge[i].u; // These two should be the same.
            v = edge[i].v;
            w = edge[i].weight;
            if (dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                token.weight = edge[i].weight;
                token.u = u;
                token.v = v;
                token.id = edge[i].id;

                frontier.push(token);
            }
        }
    }

    // Set arena's gate side.
    for (auto it = m_plain.bridges.begin(); it != m_plain.bridges.end(); it++)
    {
        if (it->IsHorizontal())
        {
            m_plain.GetArena(it->u)->gate |= DIR_RIGHT;
            m_plain.GetArena(it->v)->gate |= DIR_LEFT;
        }
        else
        {
            m_plain.GetArena(it->u)->gate |= DIR_DOWN;
            m_plain.GetArena(it->v)->gate |= DIR_UP;
        }
    }

    // Generate start and end.
    std::vector<PlainArena*> candidates;

    for (auto it = m_plain.arenas.begin(); it != m_plain.arenas.end(); it++)
    {
        if (degree[it->id] == 1)
        {
            candidates.push_back(&(*it));
        }
    }

    if (candidates.size() == 1)
    {
        candidates[0]->isStart = candidates[0]->isEnd = true;
    }
    else
    {
        std::random_shuffle(candidates.begin(), candidates.end());
        candidates[0]->isStart = true;
        candidates[1]->isEnd = true;
    }
}

/******************************************************************************
 * Prim::_Initialize -- Initialize for prim.                                  *
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
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void Prim::_Initialize()
{
    // clear old graph info
    m_plain.arenas.clear();
    m_plain.bridges.clear();

    // re-establish random edge
    edgestamp = 0;
    memset(head, 0, sizeof(head));

    int id = 0;
    for (int i = 0; i < MAP_SIZE - 1; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            _AddEdge(i + j * MAP_SIZE, i + j * MAP_SIZE + 1, id++);
            _AddEdge(j + i * MAP_SIZE, j + i * MAP_SIZE + MAP_SIZE, id++);
        }
    }
}

/******************************************************************************
 * Prim::_AddEdge -- Add edge in both way.                                    *
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
 *   2022/06/22 Tony : Created.                                               *
 *============================================================================*/
void Prim::_AddEdge(int u, int v, int id)
{
    _AddSingleEdge(u, v, id);
    _AddSingleEdge(v, u, id);
}

/******************************************************************************
 * Prim::_AddSingleEdge -- Add edge in single both way.                       *
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
 *   2022/06/22 Tony : Created.                                               *
 *============================================================================*/
void Prim::_AddSingleEdge(int u, int v, int id)
{
    edgestamp++;
    edge[edgestamp].id = id;
    edge[edgestamp].u = u;
    edge[edgestamp].v = v;
    edge[edgestamp].weight = Random(MAX_EDGE_WEIGHT);
    edge[edgestamp].next = head[u];
    head[u] = edgestamp;
}
