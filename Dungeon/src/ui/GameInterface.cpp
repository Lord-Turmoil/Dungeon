/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GameInterface.cpp                         *
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

#include "../../inc/common/Defines.h"

#include "../../inc/ui/Cursor.h"
#include "../../inc/ui/GameInterface.h"

#include "../../inc/object/Boss.h"
#include "../../inc/object/Hero.h"
#include "../../inc/object/Weapon.h"

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Settings.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** GameInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool GameInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Game");
    _RETURN_IF_ERROR();

    PlainInterface::Load(node);

    _RETURN_STATE();
}

/*
** 2022/11/25 TS:
** Added prompt for weapon name and cost.
*/
void GameInterface::Update(Event* evnt)
{
    if (m_pSubIntf)
    {
        m_pSubIntf->Update(evnt);
        return;
    }

    if (evnt && !evnt->IsFocus())
    {
        _OnPause();
        return;
    }

    if (m_pDungeon)
    {
        // Hero
        Hero* hero = m_pDungeon->GetHero();

        _UpdateStatus(static_cast<ProgressBar*>(m_pWidgetManager->GetWidget("hp")),
                      static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("hp-tip")),
                      static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("hp-alert")), hero->GetHP(),
                      hero->GetMaxHP());
        _UpdateStatus(static_cast<ProgressBar*>(m_pWidgetManager->GetWidget("mp")),
                      static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("mp-tip")),
                      static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("mp-alert")), hero->GetMP(),
                      hero->GetMaxMP());
        _UpdateStatus(static_cast<ProgressBar*>(m_pWidgetManager->GetWidget("armor")),
                      static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("armor-tip")),
                      static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("armor-alert")), hero->GetArmor(),
                      hero->GetMaxArmor());
        _UpdateChiStatus(static_cast<ProgressBar*>(m_pWidgetManager->GetWidget("chi")),
                         static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("chi-tip")), hero->GetChi(),
                         hero->GetMaxChi());

        Weapon* weapon = hero->GetWeapon();
        if (weapon)
        {
            _UpdateWeapon(static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("weapon-name")),
                          static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("weapon-cost")),
                          hero->GetWeapon()->Name().c_str(), hero->GetWeapon()->GetCost());
        }
        else
        {
            _UpdateWeapon(static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("weapon-name")),
                          static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("weapon-cost")), nullptr, 0);
        }

        // Boss
        Boss* boss = m_pDungeon->GetBoss();
        if (boss)
        {
            auto board = static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("boss-board"));
            board->Activate(true);

            auto bar = static_cast<ProgressBar*>(m_pWidgetManager->GetWidget("boss-hp"));
            bar->Activate(true);
            _UpdateStatus(bar, boss->GetHPPercent());
            bar = static_cast<ProgressBar*>(m_pWidgetManager->GetWidget("boss-armor"));
            bar->Activate(true);
            _UpdateStatus(bar, boss->GetArmorPercent());
        }
        else
        {
            static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("boss-board"))->Activate(false);
            static_cast<ProgressBar*>(m_pWidgetManager->GetWidget("boss-hp"))->Activate(false);
            static_cast<ProgressBar*>(m_pWidgetManager->GetWidget("boss-armor"))->Activate(false);
        }

        m_pDungeon->Update();
    }

    char buffer[16];
    sprintf_s(buffer, "%d", Settings::GetInstance()->Coin());
    static_cast<TextDrawer*>(static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("coin-tip"))->GetDrawer())
        ->SetText(buffer);

    m_pWidgetManager->Update(evnt);

    if (evnt)
    {
        _ProcessInput(*evnt);
    }
}

void GameInterface::Draw()
{
    m_pWidgetManager->Draw();
    m_pDungeon->Draw();
    if (m_pSubIntf)
    {
        m_pSubIntf->Draw();
    }
}

void GameInterface::Draw(IMAGE* pDestImage)
{
    m_pWidgetManager->Draw();
    m_pDungeon->Draw();
    if (m_pSubIntf)
    {
        m_pSubIntf->Draw();
    }
    Device::GetInstance()->Render(pDestImage);
}

void GameInterface::AddEvents()
{
    static_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("quit"))
        ->OnTriggered(std::bind(&GameInterface::_OnQuit, this));
    static_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("pause"))
        ->OnTriggered(std::bind(&GameInterface::_OnPause, this));
}

/********************************************************************
** -1 is the invalid id, if id is -1, it is to stop playing anything.
*/
void GameInterface::PlayTrack(int id)
{
    std::string head("track");
    std::string track;

    if (m_curTrack != -1)
    {
        track = head + std::to_string(m_curTrack);
        static_cast<AudioPlayer*>(m_pWidgetManager->GetWidget(track))->Stop();
    }

    if (id != -1)
    {
        track = head + std::to_string(id);
        static_cast<AudioPlayer*>(m_pWidgetManager->GetWidget(track))->Play();
    }

    m_curTrack = id;
}

void GameInterface::_Initialize()
{
    static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("b-level"))->ResetTransition()->StopTransition();
    static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("b-complete"))->ResetTransition()->StopTransition();
    static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("b-final"))->ResetTransition()->StopTransition();

    m_pDungeon = new Dungeon();
    m_pDungeon->SetGameInterface(this);

    if (!m_pDungeon->Initialize())
    {
        LOG_ERROR("Failed to initialize the game");
    }

    /*
    ** 2022/08/20 TS:
    ** The process of loading may take long enough to
    ** affect the timing, so... just patch it up. :(
    */
    Timer::GetInstance()->Update();

    Update(nullptr);

    SetCursorStyle(CUR_ATTACK);
}

void GameInterface::_Destroy()
{
    delete m_pDungeon;
    m_pDungeon = nullptr;

    SetCursorStyle(CUR_ARROW);

    PlayTrack(-1);
}

void GameInterface::_UpdateStatus(ProgressBar* bar, double percent)
{
    bar->SetValue(percent);
}

void GameInterface::_UpdateStatus(ProgressBar* bar, StaticWidget* text, StaticWidget* alert, int curVal, int maxVal)
{
    char buffer[16];

    sprintf_s(buffer, "%d/%d", curVal, maxVal);
    bar->SetValue(static_cast<double>(curVal) / static_cast<double>(maxVal));
    static_cast<TextDrawer*>(text->GetDrawer())->SetText(buffer);

    if (alert)
    {
        if (curVal == 0)
        {
            alert->Activate(true);
        }
        else
        {
            alert->Activate(false);
        }
    }
}

void GameInterface::_UpdateChiStatus(ProgressBar* bar, StaticWidget* text, int curVal, int maxVal)
{
    bar->SetValue(static_cast<double>(curVal) / static_cast<double>(maxVal));
    text->Activate(curVal == maxVal);
}

/*
** 2022/11/25 TS:
** Added prompt for weapon name and cost.
*/
void GameInterface::_UpdateWeapon(StaticWidget* nameText, StaticWidget* costText, const char* name, int cost)
{
    if (!name)
    {
        static_cast<TextDrawer*>(nameText->GetDrawer())->SetText("");
        static_cast<TextDrawer*>(costText->GetDrawer())->SetText("");
        return;
    }

    TextDrawer* nameDrawer = static_cast<TextDrawer*>(nameText->GetDrawer());
    if (nameDrawer->GetText() == name) // no need to update
    {
        return;
    }
    nameDrawer->SetText(name);

    char buffer[16];
    sprintf_s(buffer, "%d", cost);
    static_cast<TextDrawer*>(costText->GetDrawer())->SetText(buffer);
}

void GameInterface::_OnPause()
{
    SetCursorStyle(CUR_ARROW);
    SetSubInterface(Application::GetInstance()->GetInterface("Pause"));
}

void GameInterface::_OnQuit()
{
    SetCursorStyle(CUR_ARROW);
    SetSubInterface(Application::GetInstance()->GetInterface("Quit"));
}
