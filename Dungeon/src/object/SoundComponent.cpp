/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : SoundComponent.cpp                        *
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

#include "../../inc/object/SoundComponent.h"

/******************************************************************************
 * SoundComponent::~SoundComponent -- Destructor of the object.               *
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
 *   2022/08/13 Tony : Created.                                               *
 *============================================================================*/
SoundComponent::~SoundComponent()
{
    if (_IsPrototype())
    {
        for (auto it = m_sounds.begin(); it != m_sounds.end(); it++)
        {
            it->second->Release();
        }
    }
}

/******************************************************************************
 * SoundComponent::Clone -- Clone sound component.                            *
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
 *   2022/08/13 Tony : Created.                                               *
 *============================================================================*/
SoundComponent* SoundComponent::Clone() const
{
    SoundComponent* clone = new SoundComponent(m_updateOrder);
    clone->_MakePrototype(false);

    AbstractComponent::Clone(clone);

    for (auto it = m_sounds.begin(); it != m_sounds.end(); it++)
    {
        clone->AddSound(it->first, it->second);
    }

    return clone;
}

/******************************************************************************
 * SoundComponent::Load -- Load sound component.                              *
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
 *   2022/08/13 Tony : Created.                                               *
 *============================================================================*/
bool SoundComponent::Load(XMLElement* node)
{
    /*
    **	<Sound>
    **		<li name="" src=""></li>
    **		<li name="" src=""></li>
    **		...
    **	</Sound>
    */
    const char* name = node->Name();

    _CHECK_TAG(StaticName());
    _RETURN_IF_ERROR();

    AbstractComponent::Load(node);

    const char* soundSrc;
    const char* soundName;
    AudioResource* sound = nullptr;
    XMLElement* li = node->FirstChildElement();
    while (li)
    {
        soundName = li->Attribute("name");
        if (!soundName)
        {
            LOG_ERROR(MISSING_ATTRIBUTE, "name", name);
            return false;
        }
        soundSrc = li->Attribute("src");
        if (!soundSrc)
        {
            LOG_ERROR(MISSING_ATTRIBUTE, "src", name);
            return false;
        }
        sound = LoadResource<AudioResource>(soundSrc);
        if (!sound) // Error will be logged in LoadResource<>
        {
            return false;
        }

        sound->Load();
        AddSound(soundName, sound);

        li = li->NextSiblingElement();
    }

    _RETURN_STATE();
}

/******************************************************************************
 * SoundComponent::AddSound -- Add sound to the component.                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name                                                              *
 *          sound                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  Just no extra check, ok?                                        *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/13 Tony : Created.                                               *
 *============================================================================*/
void SoundComponent::AddSound(const std::string& name, AudioResource* sound)
{
    auto it = m_sounds.find(name);

    if (it == m_sounds.end())
    {
        m_sounds.emplace(name, sound);
    }
}

/******************************************************************************
 * SoundComponent::Play -- Play certain sound.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name -- name of the sound.                                        *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/13 Tony : Created.                                               *
 *============================================================================*/
void SoundComponent::Play(const std::string& name)
{
    auto it = m_sounds.find(name);

    if (it != m_sounds.end())
    {
        it->second->GetResource()->Play();
    }
    else
    {
        LOG_ERROR(R"(Missing sound "%s")", name.c_str());
    }
}
