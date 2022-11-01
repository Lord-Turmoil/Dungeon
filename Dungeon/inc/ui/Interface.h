/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Interface.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 21, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For all game interfaces.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _INTERFACE_H_
#define _INTERFACE_H_


#include <dungine.h>


/********************************************************************
** All interfaces are:
**   Logo                         -- Time
**   Main                         -- Plain
**   Setting, In-Game Setting     -- Plain
**   Select [Hero]                -- Plain
**   Game                         -- Game
**   LevelTransit, ChapterTransit -- Transit <- Time
**   Pause                        -- Plain
**   Quit                         -- Plain
**   Store                        -- (On hold)
**   Version & Copyright          -- Plain
**   Credits                      -- Time
**   About                        -- Plain
**   Help                         -- Plain
*/
const char* const UI_XML[] = {
	"Logo.xml",
	"Main.xml",
	"Setting.xml",
	"Help.xml",
	"Version.xml",
	"About.xml",
	"Credits.xml",
	"Game.xml",
	"Victory.xml" };

/********************************************************************
** Plain interface has nothing special. It is also the base interface
** for other advanced interfaces.
*/
class PlainInterface : public AbstractInterface
{
public:
	PlainInterface() : m_inTime(0L), m_outTime(0L) {}
	virtual ~PlainInterface() {}

	/*
	** These remain no change.
	** virtual void Launch();
	** virtual void Attach();
	** virtual void Terminate();
	** virtual void Detatch();
	*/

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();

	/*
	** These remain no change.
	** virtual void Draw();
	** virtual void Draw(IMAGE* pDestImage);
	*/

	/*
	** This will hook all event handler to the widgets.
	*/
	virtual void AddEvents() {}
	
protected:
	virtual void _ProcessInput(Event& evnt);

	virtual void _TransitIn();
	virtual void _TransitOut();

	virtual void _Initialize() {}
	virtual void _Destroy() {}

	clock_t m_inTime;
	clock_t m_outTime;
};


/********************************************************************
** Time interface will terminate after certain time.
*/
class TimeInterface : public PlainInterface
{
public:
	TimeInterface() : m_duration(0L), m_elapsedTime(0L) {}
	virtual ~TimeInterface() {}

	virtual bool Load(XMLElement* node);

	// Self-terminate in update when time's up.
	virtual void Update(Event* evnt);

protected:
	virtual void _Initialize();

	virtual void _SelfTerminate();

	clock_t m_duration;
	clock_t m_elapsedTime;
};


extern AbstractInterface* LoadInterface(XMLElement* node);

#endif