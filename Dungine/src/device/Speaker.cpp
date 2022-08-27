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
 *                    Last Update : August 12, 2022                           *
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
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/common/Common.h"
#include "../../inc/device/Speaker.h"
#include "../../inc/device/Sound.h"



/******************************************************************************
 * Speaker::Create -- Create speaker.                                         *
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
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool Speaker::Create()
{
	if (!m_speaker)
	{
		FMOD::System_Create(&m_speaker);

		if (m_speaker->init(32, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
		{
			LOG_ERROR(CANNOT_INIT_SPEAKER);
			return false;
		}
	}

	return true;
}


/******************************************************************************
 * Speaker::Destroy -- Destroy speaker.                                       *
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
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool Speaker::Destroy()
{
	if (m_speaker)
	{
		_Destroy();
		m_speaker = nullptr;
		if (Logger::Error())
			return false;
	}

	return true;
}


/******************************************************************************
 * Speaker::Play -- Play a sound immediately.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   sound -- The sound to play.                                       *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Speaker::Play(DSound* sound)
{
	bool isPlaying = false;

	/*
	** 2022/08/20 TS:
	** sound->chl may be invalid!!!
	*/
	if (sound->chl)
		sound->chl->isPlaying(&isPlaying);

	if (!isPlaying)
	{
		m_speaker->playSound(sound->snd, nullptr, false, &(sound->chl));
		sound->chl->setVolume((float)m_soundVolume);
	}
}


/******************************************************************************
 * Speaker::PlayShared -- Play sound that shares a common channel.            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   sound                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/15 Tony : Created.                                               *
 *============================================================================*/
void Speaker::PlayShared(DSound* sound)
{
	m_speaker->playSound(sound->snd, nullptr, false, &m_channel);
	m_channel->setVolume((float)m_soundVolume);
}


/******************************************************************************
 * Speaker::PlayStream -- Play background music.                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   sound -- The sound to play.                                       *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Speaker::PlayStream(DSound* sound)
{
	m_speaker->playSound(sound->snd, nullptr, false, &(sound->chl));
	sound->chl->setVolume((float)m_musicVolume);
}


/******************************************************************************
 * Speaker::Update -- Update fmod system.                                     *
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
 *   2022/08/12 Tony : Created.                                               *
 *============================================================================*/
void Speaker::Update()
{
	if (!m_stack.empty())
		m_stack.top()->Update();
	m_speaker->update();
}


/******************************************************************************
 * Speaker::Play -- Play current play list.                                   *
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
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Speaker::Play()
{
	if (!m_stack.empty())
		m_stack.top()->Play();
}


/******************************************************************************
 * Speaker::Pause -- Pause or continue current play list.                     *
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
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Speaker::Pause(bool pause)
{
	if (!m_stack.empty())
		m_stack.top()->Pause(pause);
}


/******************************************************************************
 * Speaker::Stop -- Stop current play list.                                   *
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
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Speaker::Stop()
{
	if (!m_stack.empty())
		m_stack.top()->Stop();
}


/******************************************************************************
 * Speaker::AddPlayList -- Add play list to the stack.                        *
 *                                                                            *
 *    If a new play list is added, then the previous one will be set paused.  *
 *    and the new play list will start playing automatically.                 *
 *                                                                            *
 * INPUT:   playList                                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Speaker::AddPlayList(PlayList* playList)
{
	if (!m_stack.empty())
		m_stack.top()->Pause(true);
	m_stack.push(playList);
	Play();
}


/******************************************************************************
 * Speaker::RemovePlayList -- Remove current play list.                       *
 *                                                                            *
 *    If there are remaining play lists, then the previous one will be free   *
 *    from paused.                                                            *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Speaker::RemovePlayList()
{
	if (!m_stack.empty())
	{
		m_stack.top()->Stop();
		m_stack.pop();
	}

	if (!m_stack.empty())
		Pause(false);
}


/******************************************************************************
 * Speaker::IsPlaying -- Check if there are play list playing.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return... the thing...                                            *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool Speaker::IsPlaying() const
{
	if (m_stack.empty())
		return false;
	return m_stack.top()->IsPlaying();
}


/******************************************************************************
 * Speaker::SetMusicVolume -- Set music volume.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   volume                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/20 Tony : Created.                                               *
 *============================================================================*/
void Speaker::SetMusicVolume(double volume)
{
	m_musicVolume = volume;
	
	if (!m_stack.empty())
		m_stack.top()->SetVolume(volume);
}


/******************************************************************************
 * Speaker::Speaker -- Constructor of the object.                             *
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
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
Speaker::Speaker() :
	m_speaker(nullptr),
	m_soundVolume(1.0),
	m_musicVolume(1.0),
	m_isPaused(false),
	m_channel(nullptr)
{
}


/******************************************************************************
 * Speaker::~Speaker -- Destructor of the object.                             *
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
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
Speaker::~Speaker()
{
	_Destroy();
}


/******************************************************************************
 * Speaker::_Destroy -- Close speaker.                                        *
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
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
void Speaker::_Destroy()
{
	if (m_speaker->close() != FMOD_OK)
	{
		LOG_ERROR(CANNOT_CLOSE_SPEAKER);
		return;
	}
	if (m_speaker->release() != FMOD_OK)
	{
		LOG_ERROR(CANNOT_RELEASE_SPEAKER);
		return;
	}
}