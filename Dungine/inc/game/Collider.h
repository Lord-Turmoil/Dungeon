/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Collider.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 20, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide some methods to handle collision.                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _COLLIDER_H_
#define _COLLIDER_H_

class GameObject;

enum CollisionType
{
	COLL_NONE,
	COLL_CHECK,
	COLL_ELASTIC,
	COLL_INELASTIC,

	COLL_NUM
};


class Collider
{
public:
	// A is movable and B is un-movable.
	static bool Collide(GameObject* objA, GameObject* objB);

private:
	static bool _Check(GameObject* objA, GameObject* objB);
	static bool _Elastic(GameObject* objA, GameObject* objB);
	static bool _InElastic(GameObject* objA, GameObject* objB);
};


/********************************************************************
** Collision relation is uncertain, it should be realised by client.
*/
CollisionType GetCollisionType(int idA, int idB);


#endif