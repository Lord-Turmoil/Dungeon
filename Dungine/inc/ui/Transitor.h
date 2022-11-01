/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Transitor.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 17, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Transitor handle the transit between two interfaces.                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _TRANSITOR_H_
#define _TRANSITOR_H_

#include "../common/Common.h"
#include "../device/Symbol.h"


class AbstractInterface;

/********************************************************************
** For transit between two interfaces.
** This will continue timer, but will not receive events while working.
*/
class Transitor
{
public:
	Transitor() {}
	~Transitor() {}

	static void Initialize();
	static void FadeIn(AbstractInterface* intf, clock_t duration = DEFAULT_TRANSIT_DURATION);
	static void FadeOut(AbstractInterface* intf, clock_t duration = DEFAULT_TRANSIT_DURATION);

private:
	static void _Fade(AbstractInterface* intf, clock_t duration, int begin, int end);

	static Symbol m_symbol;
};

#endif