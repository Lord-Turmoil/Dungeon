/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Space.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 7, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The space of the terrain is where the event is currently taking place.   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _SPACE_H_
#define _SPACE_H_

#include <vector>

#include <dungine.h>

#include "../common/Defines.h"
#include "../object/Brick.h"

#include "Graph.h"



/********************************************************************
** Plain terrain is the basic info of the terrain. The final terrain
** will be generated based on it.
*/
struct PlainSpace
{
	unsigned gate;

	PlainSpace(unsigned _gate = DIR_NONE) : gate(_gate) {}
};

struct PlainArena : public PlainSpace
{
	int id;		// the id in 3 * 3 grids
	bool isStart;
	bool isEnd;

	PlainArena(int _id, unsigned _gate = DIR_NONE) : PlainSpace(_gate),
		id(_id), isStart(false), isEnd(false) {}
};

struct PlainBridge : public PlainSpace
{
	// the two arenas connected by it, u < v
	int u, v;

	PlainBridge(int _u, int _v);

	bool IsHorizontal();
	bool IsVertical();
};

/********************************************************************
** Space type and attribute.
*/
enum class SpaceType
{
	SPC_ARENA,
	SPC_BRIDGE
};

struct SpaceAttribute
{
	int rounds;

	bool isDiscovered : 1;
	bool isCompleted : 1;
	bool isCurrent : 1;
	bool isStart : 1;
	bool isEnd : 1;
	bool hasBoss : 1;
	bool showCreate : 1;

	SpaceAttribute()
		: rounds(0), isDiscovered(false), isCompleted(false), isCurrent(false), 
		isStart(false), isEnd(false), hasBoss(false), showCreate(false) {}

	void Init()
	{
		rounds = 0;
		isDiscovered = isCompleted = false;
		isCurrent = false;
		isStart = isEnd = false;
		hasBoss = false;
		showCreate = false;
	}

	void Complete()
	{
		isCompleted = true;
		showCreate = true;
	}
};


class Terrain;

typedef std::vector<BrickType> BrickRow;
typedef std::vector<BrickRow> BrickMatrix;

class Space
{
public:
	Space(SpaceType type, Terrain* terrain);
	virtual ~Space();

	SpaceType Type() const { return m_type; }
	SpaceAttribute& Attribute() { return m_attr; }

	bool InRange(const Coordinate& target);

	virtual void Generate(PlainSpace* spc) = 0;
	virtual void Update() = 0;
	virtual void Draw();
	virtual void Draw(Camera* camera);
	void DrawMiniMap();

	virtual void Discover();

public:
	void SetCoord(const Coordinate& coord) { m_rect.pos = coord; }
	void SetSize(int width, int height) { m_rect.Resize(width, height); }
	void SetRect(const Rect& rect) { m_rect.Reset(rect); }

	Coordinate GetCoord() const { return m_rect.GetCenter(); }
	Rect GetBorder() const { return m_rect; }
	ObjectPool& GetBrickPool() { return m_bricks; }

protected:
	Coordinate _CoordToSub(const Coordinate& coord);
	Coordinate _SubToCoordCenter(const Coordinate& sub);
	Coordinate _SubToCoord(const Coordinate& sub);

	void _Render(BrickMatrix& matrix);

protected:
	SpaceType m_type;
	int m_id;

	Rect m_rect;
	SpaceAttribute m_attr;
	
	ObjectPool m_bricks;

	Symbol m_floor;

	Terrain* m_pTerrain;
};


/********************************************************************
** Arena is where the fight takes place.
*/
class Arena : public Space
{
public:
	Arena(Terrain* terrain);
	virtual ~Arena();

	int ID() const { return m_id; }

	virtual void Generate(PlainSpace* spc);
	virtual void Update();

	virtual void Discover();

public:
	void Entry(const Coordinate& coord);
	void Entry(const Coordinate& topLeft, const Coordinate& bottomRight);

	void DeEntry(const Coordinate& coord);
	void DeEntry(const Coordinate& topLeft, const Coordinate& bottomRight);

	Coordinate Wander(const Coordinate& src);
	Coordinate Engage(const Coordinate& src, const Coordinate& dest);
	Coordinate Retreat(const Coordinate& src, const Coordinate& dest);

	Coordinate FindBlank(int width, int height);
	Coordinate FindBlank(const Coordinate& center, int width, int height);
	bool InSight(const Coordinate& src, const Coordinate& dest);

	void Clear();

public:
	void AddNeighbor(Space* spc);
	bool IsActivated() const { return m_isActivated; }

public:
	int GetGraphWidth() const	{ return m_graph.Width(); }
	int GetGraphHeight() const	{ return m_graph.Height(); }

private:
	void _Lock();
	void _UnLock();

	bool _InTrap(const Coordinate& target);

	void _GenerateObstacle(BrickMatrix& matrix);

	void _PatternLine(BrickMatrix& matrix);
	void _PatternCorner(BrickMatrix& matrix);
	void _PatternRandom(BrickMatrix& matrix);

	std::vector<Gate*> m_gates;
	std::vector<Space*> m_neighbors;

	Graph m_graph;

	/*
	** If the player goes into the arena, it will be activated,
	** and will then generate enemies.
	*/
	bool m_isActivated;

	bool m_isLocked;
};


/********************************************************************
** Bridge only connects arenas.
*/
class Bridge : public Space
{
public:
	Bridge(Terrain* terrain);
	virtual ~Bridge();

	virtual void Generate(PlainSpace* spc);
	virtual void Update();
	
private:

};

#endif