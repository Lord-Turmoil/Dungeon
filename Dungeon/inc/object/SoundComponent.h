/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : SoundComponent.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 13, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Sound component.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _SOUND_COMPONENT_H_
#define _SOUND_COMPONENT_H_

#include <dungine.h>

class SoundComponent : public AbstractComponent
{
public:
	SoundComponent(int updateOrder = 0) : AbstractComponent(updateOrder) {}
	virtual ~SoundComponent();

	static const char* StaticName() { return "Sound"; }
	virtual const char* Name() { return StaticName(); }

	virtual SoundComponent* Clone() const;
	virtual void Clone(SoundComponent* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt) {}

public:
	void AddSound(const std::string& name, AudioResource* sound);
	void Play(const std::string& name);

private:
	/*
	** Hmm... Here, audio resource must be MonoSound.
	*/
	std::unordered_map<std::string, AudioResource*> m_sounds;
};

#endif