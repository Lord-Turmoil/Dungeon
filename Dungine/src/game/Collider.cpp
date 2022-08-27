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

#include "../../inc/common/Common.h"

#include "../../inc/game/Collider.h"
#include "../../inc/game/GameObject.h"
#include "../../inc/game/StandardComponent.h"

#include "../../inc/utility/Auxilliary.h"


/********************************************************************
** Well, for all collision checks, objA is regarded as movable, while
** objB is regarded as non-movable.
*/

/******************************************************************************
 * Collider::Collide -- Apply collision.                                      *
 *                                                                            *
 *    This function will apply correct collision based on the type.           *
 *                                                                            *
 * INPUT:   objA                                                              *
 *          objB                                                              *
 *                                                                            *
 * OUTPUT:  Whether collision happens or not.                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/20 Tony : Created.                                               *
 *============================================================================*/
bool Collider::Collide(GameObject* objA, GameObject* objB)
{
	RigidBodyComponent* rigidA = objA->GetComponent<RigidBodyComponent>();
	RigidBodyComponent* rigidB = objB->GetComponent<RigidBodyComponent>();

	if (!rigidA || !rigidB)
		return false;

	CollisionType type = GetCollisionType(rigidA->ID(), rigidB->ID());
	
	switch (type)
	{
	case CollisionType::COLL_NONE:
		return false;
	case CollisionType::COLL_CHECK:
		return _Check(objA, objB);
	case CollisionType::COLL_INELASTIC:
		return _InElastic(objA, objB);
	case CollisionType::COLL_ELASTIC:
		return _Elastic(objA, objB);
	default:
		return false;
	};
}


/******************************************************************************
 * Collider::_Check -- Only do check.                                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   objA                                                              *
 *          objB                                                              *
 *                                                                            *
 * OUTPUT:  Whether collision happens or not.                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/20 Tony : Created.                                               *
 *============================================================================*/
bool Collider::_Check(GameObject* objA, GameObject* objB)
{
	ColliderBoxComponent* boxA = objA->GetComponent<ColliderBoxComponent>();
	ColliderBoxComponent* boxB = objB->GetComponent<ColliderBoxComponent>();

	if (!boxA || !boxB)
		return false;

	return HasIntersect(boxA->GetBox(), boxB->GetBox());
}


/******************************************************************************
 * Collider::Elastic -- Perform elastic collision.                            *
 *                                                                            *
 *    For now, the force of object is neglected.                              *
 *                                                                            *
 * INPUT:   objA                                                              *
 *          objB                                                              *
 *                                                                            *
 * OUTPUT:  Whether collision happens or not.                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/20 Tony : Created.                                               *
 *============================================================================*/
bool Collider::_Elastic(GameObject* objA, GameObject* objB)
{
	ColliderBoxComponent* boxCmptA = objA->GetComponent<ColliderBoxComponent>();
	ColliderBoxComponent* boxCmptB = objB->GetComponent<ColliderBoxComponent>();
	RigidBodyComponent* rigidCmptA = objA->GetComponent<RigidBodyComponent>();
	RigidBodyComponent* rigidCmptB = objB->GetComponent<RigidBodyComponent>();

	if (!boxCmptA || !boxCmptB || !rigidCmptA || !rigidCmptB)
		return false;

	const Rect& boxA = boxCmptA->GetBox();
	const Rect& boxB = boxCmptB->GetBox();
	Coordinate overlap = Intersect(boxA, boxB);

	if (!overlap.Positive())
		return false;

	Coordinate centerA = boxA.GetCenter();
	Coordinate centerB = boxB.GetCenter();

	if (overlap.x < overlap.y)
	{
		if (centerA.x < centerB.x)
			overlap.x = -overlap.x;
		overlap.y = 0;
	}
	else
	{
		if (centerA.y < centerB.y)
			overlap.y = -overlap.y;
		overlap.x = 0;
	}

	objA->Translate(overlap);

	/*
	** A is the active object, its mass mustn't be zero.
	*/
	const double e = 0.8;
	if (rigidCmptB->GetMass() != 0.0)
	{
		const double m1 = rigidCmptA->GetMass();
		const double m2 = rigidCmptB->GetMass();
		double v11, v12, v21, v22;

		if (overlap.x != 0)
		{
			v11 = rigidCmptA->GetVelocity().x;
			v21 = rigidCmptB->GetVelocity().x;
		}
		else
		{
			v11 = rigidCmptA->GetVelocity().y;
			v21 = rigidCmptB->GetVelocity().y;
		}
		v12 = (m1 - e * m2) * v11 + (1.0 + e) * m2 * v21;
		v22 = (m2 - e * m1) * v21 + (1.0 + e) * m1 * v11;
		v12 /= (m1 + m2);
		v22 /= (m1 + m2);
		if (overlap.x != 0)
		{
			rigidCmptA->GetVelocity().x = v12;
			rigidCmptB->GetVelocity().x = v22;
		}
		else
		{
			rigidCmptA->GetVelocity().y = v12;
			rigidCmptB->GetVelocity().y = v22;
		}
	}
	else
	{
		// if m_b is zero, then v_b is zero, too.
		if (overlap.x != 0)
			rigidCmptA->GetVelocity().x = -1.0 * e * rigidCmptA->GetVelocity().x;
		else
			rigidCmptA->GetVelocity().y = -1.0 * e * rigidCmptA->GetVelocity().y;
	}

	return true;
}


/******************************************************************************
 * Collider::InElastic -- Perform inelastic collision.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   objA                                                              *
 *          objB                                                              *
 *                                                                            *
 * OUTPUT:  Whether collision happens or not.                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/20 Tony : Created.                                               *
 *============================================================================*/
bool Collider::_InElastic(GameObject* objA, GameObject* objB)
{
	ColliderBoxComponent* boxCmptA = objA->GetComponent<ColliderBoxComponent>();
	ColliderBoxComponent* boxCmptB = objB->GetComponent<ColliderBoxComponent>();
	RigidBodyComponent* rigidCmptA = objA->GetComponent<RigidBodyComponent>();
	RigidBodyComponent* rigidCmptB = objB->GetComponent<RigidBodyComponent>();

	if (!boxCmptA || !boxCmptB || !rigidCmptA || !rigidCmptB)
		return false;

	Rect& boxA = boxCmptA->GetBox();
	Rect& boxB = boxCmptB->GetBox();
	Coordinate overlap = Intersect(boxA, boxB);

	if (!overlap.Positive())
		return false;

	Coordinate centerA = boxA.GetCenter();
	Coordinate centerB = boxB.GetCenter();

	if (overlap.x < overlap.y)
	{
		if (centerA.x < centerB.x)	// A hit B on the left
			overlap.x = -overlap.x;
		overlap.y = 0;
		rigidCmptA->GetVelocity().x = 0.0;
		rigidCmptA->GetForce().x = 0.0;
	}
	else
	{
		if (centerA.y < centerB.y)
			overlap.y = -overlap.y;
		overlap.x = 0;
		rigidCmptA->GetVelocity().y = 0.0;
		rigidCmptA->GetForce().y = 0.0;
	}
	
	boxA.Translate(overlap);
	objA->Translate(overlap);

	return true;
}