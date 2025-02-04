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
 * Overview:                                                                 *
 *   Some standard components.                                                *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/game/StandardComponent.h"

#include "../../inc/device/Explorer.h"
#include "../../inc/device/Timer.h"

#include "../../inc/utility/DMath.h"
#include "../../inc/utility/Parser.h"

/******************************************************************************
 * RigidBodyComponent::Clone                                                  *
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
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
RigidBodyComponent* RigidBodyComponent::Clone() const
{
    RigidBodyComponent* clone = new RigidBodyComponent(m_updateOrder);
    clone->_MakePrototype(false);

    AbstractComponent::Clone(clone);

    clone->m_id = m_id;
    clone->m_mass = m_mass;
    clone->m_ratio = m_ratio;
    clone->m_force = m_force;
    clone->m_velocity = m_velocity;
    clone->m_maxVelocity = m_maxVelocity;
    clone->m_strength = m_strength;

    return clone;
}

/******************************************************************************
 * RigidBodyComponent::Load                                                   *
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
bool RigidBodyComponent::Load(XMLElement* node)
{
    /*
    **	<RigidBody mass="" ratio="" max-velocity="" strength="" id="">
    **	</RigidBody>
    */
    const char* name = node->Name();
    const char* attr;

    _CHECK_TAG("RigidBody");
    _RETURN_IF_ERROR();

    AbstractComponent::Load(node);

    _PARSE("mass", m_mass, name, 0.0);
    _PARSE("ratio", m_ratio, name, 0.0);
    _PARSE("max-velocity", m_maxVelocity, name, 0.0);
    _PARSE("strength", m_strength, name, 0.0);
    _PARSE("id", m_id, name, 0);

    m_force = m_velocity = VECTOR_ZERO;

    _RETURN_STATE();
}

/******************************************************************************
 * RigidBodyComponent::Update -- Update rigid body.                           *
 *                                                                            *
 *    This will refresh the velocity using force info.                        *
 *                                                                            *
 * INPUT:   evnt -- Here, evnt is ignored.                                    *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void RigidBodyComponent::Update(Event* evnt)
{
    if (IsZero(m_mass))
    {
        return;
    }

    if (IsZero(m_force))
    {
        if (!IsTrivia(m_velocity))
        {
            m_velocity *= m_ratio;
        }
        else
        {
            m_velocity.Init(0.0, 0.0);
        }
    }
    else
    {
        m_velocity += m_force / m_mass;
    }

    double v = Module(m_velocity);
    if (v > m_maxVelocity)
    {
        m_velocity *= m_maxVelocity / v;
    }
}

/******************************************************************************
 * RigidBody::Freeze -- Freeze the object.                                    *
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
 *   2022/06/26 Tony : Created.                                               *
 *============================================================================*/
void RigidBodyComponent::Freeze()
{
    m_force.Init(VECTOR_ZERO);
    m_velocity.Init(VECTOR_ZERO);
}

/******************************************************************************
 * RigidBody::Scatter -- Hmm... Scatter?                                      *
 *                                                                            *
 *    Like pressing X in Red Alert 2? :P If speed is omitted, then it will be *
 *    random between 0 and maxVelocity.                                       *
 *                                                                            *
 * INPUT:   speed -- The initial speed of scatter.                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  Haha, be careful if max velocity is not assigned. :)            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/26 Tony : Created.                                               *
 *============================================================================*/
void RigidBodyComponent::Scatter()
{
    Scatter(Random(m_maxVelocity));
}

void RigidBodyComponent::Scatter(double speed)
{
    // m_velocity += Rotate(VECTOR_NX, Random(TWO_PI)) * speed;
    m_velocity += Rotate(VECTOR_NX, Random(TWO_PI)) * Random(speed * 0.5, speed);
}

/******************************************************************************
 * ColliderBoxComponent::Clone                                                 *
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
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
ColliderBoxComponent* ColliderBoxComponent::Clone() const
{
    ColliderBoxComponent* clone = new ColliderBoxComponent(m_updateOrder);
    clone->_MakePrototype(false);

    AbstractComponent::Clone(clone);

    clone->m_offset = m_offset;
    clone->m_center = m_center;
    clone->m_box = m_box;
    clone->m_border = m_border;

    return clone;
}

/******************************************************************************
 * ColliderBoxComponent::Load                                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  There is a hidden bug that when the component is first used,    *
 *            the position of the box may be uncertain.                       *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
bool ColliderBoxComponent::Load(XMLElement* node)
{
    /*
    **	<ColliderBox offset="" box-size="" border-size="">
    **	</ColliderBox>
    */
    const char* name = node->Name();
    const char* attr;

    _CHECK_TAG("ColliderBox");
    _RETURN_IF_ERROR();

    AbstractComponent::Load(node);

    Coordinate pair;
    _PARSE_PRIVATE("offset", m_offset, name, ParseCoord);

    _PARSE_PRIVATE_ESSENTIAL("box-size", pair, name, ParseCoord);
    _RETURN_IF_ERROR();
    m_box.width = pair.x;
    m_box.height = pair.y;

    _PARSE_PRIVATE("border-size", pair, name, ParseCoord);
    m_border.width = pair.x;
    m_border.height = pair.y;

    // Update(nullptr);

    _RETURN_STATE();
}

/******************************************************************************
 * ColliderBoxComponent::Update                                               *
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
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void ColliderBoxComponent::Update(Event* evnt)
{
    m_center = m_pGameObject->GetCoord() + m_offset;

    m_box.x = m_center.x - (m_box.width >> 1);
    m_box.y = m_center.y - (m_box.height >> 1);

    m_border.x = m_center.x - (m_border.width >> 1);
    m_border.y = m_center.y - (m_border.height >> 1);
}

/******************************************************************************
 * ColliderBoxComponent::Translate -- Translate collide box.                  *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void ColliderBoxComponent::Translate(const Coordinate& offset)
{
    m_center += offset;
    m_box.pos += offset;
    m_border.pos += offset;
}

/******************************************************************************
 * MoveComponent::Clone -- Clone move component.                              *
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
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
MoveComponent* MoveComponent::Clone() const
{
    MoveComponent* clone = new MoveComponent(m_updateOrder);
    clone->_MakePrototype(false);

    AbstractComponent::Clone(clone);

    return clone;
}

/******************************************************************************
 * MoveComponent::Load -- Load move component.                                *
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
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
bool MoveComponent::Load(XMLElement* node)
{
    /*
    **	<Move>
    **	</Move>
    */
    AbstractComponent::Load(node);

    return true;
}

/******************************************************************************
 * MoveComponent::Update -- Update move component.                            *
 *                                                                            *
 *    Move component only needs to know the speed of the game object, which is*
 *    in RigidBody and should be updated earlier.                             *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/21 Tony : Created.                                               *
 *============================================================================*/
void MoveComponent::Update(Event* evnt)
{
    RigidBodyComponent* rigid = m_pGameObject->GetComponent<RigidBodyComponent>();
    auto& v = rigid->GetVelocity();

    m_pGameObject->Translate({ static_cast<int>(v.x), static_cast<int>(v.y) });

    rigid->GetForce().Init(VECTOR_ZERO);
}

/******************************************************************************
 * AnimComponent::~AnimComponent -- Destructor of the object.                 *
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
 *   2022/07/05 Tony : Created.                                               *
 *============================================================================*/
AnimComponent::~AnimComponent()
{
    if (_IsPrototype())
    {
        if (m_pResource)
        {
            m_pResource->Release();
        }
    }
}

/******************************************************************************
 * AnimComponent::Clone -- Clone it.                                          *
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
 *   2022/07/02 Tony : Created.                                               *
 *============================================================================*/
AnimComponent* AnimComponent::Clone() const
{
    AnimComponent* clone = new AnimComponent(m_updateOrder);
    clone->_MakePrototype(false);

    AbstractComponent::Clone(clone);

    clone->m_anim = m_anim;
    clone->m_offset = m_offset;

    return clone;
}

/******************************************************************************
 * AnimComponent::Load -- Load anim component.                                *
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
 *   2022/07/02 Tony : Created.                                               *
 *============================================================================*/
bool AnimComponent::Load(XMLElement* node)
{
    /*
    **	<Anim src=""></Anim>
    */
    const char* name = node->Name();
    const char* attr;

    _CHECK_TAG("Anim");
    _RETURN_IF_ERROR();

    AbstractComponent::Load(node);

    attr = node->Attribute("src");
    if (!attr)
    {
        LOG_ERROR(MISSING_ATTRIBUTE, "src", name);
        return false;
    }

    m_pResource = LoadResource<MotionResource>(attr);
    m_anim.Initialize(m_pResource->GetResource());

    _PARSE_PRIVATE("offset", m_offset, name, ParseCoord);

    _RETURN_STATE();
}

/******************************************************************************
 * AnimComponent::Update -- Update animation.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/02 Tony : Created.                                               *
 *============================================================================*/
void AnimComponent::Update(Event* evnt)
{
    m_anim.Update();

    m_pGameObject->GetSymbol()->SetImage(m_anim.GetFrame())->SetCoord(m_pGameObject->GetCoord());
}

/******************************************************************************
 * AnimComponent::Translate -- Translate animation.                           *
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
 *   2022/08/03 Tony : Created.                                               *
 *============================================================================*/
void AnimComponent::Translate(const Coordinate& offset)
{
    m_pGameObject->GetSymbol()->Translate(offset);
}

/******************************************************************************
 * AnimComponent::SetGameObject -- Set game object.                           *
 *                                                                            *
 *    Anim component will set the offset of the object's symbol.              *
 *                                                                            *
 * INPUT:   object                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/16 Tony : Created.                                               *
 *============================================================================*/
void AnimComponent::SetGameObject(GameObject* object)
{
    m_pGameObject = object;
    m_pGameObject->GetSymbol()->SetOffset(m_offset);
}

/******************************************************************************
 * Behavior::Clone -- Clone behavior.                                         *
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
 *   2022/07/02 Tony : Created.                                               *
 *============================================================================*/
void Behavior::Clone(Behavior* clone) const
{
    AbstractObject::Clone(clone);
}

/******************************************************************************
 * Behavior::Load -- Load behavior.                                           *
 *                                                                            *
 *    Infact does nothing...                                                  *
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
bool Behavior::Load(XMLElement* node)
{
    return true;
}

/******************************************************************************
 * BehaviorComponent::~BehaviorComponent -- Destructor of the object.         *
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
 *   2022/06/27 Tony : Created.                                               *
 *============================================================================*/
BehaviorComponent::~BehaviorComponent()
{
    for (auto it = m_behaviors.begin(); it != m_behaviors.end(); it++)
    {
        delete it->second;
    }
}

/******************************************************************************
 * BehaviorComponent::Clone -- Clone behavior component. *
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
 *   2022/06/27 Tony : Created.                                               *
 *============================================================================*/
BehaviorComponent* BehaviorComponent::Clone() const
{
    BehaviorComponent* clone = new BehaviorComponent(m_updateOrder);
    clone->_MakePrototype(false);

    AbstractComponent::Clone(clone);

    for (auto it = m_behaviors.begin(); it != m_behaviors.end(); it++)
    {
        if (it->second == m_pCurBehavior)
        {
            clone->m_pCurBehavior = it->second->Clone();
            clone->AddBehavior(clone->m_pCurBehavior);
        }
        else
        {
            clone->AddBehavior(it->second->Clone());
        }
    }

    return clone;
}

/******************************************************************************
 * BehaviorComponent::Load -- Load behavior component *
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
 *   2022/06/27 Tony : Created.                                               *
 *============================================================================*/
bool BehaviorComponent::Load(XMLElement* node)
{
    AbstractComponent::Load(node);

    return true;
}

/******************************************************************************
 * BehaviorComponent::Update -- Update behavior. *
 *                                                                            *
 *    Well, the main behavior logic is done here.                             *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/27 Tony : Created.                                               *
 *============================================================================*/
void BehaviorComponent::Update(Event* evnt)
{
    if (m_pCurBehavior)
    {
        m_pCurBehavior->Update(evnt);
    }
}

/******************************************************************************
 * BehaviorComponent::AddBehavior -- Add behavior. *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   behavior *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/27 Tony : Created.                                               *
 *============================================================================*/
void BehaviorComponent::AddBehavior(Behavior* behavior)
{
    behavior->SetParent(this);
    m_behaviors.emplace(behavior->Name(), behavior);
}

/******************************************************************************
 * BehaviorComponent::ChangeBehavior -- Change behavior. *
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
 *   2022/06/27 Tony : Created.                                               *
 *============================================================================*/
void BehaviorComponent::ChangeBehavior(const char* name)
{
    Behavior* behavior = _GetBehavior(name);

#ifdef DUNGINE_DEBUG
    if (!behavior)
    {
        LOG_ERROR(CRITICAL_ERROR);
        return;
    }
#endif

    if (behavior != m_pCurBehavior)
    {
        if (m_pCurBehavior)
        {
            m_pCurBehavior->OnExit();
        }
        behavior->OnEnter();
        m_pCurBehavior = behavior;
    }
}

/******************************************************************************
 * BehaviorComponent::_GetBehavior -- Get behavior by name.                   *
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
 *   2022/06/27 Tony : Created.                                               *
 *============================================================================*/
Behavior* BehaviorComponent::_GetBehavior(const char* name)
{
    auto it = m_behaviors.find(name);

    if (it == m_behaviors.end())
    {
        LOG_ERROR(R"(Missing Behavior "%s")", name);
        return nullptr;
    }
    else
    {
        return it->second;
    }
}

/******************************************************************************
 * State::Clone -- Clone state.                                               *
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
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void State::Clone(State* clone) const
{
    AbstractObject::Clone(clone);

    clone->m_duration = m_duration;
    clone->m_elapsedTime = m_elapsedTime;
}

/******************************************************************************
 * State::Load -- Load state.                                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  Return result.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
bool State::Load(XMLElement* node)
{
    return true;
}

/******************************************************************************
 * StateComponent::~StateComponent -- Destructor of the object.               *
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
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
StateComponent::~StateComponent()
{
    for (auto it = m_states.begin(); it != m_states.end(); it++)
    {
        delete it->second;
    }
}

/******************************************************************************
 * StateComponent::Clone -- Clone state component.                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return the clone.                                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
StateComponent* StateComponent::Clone() const
{
    StateComponent* clone = new StateComponent(m_updateOrder);
    clone->_MakePrototype(false);

    AbstractComponent::Clone(clone);

    for (auto it = m_states.begin(); it != m_states.end(); it++)
    {
        if (it->second == m_pCurState)
        {
            clone->m_pCurState = it->second->Clone();
            clone->AddState(clone->m_pCurState);
        }
        else
        {
            clone->AddState(it->second->Clone());
        }
    }

    return clone;
}

/******************************************************************************
 * StateComponent::Load -- Load state component.                              *
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
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
bool StateComponent::Load(XMLElement* node)
{
    AbstractComponent::Load(node);

    return true;
}

/******************************************************************************
 * StateComponent::Update -- Update state component.                          *
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
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void StateComponent::Update(Event* evnt)
{
    if (m_pCurState)
    {
        m_pCurState->Update(evnt);
    }
}

/******************************************************************************
 * StateComponent::AddState -- Add state to the component.                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   state                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void StateComponent::AddState(State* state)
{
    state->SetParent(this);
    m_states.emplace(state->Name(), state);
}

/******************************************************************************
 * StateComponent::ChangeState -- Change state.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void StateComponent::ChangeState(const char* name)
{
    State* state = _GetState(name);

#ifdef DUNGINE_DEBUG
    if (!state)
    {
        LOG_ERROR(CRITICAL_ERROR);
        return;
    }
#endif

    if (state != m_pCurState)
    {
        if (m_pCurState)
        {
            m_pCurState->OnExit();
        }
        state->OnEnter();
        m_pCurState = state;
    }
}

/******************************************************************************
 * StateComponent::_GetState -- Get state.                                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name                                                              *
 *                                                                            *
 * OUTPUT:  Return the state.                                                 *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
State* StateComponent::_GetState(const char* name)
{
    auto it = m_states.find(name);

    if (it == m_states.end())
    {
        return nullptr;
    }
    else
    {
        return it->second;
    }
}

/******************************************************************************
 * StandardComponentKit::LoadComponent -- Load basic components.              *
 *                                                                            *
 *    This is the last one, so it should log error if failed.                 *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
AbstractComponent* StandardComponentKit::LoadComponent(XMLElement* node)
{
    if (!node)
    {
        return nullptr;
    }

    const char* name = node->Name();
    AbstractComponent* rv = nullptr;

    if (_STR_SAME(name, "Anim"))
    {
        rv = new AnimComponent();
    }
    else if (_STR_SAME(name, "RigidBody"))
    {
        rv = new RigidBodyComponent();
    }
    else if (_STR_SAME(name, "ColliderBox"))
    {
        rv = new ColliderBoxComponent();
    }
    else if (_STR_SAME(name, "Behavior"))
    {
        rv = new BehaviorComponent();
    }
    else if (_STR_SAME(name, "State"))
    {
        rv = new StateComponent();
    }
    else if (_STR_SAME(name, "Move"))
    {
        rv = new MoveComponent();
    }

    if (rv)
    {
        rv->Load(node);
    }
    else
    {
        LOG_ERROR(UNKNOWN_TAG, name);
    }

    return rv;
}
