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

#include "../common/Common.h"
#include "../template/Singleton.h"


/********************************************************************
** The global timer of the game. So singleton is used.
*/
class Timer : public SingletonHungry<Timer>
{
	friend class SingletonHungry<Timer>;
public:
	void Update();

	double GetDeltaTime() const { return m_deltaTime; }
	clock_t GetDeltaTimestamp() const { return m_deltaTimestamp; }

	double GetTime() const { return m_time; }
	clock_t GetTimestamp() const { return m_timestamp; }
	
	double GetFPS() const { return m_fps; }

	void Delay(int milliseconds = DUNGINE_DELAY);

	void Pause();
	void Continue();

private:
	Timer();
	~Timer() {}

	// unit is second
	double m_time;
	double m_deltaTime;

	// unit is millisecond
	clock_t m_timestamp;
	clock_t m_deltaTimestamp;

	LARGE_INTEGER m_tickPerSecond;
	double m_rTickPerSecond;
	LARGE_INTEGER m_curTick;
	LARGE_INTEGER m_lastTick;
	LARGE_INTEGER m_startTick;
	LARGE_INTEGER m_deltaTick;

	// for delay
	LARGE_INTEGER m_clock;
	LONGLONG m_oldClock;
	int m_frequency;

	// for fps
	double m_fps;

	bool m_isPaused;
};