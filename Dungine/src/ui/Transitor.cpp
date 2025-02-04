/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Transitor.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 17, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Transitor handle the transit between two interfaces.                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/device/Device.h"
#include "../../inc/device/Timer.h"

#include "../../inc/ui/AbstractInterface.h"
#include "../../inc/ui/Transitor.h"

#include "../../inc/utility/Auxilliary.h"

Symbol Transitor::m_symbol;

/******************************************************************************
 * Transitor::Initialize -- Initialize transitor.                             *
 *                                                                            *
 *    Since... Just... this...                                                *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void Transitor::Initialize()
{
    m_symbol.GetImage()->Resize(deviceInfo.clientWidth, deviceInfo.clientHeight);
}

/******************************************************************************
 * Transitor::FadeIn -- From dark to light.                                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   intf     -- The interface to transit.                             *
 *          duration -- Duration of the transit.                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void Transitor::FadeIn(AbstractInterface* intf, clock_t duration)
{
    _Fade(intf, duration, 0, 255);
}

/******************************************************************************
 * Transitor::FadeOut -- From light to dark.                                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   intf     -- The interface to transit.                             *
 *          duration -- Duration of the transit.                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void Transitor::FadeOut(AbstractInterface* intf, clock_t duration)
{
    _Fade(intf, duration, 255, 0);
}

/******************************************************************************
 * Transitor::_Fade -- Perform fade.                                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   intf     -- The interface to transit.                             *
 *          duration -- Duration of the transit.                              *
 *          start    -- Start alpha.                                          *
 *          end      -- End alpha.                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/15 Tony : Created.                                               *
 *============================================================================*/
void Transitor::_Fade(AbstractInterface* intf, clock_t duration, int start, int end)
{
    clock_t elapsedTime = 0;
    Device* device = Device::GetInstance();
    Timer* timer = Timer::GetInstance();

    device->Abandon();
    intf->Draw(m_symbol.GetImage());
    while (elapsedTime < duration)
    {
        timer->Update();

        elapsedTime += timer->GetDeltaTimestamp();
        if (elapsedTime > duration)
        {
            elapsedTime = duration;
        }

        m_symbol.GetAttribute()->alpha =
            BlendValue(start, end, static_cast<double>(elapsedTime) / static_cast<double>(duration));

        device->AddSymbol(&m_symbol);
        device->Render();

        timer->Delay();
    }
}
