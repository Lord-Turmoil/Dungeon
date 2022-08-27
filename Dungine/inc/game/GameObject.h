/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : GameObject.h                              *
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

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <map>
#include <vector>
#include <unordered_map>

#include "AbstractObject.h"
#include "../device/Symbol.h"


class Event;

/********************************************************************
** The scene of the game... Emm, the game itself.
*/
class Scene;

/********************************************************************
** Components of game object. Will be declared somewhere else.
*/
class AbstractComponent;

class GameObject : public AbstractObject
{
public:
	GameObject(RTTIType RTTI);
	GameObject(RTTIType RTTI, Scene* scene);	// better
	virtual ~GameObject();

	virtual GameObject* Clone() const = 0;
	virtual void Clone(GameObject* clone) const;

	virtual bool Load(XMLElement* node) { return true; }
	
	virtual void Update(Event* evnt);
	virtual void Draw();

public:
	/*
	** The coordinate of the object and the center of it are
	** the same by default.
	*/
	void SetCoord(const Coordinate& coord) { m_coord = coord; }
	Coordinate GetCoord() const { return m_coord; }
	virtual void SetCenter(const Coordinate& center) { m_coord = center; }
	virtual Coordinate GetCenter() const { return m_coord; }
	virtual void Translate(const Coordinate& offset) { m_coord += offset; }

	void SetScene(Scene* scene) { m_pScene = scene; }
	Scene* GetScene() { return m_pScene; }

	void AddComponent(AbstractComponent* cmpt);
	template<typename T> T* GetComponent()
	{
		auto it = m_components.find(T::StaticName());
		if (it != m_components.end())
			return static_cast<T*>(it->second);
		else
			return nullptr;
	}

public:
	void SetActive(bool active) { m_isActive = active; }
	void SetValid(bool isValid) { m_isValid = isValid; }
	void SetDisplay(bool isToDisplay) { m_isToDisplay = isToDisplay; }
	
	bool IsValid() const { return m_isValid; }
	bool IsToDisplay() const { return m_isToDisplay; }
	bool IsActive() const { return m_isActive; }
	
	Symbol* GetSymbol() { return &m_symbol; }

protected:
	/*
	** Well, coordinate is the center of the object.
	*/
	Coordinate m_coord;

	Scene* m_pScene;

	std::unordered_map<const char*, AbstractComponent*> m_components;
	std::multimap<int, AbstractComponent*> m_cmptUpdateQueue;

	/*
	** GameObject must have a symbol to be visible. And most of
	** the time, it is a static symbol.
	*/
	Symbol m_symbol;

	bool m_isActive : 1;
	bool m_isValid : 1;
	bool m_isToDisplay : 1;	// Whether to be rendered in the next frame.
};


/********************************************************************
** Object pool makes it easier to manage lots of objects.
*/
class Camera;

class ObjectPool
{
public:
	ObjectPool() {}
	~ObjectPool();

	void Update(Event* evnt);
	void Draw();
	void Draw(Camera* camera);
	
	void AddObject(GameObject* object);
	void AddObjects(std::vector<GameObject*>& pool);
	bool RemoveObject(GameObject* object);
	void RemoveObjects(std::vector<GameObject*>& pool);
	bool DeleteObject(GameObject* object);
	void DeleteObjects(std::vector<GameObject*>& pool);
	
	void Clear();
	void Destroy();

	std::vector<GameObject*>& GetPool() { return m_pool; }

	int Size() { return (int)m_pool.size(); }

private:
	void _RemoveObject(std::vector<GameObject*>::iterator it);
	void _DeleteObject(std::vector<GameObject*>::iterator it);

	std::vector<GameObject*> m_pool;
};

#endif