/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GameInterface.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 31, 2022                             *
 *                                                                            *
 *                    Last Update : November 25, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   In game interface.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _GAME_INTERFACE_H_
#define _GAME_INTERFACE_H_

#include "Interface.h"

class Dungeon;

class GameInterface final : public PlainInterface
{
public:
    GameInterface() : m_pDungeon(nullptr), m_curTrack(-1)
    {
    }

    ~GameInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void Update(Event* evnt) override;
    void Draw() override;
    void Draw(IMAGE* pDestImage) override;

    void AddEvents() override;

    void PlayTrack(int id);

protected:
    void _Initialize() override;
    void _Destroy() override;

private:
    void _UpdateStatus(ProgressBar* bar, double percent);
    void _UpdateStatus(ProgressBar* bar, StaticWidget* text, StaticWidget* alert, int curVal, int maxVal);
    void _UpdateChiStatus(ProgressBar* bar, StaticWidget* text, int curVal, int maxVal);

    /*
    ** Added prompt for weapon name and cost.
    */
    void _UpdateWeapon(StaticWidget* nameText, StaticWidget* costText, const char* name, int cost);

    void _OnPause();
    void _OnQuit();

    Dungeon* m_pDungeon;
    int m_curTrack;
};

#endif
