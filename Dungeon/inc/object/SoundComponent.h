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
 * Overview:                                                                 *
 *   Sound component.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _SOUND_COMPONENT_H_
#define _SOUND_COMPONENT_H_

#include <dungine.h>

class SoundComponent : public AbstractComponent
{
public:
    SoundComponent(int updateOrder = 0) : AbstractComponent(updateOrder)
    {
    }

    ~SoundComponent() override;

    static const char* StaticName()
    {
        return "Sound";
    }

    const char* Name() override
    {
        return StaticName();
    }

    SoundComponent* Clone() const override;

    virtual void Clone(SoundComponent* clone) const
    {
    }

    bool Load(XMLElement* node) override;

    void Update(Event* evnt) override
    {
    }

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
