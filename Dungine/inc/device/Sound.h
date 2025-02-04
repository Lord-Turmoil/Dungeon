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
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
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

#ifndef _SOUND_H_
#define _SOUND_H_

#include <vector>

#include "../utility/fmodcore.h"
#include "../utility/tinyxml.h"

/********************************************************************
** The actual sound object. 'D' means Dungine.
*/
struct DSound
{
    FMOD::Sound* snd;   // sound
    FMOD::Channel* chl; // channel

    DSound() : snd(nullptr), chl(nullptr)
    {
    }

    ~DSound()
    {
        if (snd)
        {
            snd->release();
        }
    }
};

/********************************************************************
** There are two types of sounds, one is in-game sound effect, which
** only play once and is short. The other is BGM, which play all the
** time or play as a list.
*/
class AbstractSound
{
public:
    AbstractSound() : m_isPaused(false)
    {
    }

    virtual ~AbstractSound()
    {
    }

    virtual bool Load(XMLElement* node);

    virtual void Update() = 0;

    virtual void Play() = 0;
    virtual void Pause(bool isPaused) = 0;
    virtual void Stop() = 0;

protected:
    bool m_isPaused;
};

/********************************************************************
** All sounds that aren't background music which will play a long
** are mono-sound, and will only play once immediately after Play()
** is called.
*/
class MonoSound : public AbstractSound
{
public:
    MonoSound() : m_pSound(nullptr), m_isShared(false)
    {
    }

    ~MonoSound() override;

    bool Load(XMLElement* node) override;

    void Update() override;

    void Play() override;
    void Pause(bool isPaused) override;
    void Stop() override;

private:
    DSound* m_pSound;
    bool m_isShared; // Whether share channel with other sounds.
};

/********************************************************************
** Background music should all be contained in playlists. Even there
** is only one playlist.
*/

class PlayList : public AbstractSound
{
public:
    PlayList();
    ~PlayList() override;

    bool Load(XMLElement* node) override;

    void Update() override;

    void Play() override;
    void Pause(bool isPaused) override;
    void Stop() override;

    void SetVolume(double volume);

public:
    bool IsPlaying() const;

    bool IsLoop() const
    {
        return m_isLoop;
    }

    void Loop(bool isLoop)
    {
        m_isLoop = isLoop;
    }

    bool IsShuffle() const
    {
        return m_isShuffle;
    }

    void Shuffle(bool isShuffle)
    {
        m_isShuffle = isShuffle;
    }

private:
    void _Release();
    void _PlayNext();
    void _Shuffle();

    std::vector<DSound*> m_queue;
    std::vector<DSound*>::iterator m_current;

    bool m_isLoop;
    bool m_isShuffle;
};

DSound* LoadSound(const char* filename);
DSound* LoadStream(const char* filename);

#endif
