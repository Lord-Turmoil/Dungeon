/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Buzzer.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : March 10, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Widget can use buzzer to make sound.                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "../utility/tinyxml.h"

class MonoSound;

class Buzzer
{
public:
	Buzzer() : m_pSound(nullptr) {}

	/*
	** Buzzer doesn't need to delete the sound since it is managed by explorer.
	*/
	~Buzzer() {}

	bool Load(XMLElement* node);

	void Play();

private:
	MonoSound* m_pSound;
};

Buzzer* LoadBuzzer(XMLElement* node);

#endif