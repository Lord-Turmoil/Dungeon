/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Singleton.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9                                   *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The base class of all classes that use singleton pattern.                *
 * -------------------------------------------------------------------------- *
 * Reference:                                                                 *
 *   https://codebus.cn/sunxiaoyu/quadtree-collision                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _SINGLETON_H_
#define _SINGLETON_H_


/********************************************************************
** Lazy singleton.
*/
template<typename T>
class Singleton
{
public:
	static T* GetInstance()
	{
		static Garbo m_garbo;

		if (!m_instance)
			m_instance = new T();
		return m_instance;
	}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

protected:
	/*
	** This is used to auto-free the instance.
	*/
	class Garbo
	{
	public:
		~Garbo()
		{
			if (m_instance)
				delete m_instance;
		}
	};

	Singleton() {}
	virtual ~Singleton() {}

	static T* m_instance;
};

template<typename T>
T* Singleton<T>::m_instance = nullptr;


/********************************************************************
** Hungry singleton.
*/
template<typename T>
class SingletonHungry
{
public:
	static T* GetInstance()
	{
		static Garbo m_garbo;

		// This instance will always be available.
		return m_instance;
	}

	SingletonHungry(const SingletonHungry&) = delete;
	SingletonHungry& operator=(const SingletonHungry&) = delete;

protected:
	/*
	** This is used to auto-free the instance.
	*/
	class Garbo
	{
	public:
		~Garbo()
		{
			if (m_instance)
				delete m_instance;
		}
	};

	SingletonHungry() = default;
	virtual ~SingletonHungry() = default;

	static T* m_instance;
};

template<typename T>
T* SingletonHungry<T>::m_instance = new T();

#endif