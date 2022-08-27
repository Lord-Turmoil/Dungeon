/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : AbstractLibrary.h                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide a template for library. Library is used to store prototypes.     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _ABSTRACT_LIBRARY_H_
#define _ABSTRACT_LIBRARY_H_

#include <map>
#include <string>


/********************************************************************
** Notice that, the item got from library is the prototype itself,
** client should clone manually.
*/
template<typename ProtoType, typename KeyType = std::string>
class AbstractLibrary
{
public:
	AbstractLibrary() {}
	virtual ~AbstractLibrary() {}

	ProtoType* AddItem(const KeyType& name, ProtoType* item)
	{
		auto it = m_pool.find(name);
		if (it == m_pool.end())
			m_pool.emplace(name, item);
		else
		{
			LOG_ERROR(NAME_CONFLICT, "Library", name.c_str());
			delete it->second;
			it->second = item;
		}

		return item;
	}

	ProtoType* GetItem(const KeyType& name)
	{
		auto it = m_pool.find(name);
		if (it == m_pool.end())
		{
			LOG_ERROR("\"%s\" doesn't exist in library", name.c_str());
			return nullptr;
		}
		else
			return it->second;
	}

	virtual bool Load(XMLElement* node) = 0;
	virtual void UnLoad() = 0;

protected:
	std::map<KeyType, ProtoType*> m_pool;
};

template<typename ProtoType, typename KeyType>
class AbstractLibrary<ProtoType, KeyType*>
{
public:
	AbstractLibrary() {}
	virtual ~AbstractLibrary() {}

	ProtoType* AddItem(const KeyType* name, ProtoType* item)
	{
		auto it = m_pool.find(name);
		if (it == m_pool.end())
			m_pool.emplace(name, item);
		else
		{
			LOG_ERROR(NAME_CONFLICT, "Library", name.c_str());
			delete it->second;
			it->second = item;
		}

		return item;
	}

	ProtoType* GetItem(const KeyType* name)
	{
		auto it = m_pool.find(name);
		if (it == m_pool.end())
		{
			LOG_ERROR("\"%s\" doesn't exist in library", name.c_str());
			return nullptr;
		}
		else
			return it->second;
	}

	virtual bool Load(XMLElement* node) = 0;
	virtual void UnLoad() = 0;

protected:
	std::map<KeyType*, ProtoType*> m_pool;
};

#endif