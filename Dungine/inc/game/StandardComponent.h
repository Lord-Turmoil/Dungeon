/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : StandardComponent.h                       *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Some standard components.                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _STANDARD_COMPONENT_H_
#define _STANDARD_COMPONENT_H_

#include "AbstractComponent.h"

/********************************************************************
** RigidBody is the physical property of the object.
*/
class RigidBodyComponent : public AbstractComponent
{
public:
	RigidBodyComponent(int updateOrder = 0) : AbstractComponent(updateOrder),
		m_id(0), m_mass(0.0), m_ratio(0.0), m_maxVelocity(0.0), m_strength(0.0) {}
	virtual ~RigidBodyComponent() {}

	static const char* StaticName() { return "RigidBody"; }
	virtual const char* Name() { return StaticName(); }

	virtual RigidBodyComponent* Clone() const;
	virtual void Clone(RigidBodyComponent* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt);

public:
	void Freeze();
	void Scatter();
	void Scatter(double speed);

public:
	void SetID(int id) { m_id = id; }
	int ID() const { return m_id; }

	void SetMass(double mass) { m_mass = mass; }
	double GetMass() const { return m_mass; }

	void SetRatio(double ratio) { m_ratio = ratio; }
	double GetRatio() const { return m_ratio; }

	void SetMaxVelocity(double maxVel) { m_maxVelocity = maxVel; }
	double GetMaxVelocity() const { return m_maxVelocity; }

	void SetStrength(double strength) { m_strength = strength; }
	double GetStrength() const { return m_strength; }

	void SetForce(const Vector& force) { m_force = force; }
	void ApplyForce(const Vector& force) { m_force += force; }
	Vector& GetForce() { return m_force; }

	void SetVelocity(const Vector& velocity) { m_velocity = velocity; }
	void ApplyVelocity(const Vector& velocity) { m_velocity += velocity; }
	Vector& GetVelocity() { return m_velocity; }

private:
	/*
	** Collision ID, to handle collision type.
	*/
	int m_id;

	/*
	** 0.0 is infinite. If mass is infinite, then it cannot move,
	** which means its velocity must be absolute zero.
	*/
	double m_mass;

	// Decrease ratio of velocity if no force applied.
	double m_ratio;

	double m_maxVelocity;

	// Strength of the object. Used when move.
	double m_strength;

	Vector m_force;
	Vector m_velocity;
};


/********************************************************************
** Collide box is... the collide box of the object :P. It contains
** the narrow collision size, which is a rectangle with an offset to
** the center of the game object. The detect range for loose check in
** quadruple tree is twice the size of the narrow one.
** Notice that, the pos of the box and border is corrected only after
** Update().
*/
class ColliderBoxComponent : public AbstractComponent
{
public:
	ColliderBoxComponent(int updateOrder = 0) : AbstractComponent(updateOrder) {}
	virtual ~ColliderBoxComponent() {}

	static const char* StaticName() { return "ColliderBox"; }
	virtual const char* Name() { return StaticName(); }

	virtual ColliderBoxComponent* Clone() const;
	void Clone(ColliderBoxComponent* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt);

public:
	virtual void Translate(const Coordinate& offset);

	void SetOffset(const Coordinate& offset) { m_offset = offset; }
	Coordinate GetOffset() const { return m_offset; }

	void SetCenter(const Coordinate& center) { m_center = center; }
	Coordinate GetCenter() const { return m_center; }

	void SetBoxSize(int width, int height)
	{
		m_box.width = width;
		m_box.height = height;
	}
	void SetBorderSize(int width, int height)
	{
		m_border.width = width;
		m_border.height = height;
	}
	int GetBoxWidth() const { return m_box.width; }
	int GetBoxHeight() const { return m_box.height; }
	int GetBorderWidth() const { return m_border.width; }
	int GetBorderHeight() const { return m_border.height; }

	Rect& GetBox() { return m_box; }
	Rect& GetBorder() { return m_border; }

private:
	Coordinate m_offset;	// offset to the center of the gameobject
	Coordinate m_center;
	Rect m_box;
	Rect m_border;
};


/********************************************************************
** Move component has to access rigid body for velocity.
*/
class MoveComponent : public AbstractComponent
{
public:
	MoveComponent(int updateOrder = 0) : AbstractComponent(updateOrder) {}
	virtual ~MoveComponent() {}

	static const char* StaticName() { return "Move"; }
	virtual const char* Name() { return StaticName(); }

	virtual MoveComponent* Clone() const;
	virtual void Clone(MoveComponent* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt);

protected:

};


/********************************************************************
** Game object need anim component to display on the screen.
*/
class MotionResource;

class AnimComponent : public AbstractComponent
{
public:
	AnimComponent(int updateOrder = 0) : AbstractComponent(updateOrder), m_pResource(nullptr) {}
	virtual ~AnimComponent();

	static const char* StaticName() { return "Anim"; }
	virtual const char* Name() { return StaticName(); }

	virtual AnimComponent* Clone() const;
	virtual void Clone(AnimComponent* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt);

public:
	virtual void Translate(const Coordinate& offset);
	virtual void SetGameObject(GameObject* object);

	Animation* GetAnim() { return &m_anim; }

private:
	Animation m_anim;

	/*
	** The offset to the center of the game object.
	*/
	Coordinate m_offset;

	MotionResource* m_pResource;
};


/********************************************************************
** State component manage the behavior of the game object, and it decide
** what the game object will do.
** Reference:
**   https://blog.csdn.net/guyu2019/article/details/112159600
*/

class BehaviorComponent;

class Behavior : public AbstractObject
{
public:
	Behavior() : AbstractObject(RTTIType::RTTI_BEHAVIOR), m_parent(nullptr) {}
	virtual ~Behavior() {}

	/*
	** Get the name of the behavior. I think... C-Style string is
	** enough.
	*/
	virtual const char* Name() const = 0;

	virtual Behavior* Clone() const = 0;
	virtual void Clone(Behavior* clone) const;

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}	// Do something when switch to the behavior.
	virtual void OnExit() {}	// Do something else when switch to other behavior.

public:
	void SetParent(BehaviorComponent* parent) { m_parent = parent; }
	BehaviorComponent* GetParent() { return m_parent; }

protected:
	BehaviorComponent* m_parent;
};

class BehaviorComponent : public AbstractComponent
{
public:
	BehaviorComponent(int updateOrder = 0) : AbstractComponent(updateOrder), m_pCurBehavior(nullptr) {}
	virtual ~BehaviorComponent();

	static const char* StaticName() { return "Behavior"; }
	virtual const char* Name() { return StaticName(); }

	virtual BehaviorComponent* Clone() const;
	virtual void Clone(BehaviorComponent* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt);

	void AddBehavior(Behavior* behavior);
	void ChangeBehavior(const char* name);

private:
	Behavior* _GetBehavior(const char* name);

	std::unordered_map<const char*, Behavior*> m_behaviors;
	Behavior* m_pCurBehavior;
};


/********************************************************************
** State component manage state of the game object. However, it is
** almost the same as behavior component.
*/
class StateComponent;

class State : public AbstractObject
{
public:
	State() : AbstractObject(RTTIType::RTTI_BEHAVIOR),
		m_parent(nullptr), m_duration(0L), m_elapsedTime(0L) {}
	virtual ~State() {}

	/*
	** Get the name of the state. I think... C-Style string is
	** enough.
	*/
	virtual const char* Name() const = 0;

	virtual State* Clone() const = 0;
	virtual void Clone(State* clone) const;

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt) = 0;
	virtual void OnEnter() {}	// Do something when switch to the behavior.
	virtual void OnExit() {}	// Do something else when switch to other behavior.

public:
	void SetParent(StateComponent* parent) { m_parent = parent; }
	StateComponent* GetParent() { return m_parent; }

protected:
	StateComponent* m_parent;

	clock_t m_duration;
	clock_t m_elapsedTime;
};

class StateComponent : public AbstractComponent
{
public:
	StateComponent(int updateOrder = 0) : AbstractComponent(updateOrder), m_pCurState(nullptr) {}
	virtual ~StateComponent();

	static const char* StaticName() { return "State"; }
	virtual const char* Name() { return StaticName(); }

	virtual StateComponent* Clone() const;
	virtual void Clone(StateComponent* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt);

	void AddState(State* behavior);
	void ChangeState(const char* name);

	const char* CurrentStateName() const { return m_pCurState->Name(); }

private:
	State* _GetState(const char* name);

	std::unordered_map<const char*, State*> m_states;
	State* m_pCurState;
};


/********************************************************************
** Since custom component can be defined, so use a class is better.
*/
class StandardComponentKit
{
public:
	StandardComponentKit() {}
	virtual ~StandardComponentKit() {}

	virtual AbstractComponent* LoadComponent(XMLElement* node);
};

#endif
