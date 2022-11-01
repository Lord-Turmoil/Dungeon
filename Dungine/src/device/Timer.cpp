/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Timer.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 19, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For global timing.                                                       *
 * -------------------------------------------------------------------------- *
 * Reference:                                                                 *
 *   https://codebus.cn/sunxiaoyu/character-rain                              *
 *   https://codebus.cn/yangw/accurate-delay                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Macros.h"
#include "../../inc/device/Timer.h"


/******************************************************************************
 * Timer::Update -- Update timer.                                             *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void Timer::Update()
{
	if (m_isPaused)
		return;

	m_lastTick = m_curTick;
	QueryPerformanceCounter(&m_curTick);
	m_time = (double)(m_curTick.QuadPart - m_startTick.QuadPart) * m_rTickPerSecond;
	m_deltaTime = (double)(m_curTick.QuadPart - m_lastTick.QuadPart) * m_rTickPerSecond;

	m_deltaTimestamp = (clock_t)(m_deltaTime * 1000.0);
	m_timestamp = m_deltaTimestamp;

#ifndef NO_FPS
	static double nextFpsTime = 0.0;
	if (m_time > nextFpsTime)
	{
		nextFpsTime += 1.0;
		m_fps = 1.0 / m_deltaTime;
	}
#endif
}


/******************************************************************************
 * Timer::Delay -- Absolute dealy.                                            *
 *                                                                            *
 *    If HIGH_PRECISION_DELAY is not enabled, the minimum delay is ablout 18  *
 *    milliseconds and can keep a fps about 70.                               *
 *                                                                            *
 * INPUT:   milliseconds -- Time to delay.                                    *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void Timer::Delay(int milliseconds)
{
	LONGLONG elapse = milliseconds * m_frequency;

	m_oldClock += elapse;

	QueryPerformanceCounter(&m_clock);

	if (m_clock.QuadPart > m_oldClock)
		m_oldClock = m_clock.QuadPart;
	else
	{
		do
		{
#ifndef HIGH_PRECISION_DELAY
			Sleep(1);
#endif
			// Get ending clock
			QueryPerformanceCounter(&m_clock);
		} while (m_clock.QuadPart < m_oldClock);
	}
}


/******************************************************************************
 * Timer::Pause -- Pause the timer.                                           *
 *                                                                            *
 *    This will pause the time. m_time will jump over this period and won't be*
 *    affected after continued. However, m_timestamp will lose this period    *
 *    and only record how many timestamp passed.                              *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void Timer::Pause()
{
	// Does it make any sense?
}


/******************************************************************************
 * Timer::Continue -- Continue the timer.                                     *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void Timer::Continue()
{
	// Does it make any sense?
}


/******************************************************************************
 * Timer::Timer -- Constructor of the object.                                 *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Timer::Timer()
{
	m_isPaused = false;

	QueryPerformanceFrequency(&m_tickPerSecond);
	QueryPerformanceCounter(&m_startTick);
	m_curTick.QuadPart = m_startTick.QuadPart;
	m_lastTick.QuadPart = m_startTick.QuadPart - 1;
	m_rTickPerSecond = 1.0 / (double)m_tickPerSecond.QuadPart;

	// QueryPerformanceFrequency(&m_clock);
	// m_frequency = (int)m_clock.QuadPart / 1000;
	// QueryPerformanceCounter(&m_clock);
	// m_oldClock = m_clock.QuadPart;
	m_frequency = (int)(m_tickPerSecond.QuadPart / 1000);
	m_oldClock = m_startTick.QuadPart;

	m_timestamp = 0;

	Update();
}