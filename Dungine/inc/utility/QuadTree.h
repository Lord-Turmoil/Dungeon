/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : QuadTree.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 9, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide extra function of quadruple tree for collision check.            *
 * -------------------------------------------------------------------------- *
 * Reference:                                                                 *
 *   https://codebus.cn/sunxiaoyu/quadtree-collision                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _QUADTREE_H_
#define _QUADTREE_H_

/********************************************************************
** Quadruple tree needs collidebox to perform loose collision check.
*/

#include <vector>

#include "../common/Common.h"
#include "../game/GameObject.h"

const int DEFAULT_DENSITY = 4;

/*
** For the final node which can't split. If this number is
** exceeded, then it won't insert new ones.
*/
const int MAX_DENSITY = 64;

class QuadTreeNode
{
	friend class QuadTree;
public:
	QuadTreeNode(const Rect& range, int maxDensity);
	
private:
	union
	{
		/*
		**          |
		** m_pDelta | m_pGamma
		** --------------------->
		** m_pBeta  | m_pAlpha
		**          | 
		**          V
		*/
		struct
		{
			QuadTreeNode* m_pAlpha;
			QuadTreeNode* m_pBeta;
			QuadTreeNode* m_pGamma;
			QuadTreeNode* m_pDelta;
		};
		QuadTreeNode* m_pSubNode[4];
	};

	Rect m_range;				// border of the node
	GameObject** m_pObjects;	// boxes in current rect
	int m_boxNum;				// number of points in range
	int m_maxDensity;			// max point number in range
	bool m_isSplit;				// whether split or not
};


class QuadTree
{
public:
	QuadTree();
	QuadTree(const Rect& range, int maxDensity = DEFAULT_DENSITY);
	~QuadTree();

	void Initialize(const Rect& range, int maxDensity = DEFAULT_DENSITY);
	void Destroy();

	void Insert(GameObject* obj);
	void Query(const Rect& range, std::vector<GameObject*>& results);
	void Query(GameObject* obj, std::vector<GameObject*>& results);
	std::vector<GameObject*> Query(const Rect& range);			// RVO?
	std::vector<GameObject*> Query(GameObject* obj);	// RVO?
	
	void Clear();

private:
	bool _Split(QuadTreeNode* node);
	bool _Insert(QuadTreeNode* node, GameObject* obj);
	void _Query(QuadTreeNode* node, const Rect& range, std::vector<GameObject*>& results);
	void _Destroy(QuadTreeNode* node);

private:
	QuadTreeNode* m_pQuadTree;
	int m_maxBoxDensity;
};

#endif