/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Brick.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 24, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For base class of brick in game.                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Brick.h"
#include "../../inc/object/GateBehavior.h"


/******************************************************************************
 * Brick::Clone -- Clone brick.                                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   clone                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void Brick::Clone(Brick* clone) const
{
	Object::Clone(clone);
}


/******************************************************************************
 * Brick::Load                                                                *
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
 *   2022/06/26 Tony : Created.                                               *
 *============================================================================*/
bool Brick::Load(XMLElement* node)
{
	Object::Load(node);
	
	_RETURN_STATE();
}


/******************************************************************************
 * Wall::Clone -- Clone a wall.                                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return the exact clone.                                           *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
Wall* Wall::Clone() const
{
	Wall* clone = new Wall(m_pScene);
	clone->_MakePrototype(false);

	Brick::Clone(clone);

	return clone;
}


/******************************************************************************
 * Wall::Load -- Load a wall from xml node.                                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return succeeded or not.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
bool Wall::Load(XMLElement* node)
{
/*
**	<Wall>
**		<Components>
**			<Anim></Anim>
**			<CollideBox></collidebox>
**			<Rigidbody></rigidbody>
**		</Components>
**	</Wall>
*/
	const char* name = node->Name();

	_CHECK_TAG("Wall");
	_RETURN_IF_ERROR();

	Brick::Load(node);

	_RETURN_STATE();
}


/******************************************************************************
 * Gate::Clone -- Clone gate.                                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return exact clone.                                               *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
Gate* Gate::Clone() const
{
	Gate* clone = new Gate(m_pScene);
	clone->_MakePrototype(false);

	Brick::Clone(clone);

	return clone;
}


/******************************************************************************
 * Gate::Load -- Load gate.                                                   *
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
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
bool Gate::Load(XMLElement* node)
{
	/*
	**	<Gate>
	**		<Anim></Anim>
	**		<CollideBox></Collidebox>
	**		<RigidBody></Rigidbody>
	**	</Gate>
	*/
	const char* name = node->Name();

	_CHECK_TAG("Gate");
	_RETURN_IF_ERROR();

	Brick::Load(node);

	_InitBehavior();

	_RETURN_STATE();
}


/******************************************************************************
 * Gate::Open -- Open the gate.                                               *
 *                                                                            *
 *    If gate is open, then nothing will happen. Or is opening.               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  No check!!!                                                     *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/26 Tony : Created.                                               *
 *============================================================================*/
void Gate::Open()
{
	if (!m_isOpen)
		GetComponent<BehaviorComponent>()->ChangeBehavior("Opening");
}


/******************************************************************************
 * Gate::Close -- Close the gate.                                             *
 *                                                                            *
 *    Hmm... the same as Open().                                              *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/26 Tony : Created.                                               *
 *============================================================================*/
void Gate::Close()
{
	if (!m_isClosed)
		GetComponent<BehaviorComponent>()->ChangeBehavior("Closing");
}


/******************************************************************************
 * Gate::_InitState -- Initialize state of gate.                              *
 *                                                                            *
 *    Register all states to the state component.                             *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/02 Tony : Created.                                               *
 *============================================================================*/
void Gate::_InitBehavior(XMLElement* node)
{
	auto parent = GetComponent<BehaviorComponent>();

	parent->AddBehavior(new GateOpen());
	parent->AddBehavior(new GateClosed());
	parent->AddBehavior(new GateOpening());
	parent->AddBehavior(new GateClosing());

	parent->ChangeBehavior("Open");
}