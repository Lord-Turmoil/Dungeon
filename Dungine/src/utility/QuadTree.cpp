/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : QuadTree.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 9, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
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

#include "../../inc/common/Common.h"

#include "../../inc/utility/Auxilliary.h"
#include "../../inc/utility/QuadTree.h"

#include "../../inc/game/StandardComponent.h"

/******************************************************************************
 * QuadTreeNode::QuadTreeNode -- Constructor of the object.                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   range                                                             *
 *          maxPointDesity                                                    *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
QuadTreeNode::QuadTreeNode(const Rect& range, int maxDensity)
    : m_pAlpha(nullptr), m_pBeta(nullptr), m_pGamma(nullptr), m_pDelta(nullptr), m_boxNum(0), m_maxDensity(maxDensity),
      m_isSplit(false)
{
    m_range.Reset(range);
    m_pObjects = new GameObject*[maxDensity];
}

/******************************************************************************
 * QuadTree::QuadTree -- Constructor of the object.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   range           -- The primary range of the tree.                 *
 *          maxDensity -- How many points can be in one node.              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
QuadTree::QuadTree() : m_pQuadTree(nullptr), m_maxBoxDensity(0)
{
}

QuadTree::QuadTree(const Rect& range, int maxDensity)
{
    m_pQuadTree = new QuadTreeNode(range, maxDensity);
    m_maxBoxDensity = maxDensity;
}

/******************************************************************************
 * QuadTree::~QuadTree -- Destructor of the object.                           *
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
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
QuadTree::~QuadTree()
{
    if (m_pQuadTree)
    {
        _Destroy(m_pQuadTree);
    }
}

/******************************************************************************
 * QuadTree::Initialize -- Initialize the quad tree.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   range           -- The primary range of the tree.                 *
 *          maxDensity -- How many points can be in one node.                 *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
void QuadTree::Initialize(const Rect& range, int maxDensity)
{
    if (m_pQuadTree)
    {
        _Destroy(m_pQuadTree);
    }

    m_pQuadTree = new QuadTreeNode(range, maxDensity);
    m_maxBoxDensity = maxDensity;
}

/******************************************************************************
 * QuadTree::Destroy -- Destroy the quad tree.                                *
 *                                                                            *
 *    This will destroy everything, even the initial node.                    *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
void QuadTree::Destroy()
{
    if (m_pQuadTree)
    {
        _Destroy(m_pQuadTree);
        m_pQuadTree = nullptr;
    }
}

/******************************************************************************
 * QuadTree::Insert -- Insert a collide obj to the quad tree.                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   obj -- The collide obj.                                           *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *   2022/10/22 Tony : Added premise check.                                   *
 *============================================================================*/
void QuadTree::Insert(GameObject* obj)
{
    auto rigid = obj->GetComponent<RigidBodyComponent>();
    auto box = obj->GetComponent<ColliderBoxComponent>();

    if (!rigid || !box)
    {
        return;
    }

    _Insert(m_pQuadTree, obj);
}

/******************************************************************************
 * QuadTree::Query -- Query neighboring collide boxes.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   range   -- The range to query.                                    *
 *          obj     -- Query the obj's neighbors.                             *
 *          results -- The vector to store results.                           *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
void QuadTree::Query(const Rect& range, std::vector<GameObject*>& results)
{
    _Query(m_pQuadTree, range, results);
}

void QuadTree::Query(GameObject* obj, std::vector<GameObject*>& results)
{
    _Query(m_pQuadTree, obj->GetComponent<ColliderBoxComponent>()->GetBorder(), results);
}

/******************************************************************************
 * QuadTree::Query -- Query neighboring collide boxes.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   range -- The range to query.                                      *
 *          obj   -- Query the obj's neighbors.                               *
 *                                                                            *
 * OUTPUT:  Return the result vector.                                         *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
std::vector<GameObject*> QuadTree::Query(const Rect& range)
{
    std::vector<GameObject*> results;

    _Query(m_pQuadTree, range, results);

    return results;
}

std::vector<GameObject*> QuadTree::Query(GameObject* obj)
{
    std::vector<GameObject*> results;

    _Query(m_pQuadTree, obj->GetComponent<ColliderBoxComponent>()->GetBorder(), results);

    return results;
}

/******************************************************************************
 * QuadTree::Clear -- Clear the quad tree.                                    *
 *                                                                            *
 *    This just empty the quad tree, the primary node will be remained.       *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
void QuadTree::Clear()
{
    for (int i = 0; i < 4; i++)
    {
        if (m_pQuadTree->m_pSubNode[i])
        {
            _Destroy(m_pQuadTree->m_pSubNode[i]);
            m_pQuadTree->m_pSubNode[i] = nullptr;
        }
    }

    // If divided, the points will be freed.
    if (!m_pQuadTree->m_pObjects)
    {
        m_pQuadTree->m_pObjects = new GameObject*[m_maxBoxDensity];
    }
    m_pQuadTree->m_isSplit = false;
    m_pQuadTree->m_boxNum = 0;
}

/******************************************************************************
 * QuadTree::_Split -- Split a node.                                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
bool QuadTree::_Split(QuadTreeNode* node)
{
    Rect rect = node->m_range;
    int halfWidth = rect.width >> 1;
    int halfHeight = rect.height >> 1;

    // Too small...
    if ((halfWidth == 0) || (halfHeight == 0))
    {
        // LOG_MESSAGE("QuadTree::_Split() Failed");
        return false;
    }

    Rect range;
    int maxDensity;

    if ((halfWidth < 3) || (halfHeight < 3))
    {
        maxDensity = MAX_DENSITY;
    }
    else
    {
        maxDensity = m_maxBoxDensity;
    }

    range.Reset(rect.pos.x, rect.pos.y, halfWidth, halfHeight);
    node->m_pDelta = new QuadTreeNode(range, maxDensity);
    range.Reset(rect.pos.x, rect.pos.y + halfHeight, halfWidth, rect.height - halfHeight);
    node->m_pBeta = new QuadTreeNode(range, maxDensity);
    range.Reset(rect.pos.x + halfWidth, rect.pos.y, rect.width - halfWidth, halfHeight);
    node->m_pGamma = new QuadTreeNode(range, maxDensity);
    range.Reset(rect.pos.x + halfWidth, rect.pos.y + halfHeight, rect.width - halfWidth, rect.height - halfHeight);
    node->m_pAlpha = new QuadTreeNode(range, maxDensity);

    node->m_isSplit = true;

    return true;
}

/******************************************************************************
 * QuadTree::_Insert -- Insert a collide obj to the tree.                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node -- The node to add the obj.                                  *
 *          obj  -- The collide obj waiting to be added.                      *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
bool QuadTree::_Insert(QuadTreeNode* node, GameObject* obj)
{
    Coordinate pos = obj->GetCenter();

    if (!InRectangle(node->m_range, pos))
    {
        return false;
    }

    if (node->m_boxNum < node->m_maxDensity)
    {
        node->m_pObjects[node->m_boxNum++] = obj;
        return true;
    }
    else if (node->m_isSplit)
    {
        for (int i = 0; i < 4; i++)
        {
            if (_Insert(node->m_pSubNode[i], obj))
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        if (!_Split(node))
        {
            return false;
        }

        // Pass current boxes to sub nodes.
        for (int i = 0; i < node->m_boxNum; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (_Insert(node->m_pSubNode[j], node->m_pObjects[i]))
                {
                    break;
                }
            }
        }

        delete[] node->m_pObjects;
        node->m_pObjects = nullptr;

        // Insert new node.
        for (int i = 0; i < 4; i++)
        {
            if (_Insert(node->m_pSubNode[i], obj))
            {
                return true;
            }
        }
        return false;
    }

    return false; // Never reach here.
}

/******************************************************************************
 * QuadTree::_Query -- Query neighboring collide boxes.                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *          range                                                             *
 *          results                                                           *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *   2022/12/09 Tony : Critical bug found! Too large object may not be found  *
 *                     by the other object.                                   *
 *============================================================================*/
void QuadTree::_Query(QuadTreeNode* node, const Rect& range, std::vector<GameObject*>& results)
{
    if (!HasIntersect(node->m_range, range))
    {
        return;
    }

    if (node->m_isSplit)
    {
        for (int i = 0; i < 4; i++)
        {
            _Query(node->m_pSubNode[i], range, results);
        }
    }
    else
    {
        for (int i = 0; i < node->m_boxNum; i++)
        {
            if (InRectangle(range, node->m_pObjects[i]->GetCenter()))
            {
                results.push_back(node->m_pObjects[i]);
            }
        }
    }
}

/******************************************************************************
 * QuadTree::_Destroy -- Destroy a quad tree node and all its children.       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
void QuadTree::_Destroy(QuadTreeNode* node)
{
    if (!node)
    {
        return;
    }

    if (node->m_isSplit)
    {
        for (int i = 0; i < 4; i++)
        {
            _Destroy(node->m_pSubNode[i]);
        }
    }
    else
    {
        delete[] node->m_pObjects;
        node->m_pObjects = nullptr;
    }

    delete node;
}
