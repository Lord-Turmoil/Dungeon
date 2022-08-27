/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GateComponent.cpp                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 2, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For gate component.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/object/Brick.h"
#include "../../inc/object/GateBehavior.h"



/******************************************************************************
 * Hmm... Just the name.                                                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   See below...                                                      *
 *                                                                            *
 * OUTPUT:  See below...                                                      *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/02 Tony : Created.                                               *
 *============================================================================*/

void GateBehavior::Clone(GateBehavior* clone) const
{
	Behavior::Clone(clone);
}


// GateOpen
GateOpen* GateOpen::Clone() const
{
	GateOpen* clone = new GateOpen();
	clone->_MakePrototype(false);
	
	GateBehavior::Clone(clone);

	return clone;
}

void GateOpen::Update(Event* evnt)
{
}

void GateOpen::OnEnter()
{
	Gate* gate = static_cast<Gate*>(m_parent->GetGameObject());
	auto anim = gate->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(GATE_ANIM_OPEN);
	anim->SetFrame(Random(anim->GetTotalFrameNum()));
	gate->GetComponent<RigidBodyComponent>()->SetID(COLL_ID_WALL_OPEN);
	gate->GetSymbol()->SetLayer(LAYER_BRICK - 1);
	
	gate->SetOpen(true);
}


// GateClosed
GateClosed* GateClosed::Clone() const
{
	GateClosed* clone = new GateClosed();
	clone->_MakePrototype(false);

	GateBehavior::Clone(clone);

	return clone;
}

void GateClosed::Update(Event* evnt)
{
}

void GateClosed::OnEnter()
{
	Gate* gate = static_cast<Gate*>(m_parent->GetGameObject());
	auto anim = gate->GetComponent<AnimComponent>()->GetAnim();

	anim->SetMotion(GATE_ANIM_CLOSED);
	anim->SetFrame(Random(anim->GetTotalFrameNum()));
	gate->GetComponent<RigidBodyComponent>()->SetID(COLL_ID_WALL);
	gate->GetSymbol()->SetLayer(LAYER_BRICK);

	gate->SetClosed(true);
}


// GateOpening
GateOpening* GateOpening::Clone() const
{
	GateOpening* clone = new GateOpening();
	clone->_MakePrototype(false);

	GateBehavior::Clone(clone);

	return clone;
}

void GateOpening::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Open");
	}
}

void GateOpening::OnEnter()
{
	Gate* gate = static_cast<Gate*>(m_parent->GetGameObject());

	gate->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(GATE_ANIM_OPENING);
	gate->GetSymbol()->SetLayer(LAYER_BRICK);

	gate->SetClosed(false);
}


// GateClosing
GateClosing* GateClosing::Clone() const
{
	GateClosing* clone = new GateClosing();
	clone->_MakePrototype(false);

	GateBehavior::Clone(clone);

	return clone;
}

void GateClosing::Update(Event* evnt)
{
	if (m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim()->IsOver())
	{
		m_parent->ChangeBehavior("Closed");
	}
}

void GateClosing::OnEnter()
{
	Gate* gate = static_cast<Gate*>(m_parent->GetGameObject());

	gate->GetComponent<AnimComponent>()
		->GetAnim()->SetMotion(GATE_ANIM_CLOSING);
	gate->GetComponent<RigidBodyComponent>()->SetID(COLL_ID_WALL);
	gate->GetSymbol()->SetLayer(LAYER_BRICK);

	gate->SetOpen(false);
}