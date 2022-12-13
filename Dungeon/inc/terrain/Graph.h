/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Graph.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 6, 2022                              *
 *                                                                            *
 *                    Last Update : August 13, 2022                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide all graph algorithms.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>

#include <dungine.h>


/********************************************************************
** The direction of each move, and their corresponding costs for
** AStar.
** 2022/08/13 TS:
** Here is a trick that figure may sometimes be stuck if he's gonna
** go diagonal, so DIR[] is carefully arranged. :)
** 
*/
const int DIR_NUM = 8;

const Coordinate DIR[8] = {
	{ -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 },
	{ -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 } };

const int DIR_COST[8] = { 2, 2, 2, 2, 3, 3, 3, 3 };

const int INF_COST = INF_INT;


enum class GridType
{
	GRID_OBJECT = 0,	// temporary obstacle
	GRID_OBSTACLE,
	GRID_PLAIN		// nothing.
};

struct Grid
{
	GridType type;
	int cost;
	Coordinate father;	// For path finding.

	Grid() : type(GridType::GRID_PLAIN), cost(INF_COST) {}
	
	void Reset()
	{
		cost = INF_COST;
		father = COORD_ZERO;
	}
};


/********************************************************************
** Token for A* algorithm.
*/
struct AStarToken
{
	Coordinate sub;	// subscription of the grid
	int priority;	// cost and heuritics

	AStarToken(Coordinate _sub = COORD_ZERO, int _priority = 0) :
		sub(_sub), priority(_priority) {}
};

// For shortest path heap.
struct AStarTokenGreater
{
	bool operator()(const AStarToken& t1, const AStarToken& t2) const
	{
		return t1.priority > t2.priority;
	};
};

// For long path heap.
struct AStarTokenLess
{
	bool operator()(const AStarToken& t1, const AStarToken& t2) const
	{
		return t1.priority < t2.priority;
	};
};


// Useful? I doubt that.
typedef std::vector<Grid> GridRow;
typedef std::vector<std::vector<Grid>> GridMatrix;

/********************************************************************
** Graph represents the raw state of a single space. Here, all units
** are subscription of 2D array. Although graph provides all methods,
** it needs an adapter to fit the space.
*/
class Graph
{
public:
	Graph() : m_width(0), m_height(0) {}
	~Graph() {}

	/*
	** Initialize the map with assigned size, and then
	** call the generate() to make it enclosed;
	*/
	void Initialize(int width, int height);

	/*
	** Clear everything in the map except the obstacles.
	*/
	void Clear();

	void EntryObstacle(const Coordinate& sub);
	void EntryObstacle(const Coordinate& topLeft, const Coordinate& bottomRight);

	void EntryObject(const Coordinate& sub);
	void EntryObject(const Coordinate& topLeft, const Coordinate& bottomRight);

	void DeEntryObstacle(const Coordinate& sub);
	void DeEntryObstacle(const Coordinate& topLeft, const Coordinate& bottomRight);

	void DeEntryObject(const Coordinate& sub);
	void DeEntryObject(const Coordinate& topLeft, const Coordinate& bottomRight);

	/*
	** Find a path and if there has, store the next
	** move int next.
	*/
	bool Wander(const Coordinate& src);
	bool Engage(const Coordinate& src, const Coordinate& dest);
	bool Retreat(const Coordinate& src, const Coordinate& dest);

	/*
	** Find a blank position in the graph.
	*/
	bool FindBlank(int width, int height);
	bool FindBlank(const Coordinate& center, int width, int height);

	/*
	** Check if the src can see the dest or not.
	*/
	bool InSight(const Coordinate& src, const Coordinate& dest);

public:
	Coordinate Next() const { return m_next; }
	Coordinate Blank() const { return m_blank; }

	int Width() const { return m_width; }
	int Height() const { return m_height; }

private:
	bool _IsLegal(const Coordinate& sub);

	int _Heuristic(const Coordinate& src, const Coordinate& dest);

	void _ClearPath();

private:
	int m_width;
	int m_height;

	Coordinate m_next;
	Coordinate m_blank;

	GridMatrix m_graph;

	std::vector<Coordinate> m_objectList;
};

#endif