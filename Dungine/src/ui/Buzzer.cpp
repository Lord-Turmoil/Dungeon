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
 * Overview:                                                                 *
 *   Widget can use buzzer to make sound.                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Common.h"

#include "../../inc/device/Explorer.h"
#include "../../inc/device/Sound.h"

#include "../../inc/ui/Buzzer.h"

#include "../../inc/utility/Parser.h"

/******************************************************************************
 * Buzzer::Load -- Load buzzer from explorer.                                 *
 *                                                                            *
 *    Sound resource already loaded in explorer, so here just get the sound   *
 *    from explorer.                                                          *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
bool Buzzer::Load(XMLElement* node)
{
    /*
    **	<Buzzer src=""></Buzzer>
    */
    const char* name = node->Name();
    const char* attr;

    _CHECK_TAG("Buzzer");
    _RETURN_IF_ERROR();

    attr = node->Attribute("src");
    if (!attr)
    {
        LOG_ERROR(MISSING_ATTRIBUTE, "src", name);
        return false;
    }

    AudioResource* res = LoadResource<AudioResource>(attr);
    if (!res)
    {
        LOG_ERROR(INVALID_RESOURCE_ID, attr);
        return false;
    }
    m_pSound = dynamic_cast<MonoSound*>(res->GetResource());
    if (!m_pSound)
    {
        LOG_ERROR(RESOURCE_MISMATCH, attr);
        return false;
    }

    return true;
}

/******************************************************************************
 * Buzzer::Play -- Play the sound.                                            *
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
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void Buzzer::Play()
{
    if (m_pSound)
    {
        m_pSound->Play();
    }
}

/******************************************************************************
 * LoadBuzzer -- Load a buzzer.                                               *
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
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
Buzzer* LoadBuzzer(XMLElement* node)
{
    if (!node)
    {
        return nullptr;
    }

    Buzzer* buzzer = new Buzzer();

    if (!buzzer->Load(node))
    {
        delete buzzer;
        return nullptr;
    }

    return buzzer;
}
