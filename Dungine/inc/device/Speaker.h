/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Speaker.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 14, 2022                             *
 *                                                                            *
 *                    Last Update : November 24, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The sound module of the engine. It manages all sound and music in play.  *
 *   On the one hand, it take possession of play lists in a stack. On the     *
 *   other hand, it can play instant sounds.                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _SPEAKER_H_
#define _SPEAKER_H_

#include <stack>

#include "../template/Singleton.h"
#include "../utility/fmodcore.h"

struct DSound;
class PlayList;

class Speaker : public Singleton<Speaker>
{
	friend class Singleton<Speaker>;
public:
	bool Create();
	bool Destroy();

	void Play(DSound* sound);		// mono sound in differnt channel
	void PlayShared(DSound* sound);	// mono sound in shared channel
	void PlayStream(DSound* sound);	// background music

	/*
	** 2022/08/12 TS:
	** FMOD system must update on each frame!!! :(
	*/
	void Update();

	// For play list
	void Play();
	void Pause(bool pause);
	void Stop();
	void AddPlayList(PlayList* playList);
	void RemovePlayList();
	bool IsPlaying() const;
public:
	void SetSoundVolume(double volume) { m_soundVolume = volume; }
	void SetMusicVolume(double volume);

	double GetSoundVolume() const { return m_soundVolume; }
	// double GetMusicVolume() const { return m_soundVolume; }
	double GetMusicVolume() const { return m_musicVolume; }	// Be careful...

	FMOD::System* System() { return m_speaker; }

private:
	Speaker();
	~Speaker();

	void _Destroy();

	FMOD::System* m_speaker;
	FMOD::Channel* m_channel;

	std::stack<PlayList*> m_stack;

	/*
	** FMOD uses float as volume, however, for compatibility with
	** widgets' callback, here use double.
	*/
	double m_soundVolume;	// Sound effect, such as explosion...
	double m_musicVolume;	// Background music.
	bool m_isPaused;
};


#endif