/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Scene.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 9, 2022                              *
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

#ifndef _SCENE_H_
#define _SCENE_H_

#include <easyx.h>

#include "GameObject.h"
#include "Camera.h"


class Event;

/********************************************************************
** Well... This Scene is not very nice. Hmm... Just a base? :P
*/
class Scene
{
public:
	Scene() : m_isUpdating(false) {}
	virtual ~Scene() {}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	void AddObject(GameObject* object);
	void AddObjects(std::vector<GameObject*>& pool);
	void RemoveObject(GameObject* object);
	void RemoveObjects(std::vector<GameObject*>& pool);

	// Delete object can't happen during update.
protected:
	void _DeleteObject(GameObject* object);
	void _DeleteObjects(std::vector<GameObject*>& pool);

public:
	Camera* GetCamera() { return &m_camera; }

protected:
	void _UpdateObjectPool();

	ObjectPool m_gameObjects;
	ObjectPool m_pendingObjects;
	ObjectPool m_dirtyObjects;	// objects that removed during update

	Camera m_camera;

	bool m_isUpdating;
};

#endif