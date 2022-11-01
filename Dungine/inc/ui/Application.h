/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Application.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 17, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Dungine's main loop is encapsulated in the interface mangager, which is  *
 *   also called Application.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <stack>
#include <unordered_map>

#include "../template/Singleton.h"


class AbstractInterface;

/********************************************************************
** Application manage all interfaces, and it is the entrance
** of the whole game. There should only be one of it.
*/
class Application : public Singleton<Application>
{
	friend class Singleton<Application>;
public:
	Application();
	virtual ~Application();

	bool Load(const char* filename);
	void UnLoad();

	/*
	** Initialize properties for start up. Must be called before
	** Run.
	*/
	bool Initialize();

	/*
	** Well, technically the main entrance of the whole game. This
	** is the main loop of the game.
	*/
	void Run();

	/*
	** Vital function for interface transition. Quite clumsy, it simply
	** assume that no misuse happens.
	*/
	void Launch(const std::string& name);
	void Attatch(const std::string& name);
	void Terminate();
	void Detach();

public:
	AbstractInterface* AddInterface(AbstractInterface* intf);
	AbstractInterface* GetInterface(const std::string& name);
	AbstractInterface* GetCurrentInterface() { return m_pCurIntf; }
	AbstractInterface* operator[](const std::string& name)
	{
		return m_interfaces[name];
	}

	void SetFPS(int fps) { m_delay = 1000 / fps; }

	auto& GetInterfacePool() { return m_interfaces; }

protected:
	/*
	** Start up interface is the first interface in the xml file, or
	** the first one that is added to the manager.
	*/
	AbstractInterface* m_pStartIntf;

	AbstractInterface* m_pCurIntf;

	int m_delay;	// determine the time of each frame

	bool m_isRunning;

	std::stack<AbstractInterface*> m_activeInterfaces;
	std::unordered_map<std::string, AbstractInterface*> m_interfaces;
};


#endif