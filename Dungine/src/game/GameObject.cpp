/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : GameObject.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 4, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For the base class of all game object. Game objects are the... physical  *
 *   objects that you can actually see one the battlefield, including walls,  *
 *   weapons, figures and so on. Components, animations are not one of it.    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include <algorithm>

#include "../../inc/device/Device.h"

#include "../../inc/game/GameObject.h"
#include "../../inc/game/AbstractComponent.h"
#include "../../inc/game/Scene.h"

/******************************************************************************
 * GameObject::GameObject -- Constructor of the object.                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   RTTI -- The RTTI type of the object.                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *   2022/07/03 Tony : Symbol of game objects should be static.               *
 *============================================================================*/
GameObject::GameObject(RTTIType RTTI) : AbstractObject(RTTI),
	m_pScene(nullptr),
	m_isActive(true),
	m_isValid(true),
	m_isToDisplay(true)
{
	m_symbol.SetStatic(true);
}

GameObject::GameObject(RTTIType RTTI, Scene* scene) : AbstractObject(RTTI),
	m_pScene(scene),
	m_isActive(true),
	m_isValid(true),
	m_isToDisplay(true)
{
	m_symbol.SetStatic(true);
}


/******************************************************************************
 * GameObject::~GameObject -- Destructor of the object.                       *
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
GameObject::~GameObject()
{
	for (auto it = m_components.begin(); it != m_components.end(); it++)
		delete it->second;
}


/******************************************************************************
 * GameObject::Clone -- Clone game object.                                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   clone -- clone to a existing object.                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void GameObject::Clone(GameObject* clone) const
{
	AbstractObject::Clone(clone);
	
	clone->m_coord = m_coord;
	clone->m_pScene = m_pScene;

	clone->m_symbol = m_symbol;
	clone->m_isActive = m_isActive;
	clone->m_isValid = m_isValid;
	clone->m_isToDisplay = m_isToDisplay;

	for (auto it = m_components.begin(); it != m_components.end(); it++)
		clone->AddComponent(it->second->Clone());
}


/******************************************************************************
 * GameObject::Update -- Update game object.                                  *
 *                                                                            *
 *    All updates of gameobject is done by its components.                    *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void GameObject::Update(Event* evnt)
{
	if (m_isActive)
	{
		for (auto it = m_cmptUpdateQueue.begin(); it != m_cmptUpdateQueue.end(); it++)
			it->second->Update(evnt);
	}
}


/******************************************************************************
 * GameObject::Draw -- Draw game object.                                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   pDestImage                                                        *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/24 Tony : Created.                                               *
 *============================================================================*/
void GameObject::Draw()
{
	if (m_isToDisplay)
		Device::GetInstance()->AddSymbol(&m_symbol);
}

/******************************************************************************
 * GameObject::AddComponent -- Add component to the game object.              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   cmpt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void GameObject::AddComponent(AbstractComponent* cmpt)
{
	cmpt->SetGameObject(this);
	m_components.emplace(cmpt->Name(), cmpt);
	m_cmptUpdateQueue.emplace(cmpt->UpdateOrder(), cmpt);
}


/******************************************************************************
 * ObjectPool::~ObjectPool -- Destructor of the object.                       *
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
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
ObjectPool::~ObjectPool()
{
	Destroy();
}


/******************************************************************************
 * ObjectPool::Update -- Update all the objects int the pool.                 *
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
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void ObjectPool::Update(Event* evnt)
{
	for (auto it = m_pool.begin(); it != m_pool.end(); it++)
		(*it)->Update(evnt);
}


/******************************************************************************
 * ObjectPool::Draw -- Draw all objects in the pool.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   camera -- Draw to the camera.                                     *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void ObjectPool::Draw()
{
	for (auto it = m_pool.begin(); it != m_pool.end(); it++)
		(*it)->Draw();
}

void ObjectPool::Draw(Camera* camera)
{
	for (auto it = m_pool.begin(); it != m_pool.end(); it++)
		camera->Capture(*it);
}


/******************************************************************************
 * ObjectPool::AddObject -- Add object to the pool.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   object                                                            *
 *          pool                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void ObjectPool::AddObject(GameObject* object)
{
	m_pool.push_back(object);
}

void ObjectPool::AddObjects(std::vector<GameObject*>& pool)
{
	for (auto it = pool.begin(); it != pool.end(); it++)
		AddObject(*it);
}


/******************************************************************************
 * ObjectPool::RemoveObject -- Remove certain object in the pool.             *
 * ObjectPool::DeleteObject -- Delete certain object in the pool.             *
 *                                                                            *
 *    Remove only remove the object, it does not delete it.                   *
 *                                                                            *
 * INPUT:   object                                                            *
 *          pool                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
bool ObjectPool::RemoveObject(GameObject* object)
{
	auto it = std::find(m_pool.begin(), m_pool.end(), object);
	if (it != m_pool.end())
	{
		_RemoveObject(it);
		return true;
	}
	else
		return false;
}

void ObjectPool::RemoveObjects(std::vector<GameObject*>& pool)
{
	for (auto it = pool.begin(); it != pool.end(); it++)
		RemoveObject(*it);
}

bool ObjectPool::DeleteObject(GameObject* object)
{
	auto it = std::find(m_pool.begin(), m_pool.end(), object);
	if (it != m_pool.end())
	{
		_DeleteObject(it);
		return true;
	}
	else
		return false;
}

void ObjectPool::DeleteObjects(std::vector<GameObject*>& pool)
{
	for (auto it = pool.begin(); it != pool.end(); it++)
		DeleteObject(*it);
}


/******************************************************************************
 * ObjectPool::Clear -- Clear object pool.                                    *
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
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void ObjectPool::Clear()
{
	m_pool.clear();
}

void ObjectPool::Destroy()
{
	for (auto it = m_pool.begin(); it != m_pool.end(); it++)
		delete (*it);
	m_pool.clear();
}


/******************************************************************************
 * ObjectPool::_RemoveObject -- Remove the object.                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   it -- The iterator of the object to remove.                       *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void ObjectPool::_RemoveObject(std::vector<GameObject*>::iterator it)
{
	std::swap(*it, *(m_pool.end() - 1));
	m_pool.pop_back();
}

void ObjectPool::_DeleteObject(std::vector<GameObject*>::iterator it)
{
	delete (*it);
	std::swap(*it, *(m_pool.end() - 1));
	m_pool.pop_back();
}