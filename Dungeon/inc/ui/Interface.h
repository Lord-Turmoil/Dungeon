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
 *                    Last Update : November 22, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
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
const char* const UI_XML[] = { "Logo.xml",  "Main.xml",    "Setting.xml", "Help.xml",   "Version.xml",
                               "About.xml", "Credits.xml", "Game.xml",    "Victory.xml" };

/********************************************************************
** Plain interface has nothing special. It is also the base interface
** for other advanced interfaces.
*/
class PlainInterface : public AbstractInterface
{
public:
    PlainInterface() : m_inTime(0L), m_outTime(0L)
    {
    }

    ~PlainInterface() override
    {
    }

    /*
     ** These remain no change.
     ** virtual void Launch();
     ** virtual void Attach();
     ** virtual void Terminate();
     ** virtual void Detatch();
     */

    bool Load(XMLElement* node) override;

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;

    /*
    ** These remain no change.
    ** virtual void Draw();
    ** virtual void Draw(IMAGE* pDestImage);
    */

    /*
    ** This will hook all event handler to the widgets.
    */
    virtual void AddEvents()
    {
    }

    /*
    ** 2022/11/22 TS:
    ** Added control over in and out time.
    */
    void SetInTime(clock_t inTime)
    {
        m_inTime = inTime;
    }

    void SetOutTime(clock_t outTime)
    {
        m_outTime = outTime;
    }

    clock_t GetInTime()
    {
        return m_inTime;
    }

    clock_t GetOutTime()
    {
        return m_outTime;
    }

protected:
    virtual void _ProcessInput(Event& evnt);

    void _TransitIn() override;
    void _TransitOut() override;

    void _Initialize() override
    {
    }

    void _Destroy() override
    {
    }

    clock_t m_inTime;
    clock_t m_outTime;
};

/********************************************************************
** Time interface will terminate after certain time.
*/
class TimeInterface : public PlainInterface
{
public:
    TimeInterface() : m_duration(0L), m_elapsedTime(0L)
    {
    }

    ~TimeInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    // Self-terminate in update when time's up.
    void Update(Event* evnt) override;

protected:
    void _Initialize() override;

    virtual void _SelfTerminate();

    clock_t m_duration;
    clock_t m_elapsedTime;
};

extern AbstractInterface* LoadInterface(XMLElement* node);

#endif
