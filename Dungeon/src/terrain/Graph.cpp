/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Graph.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 6, 2022                              *
 *                                                                            *
 *                    Last Update : August 12, 2022                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide all graph algorithms.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include <queue>

#include "../../inc/terrain/Graph.h"


/******************************************************************************
 * Graph::Initialize -- Initialize graph.                                     *
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
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
void Graph::Initialize(int width, int height)
{
	m_width = width;
	m_height = height;

	m_graph.clear();
	m_graph.resize(height, GridRow(width));
}


/******************************************************************************
 * Graph::Clear -- Clear non-obstacle objects.                                *
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
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
void Graph::Clear()
{
	for (auto it = m_objectList.begin(); it != m_objectList.end(); it++)
		m_graph[it->y][it->x].type = GridType::GRID_PLAIN;
	m_objectList.clear();
}


/******************************************************************************
 * Graph::EntryObstacle -- Entry constant obstacle                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   sub         --                                                    *
 *          topLeft     -- From top left to bottom right.                     *
 *          bottomRight -- From top left to bottom right.                     *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
void Graph::EntryObstacle(const Coordinate& sub)
{
	m_graph[sub.y][sub.x].type = GridType::GRID_OBSTACLE;
}

void Graph::EntryObstacle(const Coordinate& topLeft, const Coordinate& bottomRight)
{
	for (int i = topLeft.y; i <= bottomRight.y; i++)
	{
		for (int j = topLeft.x; j <= bottomRight.x; j++)
			m_graph[i][j].type = GridType::GRID_OBSTACLE;
	}
}


/******************************************************************************
 * Graph::EntryObject -- Entry object.                                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   sub         --                                                    *
 *          topLeft     -- From top left to bottom right.                     *
 *          bottomRight -- From top left to bottom right.                     *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
void Graph::EntryObject(const Coordinate& sub)
{
	m_graph[sub.y][sub.x].type = GridType::GRID_OBJECT;
	m_objectList.emplace_back(sub);
}

void Graph::EntryObject(const Coordinate& topLeft, const Coordinate& bottomRight)
{
	for (int i = topLeft.y; i <= bottomRight.y; i++)
	{
		for (int j = topLeft.x; j <= bottomRight.x; j++)
		{
			m_graph[i][j].type = GridType::GRID_OBJECT;
			m_objectList.emplace_back(j, i);
		}
	}
}


/******************************************************************************
 * Graph::DeEntryObstacle -- De-entry an constant obstacle.                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   sub                                                               *
 * 	        topLeft                                                           *
 * 	        bottomRight                                                       *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
void Graph::DeEntryObstacle(const Coordinate& sub)
{
	if (m_graph[sub.y][sub.x].type == GridType::GRID_OBSTACLE)
		m_graph[sub.y][sub.x].type = GridType::GRID_PLAIN;
}

void Graph::DeEntryObstacle(const Coordinate& topLeft, const Coordinate& bottomRight)
{
	for (int i = topLeft.y; i <= bottomRight.y; i++)
	{
		for (int j = topLeft.x; j <= bottomRight.x; j++)
		{
			if (m_graph[i][j].type == GridType::GRID_OBSTACLE)
				m_graph[i][j].type = GridType::GRID_PLAIN;
		}
	}
}


/******************************************************************************
 * Graph::DeEntryObject -- De-entry an object.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   sub                                                               *
 * 	        topLeft                                                           *
 * 	        bottomRight                                                       *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  DEPRECATED!!!                                                   *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
void Graph::DeEntryObject(const Coordinate& sub)
{
	if (m_graph[sub.y][sub.x].type == GridType::GRID_OBJECT)
		m_graph[sub.y][sub.x].type = GridType::GRID_PLAIN;
}

void Graph::DeEntryObject(const Coordinate& topLeft, const Coordinate& bottomRight)
{
	for (int i = topLeft.y; i <= bottomRight.y; i++)
	{
		for (int j = topLeft.x; j <= bottomRight.x; j++)
		{
			if (m_graph[i][j].type == GridType::GRID_OBJECT)
				m_graph[i][j].type = GridType::GRID_PLAIN;
		}
	}
}


/******************************************************************************
 * Graph::Wander -- Wander aimlessly.                                         *
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
 *   2022/07/30 Tony : Created.                                               *
 *============================================================================*/
bool Graph::Wander(const Coordinate& src)
{
	std::vector<Coordinate> candidates;
	Coordinate next;

	for (int i = 0; i < DIR_NUM; i++)
	{
		next = src + DIR[i];
		if (m_graph[next.y][next.x].type == GridType::GRID_PLAIN)
			candidates.push_back(next);
	}

	if (candidates.size() == 0)
		return false;
	else
	{
		m_next = Random(candidates);
		return true;
	}
}


/******************************************************************************
 * Graph::Engage -- Find the shortest path.                                   *
 *                                                                            *
 *    This one find the shortest path to the target and then keep the next    *
 *    step to go.                                                             *
 *                                                                            *
 * REFERENCE:                                                                 *
 *          https://www.redblobgames.com/pathfinding/a-star/introduction.html *
 *                                                                            *
 * INPUT:   src  -- Source of the finding.                                    *
 *          dest -- Destination.                                              *
 *                                                                            *
 * OUTPUT:  Return whether a path exists.                                     *
 *                                                                            *
 * WARNINGS:  Remember to clear the path after finding.                       *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
bool Graph::Engage(const Coordinate& src, const Coordinate& dest)
{
	if (src == dest)
		return false;

	std::priority_queue<AStarToken, std::vector<AStarToken>, AStarTokenGreater> frontier;
	AStarToken current;
	Coordinate sub;
	int new_cost;
	bool hasPath = false;

	/*
	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0; j < m_width; j++)
			printf("%d", (int)(m_graph[i][j].type));
		putchar('\n');
	}
	*/

	m_graph[src.y][src.x].cost = 0;
	m_graph[src.y][src.x].father = src;

	frontier.push(AStarToken(src, 0));

	while (!frontier.empty())
	{
		current = frontier.top();
		frontier.pop();

		if (current.sub == dest)
		{
			hasPath = true;
			break;
		}

		for (int d = 0; d < DIR_NUM; d++)
		{
			sub = current.sub + DIR[d];

			if (!_IsLegal(sub))
				continue;

			// if ((sub != dest) && (m_graph[sub.y][sub.x].type != GridType::GRID_PLAIN))
				// continue;
			if ((sub != dest) && (m_graph[sub.y][sub.x].type != GridType::GRID_PLAIN))
				continue;

			if (d > 3)
			{
				int d1 = d & 1;
				int d2 = d >> 1;
				Coordinate sub1 = current.sub + DIR[d1];
				Coordinate sub2 = current.sub + DIR[d2];

				if ((m_graph[sub1.y][sub1.x].type != GridType::GRID_PLAIN) && \
					(m_graph[sub2.y][sub2.x].type != GridType::GRID_PLAIN))
				{
					continue;
				}
			}

			new_cost = m_graph[current.sub.y][current.sub.x].cost + DIR_COST[d];

			if (new_cost < m_graph[sub.y][sub.x].cost)
			{
				m_graph[sub.y][sub.x].father = current.sub;
				m_graph[sub.y][sub.x].cost = new_cost;
				frontier.push(AStarToken(sub, new_cost + _Heuristic(sub, dest)));
			}
			else if (new_cost == m_graph[sub.y][sub.x].cost)
			{
				if (Random(2))
					m_graph[sub.y][sub.x].father = current.sub;
			}
		}
	}

	if (hasPath)
	{
		sub = dest;
		while (m_graph[sub.y][sub.x].father != src)
			sub = m_graph[sub.y][sub.x].father;
		m_next = sub;
	}
	
	_ClearPath();

	return hasPath;
}


/******************************************************************************
 * Graph::Retreat -- Find the path to retreat.                                *
 *                                                                            *
 *    This one only search one step and choose the "worst" one to go, which   *
 *    looks like a retreat.                                                   *
 *                                                                            *
 * REFERENCE:                                                                 *
 *          https://www.redblobgames.com/pathfinding/a-star/introduction.html *
 *                                                                            *
 * INPUT:   src  -- Source of the finding.                                    *
 *          dest -- Destination.                                              *
 *                                                                            *
 * OUTPUT:  Return whether a choice exists.                                   *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/06 Tony : Created.                                               *
 *   2022/08/25 Tony : Bug fixed.                                             *
 *============================================================================*/
bool Graph::Retreat(const Coordinate& src, const Coordinate& dest)
{
/*
**	+------> x
**	|
**	V
**  y
*/
	if (src == dest)
		return false;

	std::priority_queue<AStarToken, std::vector<AStarToken>, AStarTokenLess> frontier;
	AStarToken current(src, 0);
	Coordinate sub;
	int new_cost;
	
	m_graph[src.y][src.x].cost = 0;

	for (int d = 0; d < DIR_NUM; d++)
	{
		sub = current.sub + DIR[d];

		/*
		** 2022/08/25 TS:
		**   Come on... sub may be illegal. :(
		*/
		if (!_IsLegal(sub))
			continue;
		if (m_graph[sub.y][sub.x].type != GridType::GRID_PLAIN)
			continue;

		new_cost = m_graph[current.sub.y][current.sub.x].cost + DIR_COST[d];

		if (new_cost < m_graph[sub.y][sub.x].cost)
			frontier.push(AStarToken(sub, new_cost + _Heuristic(sub, dest)));
	}

	// Here, only one grid changes.
	m_graph[src.y][src.x].Reset();

	if (frontier.empty())
		return false;

	m_next = frontier.top().sub;

	return true;
}


/******************************************************************************
 * Graph::FindBlank -- Find a blank position in the graph.                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   width  -- The width needed.                                      *
 *          height -- The height needed.                                     *
 *                                                                            *
 * OUTPUT:  Return whether is blank or not.                                   *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
bool Graph::FindBlank(int width, int height)
{
	int max_x = m_width - 1 - width;
	int max_y = m_height - 1 - height;

	if ((max_x < 1) || (max_y < 1))
		return false;

	return FindBlank({ Random(1, max_x), Random(1, max_y) }, width, height);
}

bool Graph::FindBlank(const Coordinate& center, int width, int height)
{
	std::queue<Coordinate> candidates;
	std::map<Coordinate, int, CompareCoordinate> visited;
	Coordinate current;
	int max_x = m_width - 1 - width;
	int max_y = m_height - 1 - height;
	int nx, ny;
	bool flag = false;

	if ((max_x < 1) || (max_y < 1))
		return false;

	candidates.push(center);
	while (!candidates.empty())
	{
		current = candidates.front();
		candidates.pop();
		if (visited.find(current) != visited.end())
			continue;
		visited[current] = 1;

		flag = true;
		for (int i = height - 1; (i >= 0) && flag; i--)
		{
			ny = current.y + i;
			for (int j = width - 1; (j >= 0) && flag; j--)
			{
				nx = current.x + j;

				if (m_graph[ny][nx].type != GridType::GRID_PLAIN)
					flag = false;
				else
				{
					if ((nx < max_x) && (ny < max_y))
						candidates.push({ nx, ny });
				}
			}
		}
		if (flag)
		{
			m_blank = current;
			break;
		}

		if (current.x - width > 1)
			candidates.push({ current.x - width , current.y });
		if (current.y - height > 1)
			candidates.push({ current.x, current.y - height });
		if (current.x + width < max_x)
			candidates.push({ current.x + width, current.y });
		if (current.y + height < max_y)
			candidates.push({ current.x, current.y + height });
	}

	return flag;
}

/******************************************************************************
 * Graph::inSight -- Check if dest is in sight of src.                        *
 *                                                                            *
 *    This function works by drawing a line between two points and see if any *
 *    block blocks it.                                                        *
 *                                                                            *
 * REFERENCE:  https://www.redblobgames.com/grids/line-drawing.html           *
 *                                                                            *
 * INPUT:   width  -- The width needed.                                      *
 *          height -- The height needed.                                     *
 *                                                                            *
 * OUTPUT:  Return whether is blank or not.                                   *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
bool Graph::InSight(const Coordinate& src, const Coordinate& dest)
{
	Coordinate diff = dest - src;
	Coordinate p = src;
	int dx = sign(diff.x);
	int dy = sign(diff.y);
	int decision;

	diff.x = abs(diff.x);
	diff.y = abs(diff.y);

	for (int ix = 0, iy = 0; (ix < diff.x) || (iy < diff.y);)
	{
		decision = (1 + 2 * iy) * diff.x - (1 + 2 * ix) * diff.y;
		if (decision == 0)
		{
			// diagonal
			p.x += dx;
			p.y += dy;
			ix++;
			iy++;
		}
		else if (decision < 0)
		{
			// horizontal
			p.y += dy;
			iy++;
		}
		else
		{
			// vertical
			p.x += dx;
			ix++;
		}
		if (m_graph[p.y][p.x].type != GridType::GRID_PLAIN)
			return false;
	}

	return true;
}


/******************************************************************************
 * Graph::_IsLegal -- Check if a subscription is legal or not.                *
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
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
bool Graph::_IsLegal(const Coordinate& sub)
{
	return ((0 <= sub.x) && (sub.x < m_width) && (0 <= sub.y) && (sub.y < m_height));
}


/******************************************************************************
 * Graph::_Heuristic -- The heuristic function.                               *
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
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
int Graph::_Heuristic(const Coordinate& src, const Coordinate& dest)
{
	return ManhattanDist(src, dest);
}


/******************************************************************************
 * Graph::_ClearPath -- Clear previous path data.                             *
 *                                                                            *
 *    Previous path need to be cleared each time.                             *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/06 Tony : Created.                                               *
 *============================================================================*/
void Graph::_ClearPath()
{
	for (int i = m_height - 2; i > 0; i--)
	{
		for (int j = m_width - 2; j > 0; j--)
			m_graph[i][j].Reset();
	}
}