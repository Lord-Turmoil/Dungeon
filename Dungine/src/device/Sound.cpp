/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Sound.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 10, 2022                             *
 *                                                                            *
 *                    Last Update : December 17, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide basic sound types.                                               *
 * -------------------------------------------------------------------------- *
 * Reference:                                                                 *
 *   https://www.fmod.com/                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <algorithm>	// random_shuffle

#include "../../inc/common/Common.h"

#include "../../inc/device/Sound.h"
#include "../../inc/device/Speaker.h"

#include "../../inc/utility/Parser.h"


/******************************************************************************
 * AbstractSound                                                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
bool AbstractSound::Load(XMLElement* node)
{
/*
**	<resource>
**		<...></...>
**	</resource>
*/
	return true;
}



/******************************************************************************
 * MonoSound::~MonoSound -- Destructor of the object.                         *
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
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
MonoSound::~MonoSound()
{
	_DELETE(m_pSound);
}


/******************************************************************************
 * MonoSound::Load -- Load the sound into the memory.                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
bool MonoSound::Load(XMLElement* node)
{
/*
**	<Mono src=""></Mono>
*/
	if (m_pSound)
		return true;

	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("Mono");
	_RETURN_IF_ERROR();

	_PARSE("share", m_isShared, name, false);

	attr = node->Attribute("src");
	if (!attr)
	{
		LOG_ERROR(MISSING_ATTRIBUTE, "src", name);
		return false;
	}

	m_pSound = LoadSound(attr);
	if (!m_pSound)
	{
		LOG_ERROR(INVALID_RESOURCE, attr);
		return false;
	}

	return true;
}


/******************************************************************************
 * MonoSound::Update -- Update mono sound.                                    *
 *                                                                            *
 *    Monosound has nothing to update.                                        *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void MonoSound::Update()
{
}


/******************************************************************************
 * MonoSound::Play -- Play the sound once immediately.                        *
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
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void MonoSound::Play()
{
	if (m_isShared)
		Speaker::GetInstance()->PlayShared(m_pSound);
	else
		Speaker::GetInstance()->Play(m_pSound);
}


/******************************************************************************
 * MonoSound::Pause -- Pause the sound.                                       *
 *                                                                            *
 *    Hmm... Mono sound cannot be paused, I think?                            *
 *                                                                            *
 * INPUT:   isPaused                                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void MonoSound::Pause(bool isPaused)
{
}


/******************************************************************************
 * MonoSound::Stop -- ...                                                     *
 *                                                                            *
 *    Cannot be stopped.eaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void MonoSound::Stop()
{
}



/******************************************************************************
 * PlayList::PlayList -- Constructor of the object.                           *
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
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
PlayList::PlayList() : m_isLoop(true), m_isShuffle(false)
{
}


/******************************************************************************
 * PlayList::~PlayList -- Destructor of the object.                           *
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
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
PlayList::~PlayList()
{
	_Release();
}


/******************************************************************************
 * PlayList::Load -- Load play list.                                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
bool PlayList::Load(XMLElement* node)
{
	/*
	**	<PlayList loop="" shuffle="">
	**		<li src=""></li>
	**		<li src=""></li>
	**		...
	**	</PlayList>
	*/
	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("PlayList");
	_RETURN_IF_ERROR();

	_PARSE("loop", m_isLoop, name, true);
	_PARSE("shuffle", m_isShuffle, name, false);

	XMLElement* li = node->FirstChildElement();
	DSound* sound;

	while (li)
	{
		attr = li->Attribute("src");
		if (!attr)
		{
			LOG_ERROR(MISSING_ATTRIBUTE, "src", li->Name());
			return false;
		}
		sound = LoadStream(attr);
		if (!sound)
		{
			LOG_ERROR(INVALID_RESOURCE, attr);
			return false;
		}
		m_queue.push_back(sound);

		li = li->NextSiblingElement();
	}

	m_current = m_queue.begin();

	return true;
}

			
/******************************************************************************
 * PlayList::Update -- Update play list.                                      *
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
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void PlayList::Update()
{
	if (m_current == m_queue.end())
		return;

	if (!IsPlaying())
		_PlayNext();
}


/******************************************************************************
 * PlayList::Play -- Start to play the list.                                  *
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
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void PlayList::Play()
{
	m_current = m_queue.begin();
	if (m_current == m_queue.end())	// empty list
		return;

	if (m_isShuffle)
		_Shuffle();

	Speaker::GetInstance()->PlayStream(*m_current);
}


/******************************************************************************
 * PlayList::Pause -- Pause and continue the play list.                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   isPaused                                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void PlayList::Pause(bool isPaused)
{
	if (m_current == m_queue.end())
		return;

	if (m_isPaused != isPaused)
	{
		m_isPaused = isPaused;
		(*m_current)->chl->setPaused(m_isPaused);
		if (!m_isPaused)
			(*m_current)->chl->setVolume((float)Speaker::GetInstance()->GetMusicVolume());
	}
}


/******************************************************************************
 * PlayList::Stop -- Stop the play list.                                      *
 *                                                                            *
 *    If stopped, must play again to continue.                                *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void PlayList::Stop()
{
	if (m_current != m_queue.end())
	{
		(*m_current)->chl->stop();
		m_current = m_queue.begin();
	}
}

/******************************************************************************
 * PlayList::SetVolume -- Set volume of play list.                            *
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
void PlayList::SetVolume(double volume)
{
	if (IsPlaying())
	{
		(*m_current)->chl->setVolume((float)volume);
	}
}


/******************************************************************************
 * PlayList::IsPlaying -- If the play list is still playing or not.           *
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
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
bool PlayList::IsPlaying() const
{
	if (m_current == m_queue.end())
		return false;

	bool isPlaying;
	(*m_current)->chl->isPlaying(&isPlaying);

	return isPlaying;
}


/******************************************************************************
 * PlayList::_Release -- Release all sounds.                                  *
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
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void PlayList::_Release()
{
	for (auto it = m_queue.begin(); it != m_queue.end(); it++)
		delete (*it);
}


/******************************************************************************
 * PlayList::_PlayNext -- Play next one in the list.                          *
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
 *   2022/06/15 Tony : Created.                                               *
 *   2022/12/17 Tony : Bug fixed.                                             *
 *============================================================================*/
void PlayList::_PlayNext()
{
	if (m_current == m_queue.end())
		return;

	m_current++;
	if (m_current == m_queue.end())
	{
		if (m_isLoop)
			Play();
	}
	else
	{
		/*
		** 2022/12/17 TS:
		** This should not be reached if previous if is true.
		*/
		Speaker::GetInstance()->Play(*m_current);
	}
}


/******************************************************************************
 * PlayList::_Shuffle -- Shuffle play list.                                   *
 *                                                                            *
 *    Similar to Windows Media Player, the random play just shuffle the play  *
 *    queue.                                                                  *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
void PlayList::_Shuffle()
{
	if (m_queue.begin() == m_queue.end())
		return;

	std::random_shuffle(m_queue.begin(), m_queue.end());
}


/******************************************************************************
 * LoadSound -- Load sound from memory.                                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   filename                                                          *
 *                                                                            *
 * OUTPUT:  Return loaded sound. nullptr if failed.                           *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
DSound* LoadSound(const char* filename)
{
	DSound* stream = new DSound();

	if (Speaker::GetInstance()->System()->createSound(filename, FMOD_DEFAULT, nullptr, &(stream->snd)) != FMOD_OK)
	{
		delete stream;
		return nullptr;
	}

	return stream;
}


/******************************************************************************
 * LoadStream -- Load stream from memory.                                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   filename                                                          *
 *                                                                            *
 * OUTPUT:  Return loaded sound. nullptr if failed.                           *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
DSound* LoadStream(const char* filename)
{
	DSound* stream = new DSound();

	if (Speaker::GetInstance()->System()->createStream(filename, FMOD_DEFAULT, nullptr, &(stream->snd)) != FMOD_OK)
	{
		delete stream;
		return nullptr;
	}

	return stream;
}