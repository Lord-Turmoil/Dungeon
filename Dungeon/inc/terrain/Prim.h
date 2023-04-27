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
 * Over View:                                                                 *
 *   Provide terrain generation algorithm.                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _PRIM_H_
#define _PRIM_H_

#include <vector>

#include <dungine.h>

#include "Space.h"
#include "Terrain.h"

/********************************************************************
** To avoid name conflict with std::map, I use terrain instead.
** Terrain is generated with randomized Prim. There are two types
** of terrains, one is in campaign mode, which is in a 3 * 3 room.
** The other is death mode, which is one large room.
*/


/********************************************************************
** Prim. The edge info will be declared in source file.
*/
struct PrimEdge
{
	int u;
	int v;
	int weight;
	int id;		// used to identify same edge with inverted u, v
	int next;
};

struct PrimToken
{
	int u;
	int v;
	int weight;
	int id;
};

struct ComparePrimToken
{
	bool operator()(const PrimToken& t1, const PrimToken& t2) const
	{
		// decreasing
		return t1.weight > t2.weight;
	};
};


const int EDGE_NUM = 2 * MAP_SIZE * (MAP_SIZE - 1);
const int VERTEX_NUM = MAP_SIZE * MAP_SIZE;
const int MAX_EDGE_WEIGHT = EDGE_NUM * EDGE_NUM;

class Prim
{
public:
	static void Generate(int vertexNum);
	
	static PlainTerrain* GetPlainTerrain() { return &m_plain; }

private:
	static void _Initialize();
	static void _AddEdge(int u, int v, int id);
	static void _AddSingleEdge(int u, int v, int id);
	
	static PlainTerrain m_plain;
};


#endif