/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Scene.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 13, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide basic game core.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/common/Common.h"
#include "../../inc/game/Scene.h"


/******************************************************************************
 * Scene::FunctionName -- Literally.                                          *
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
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Scene::AddObject(GameObject* object)
{
	object->SetScene(this);
	if (m_isUpdating)
		m_pendingObjects.AddObject(object);
	else
		m_gameObjects.AddObject(object);
}

void Scene::AddObjects(std::vector<GameObject*>& pool)
{
	if (m_isUpdating)
	{
		for (auto it = pool.begin(); it != pool.end(); it++)
		{
			(*it)->SetScene(this);
			m_pendingObjects.AddObject(*it);
		}
	}
	else
	{
		for (auto it = pool.begin(); it != pool.end(); it++)
		{
			(*it)->SetScene(this);
			m_gameObjects.AddObject(*it);
		}
	}
}

void Scene::RemoveObject(GameObject* object)
{
	if (m_isUpdating)
		m_dirtyObjects.AddObject(object);
	else
	{
		if (!m_gameObjects.RemoveObject(object))
		{
			if (!m_pendingObjects.RemoveObject(object))
				LOG_ERROR("Invalid object to remove");
		}
	}
}

void Scene::RemoveObjects(std::vector<GameObject*>& pool)
{
	if (m_isUpdating)
		m_dirtyObjects.AddObjects(pool);
	else
	{
		for (auto it = pool.begin(); it != pool.end(); it++)
			RemoveObject(*it);
	}
}

void Scene::_DeleteObject(GameObject* object)
{
	if (!m_gameObjects.DeleteObject(object))
	{
		if (!m_pendingObjects.DeleteObject(object))
			LOG_ERROR("Invalid object to delete");
	}
}

void Scene::_DeleteObjects(std::vector<GameObject*>& pool)
{
	for (auto it = pool.begin(); it != pool.end(); it++)
		DeleteObject(*it);
}


/******************************************************************************
 * Scene::_UpdateObjectPool -- Update object pools.                           *
 *                                                                            *
 *    Move pending objects to object pool, and remove invalid objects.        *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/13 Tony : Created.                                               *
 *============================================================================*/
void Scene::_UpdateObjectPool()
{
	m_gameObjects.AddObjects(m_pendingObjects.GetPool());
	m_pendingObjects.Clear();

	// remove dirty ones.
	RemoveObjects(m_dirtyObjects.GetPool());
	m_dirtyObjects.Clear();

	// remove invalid ones.
	std::vector<GameObject*> invalidObjects;
	auto& pool = m_gameObjects.GetPool();
	for (auto it = pool.begin(); it != pool.end(); it++)
	{
		if (!((*it)->IsValid()))
			invalidObjects.push_back(*it);
	}

	m_gameObjects.DeleteObjects(invalidObjects);
}