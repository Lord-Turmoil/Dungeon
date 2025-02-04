/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : MenuInterface.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 30, 2022                             *
 *                                                                            *
 *                    Last Update : December 13, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Menu interfaces.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _MENU_INTERFACE_H_
#define _MENU_INTERFACE_H_

#include "Interface.h"

class LogoInterface final : public TimeInterface
{
public:
    LogoInterface()
    {
    }

    ~LogoInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;

protected:
    void _SelfTerminate() override;

private:
    static void _OnSkip();
};

class MainInterface final : public PlainInterface
{
public:
    MainInterface()
    {
    }

    ~MainInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;

protected:
    void _Initialize() override;
    void _Destroy() override;

    static void _OnPlay();
    static void _OnSetting();
    static void _OnHelp();
    static void _OnAbout();
    static void _OnVersion();
};

class SettingInterface final : public PlainInterface
{
public:
    SettingInterface()
    {
    }

    ~SettingInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;

protected:
    void _Initialize() override;
    void _Destroy() override;

private:
    void _SaveSettings();
    void _LoadSettings();
    void _SetDifficulty(int difficulty);
    void _SetFullscreen(bool fullscreen);

    void _OnDifficultyIncrease();
    void _OnDifficultyDecrease();
    void _OnFullscreenChange();
    static void _OnMusicVolume(double volume);
    static void _OnSoundVolume(double volume);
};

class SelectInterface final : public PlainInterface
{
public:
    SelectInterface()
    {
    }

    ~SelectInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;

protected:
    void _Initialize() override;

private:
    void _ResetState();
    void _InitWidget();

    void _OnPurchaseBlackHand();

    static void _OnYony(bool isSelected);
    static void _OnSpecter(bool isSelected);
    static void _OnBlackHand(bool isSelected);
    static void _OnChangeMode(bool isSelected);
    static void _OnStart();

    static bool m_selected;
};

class HelpInterface final : public PlainInterface
{
public:
    HelpInterface()
    {
    }

    ~HelpInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;
};

// Pause interface as a pop up.
class PauseInterface final : public PlainInterface
{
public:
    PauseInterface()
    {
    }

    ~PauseInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;

private:
    void _OnResume();
    static void _OnSetting();
};

class QuitInterface final : public PlainInterface
{
public:
    QuitInterface()
    {
    }

    ~QuitInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;

private:
    void _OnConfirm();
    void _OnCancel();
};

class AboutInterface final : public PlainInterface
{
public:
    AboutInterface()
    {
    }

    ~AboutInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;

private:
    static void _OnOpenUrl(std::string url);
    static bool _LoadLink(XMLElement* root, const char* link, std::string& url);

    static std::string m_studioUrl; // studio url
    static std::string m_githubUrl; // github url
};

/*
** 2022/11/25 TS:
** Make Victory and Lost interface independent.
*/
class VictoryInterface final : public TimeInterface
{
public:
    VictoryInterface() : m_isVictory(false)
    {
    }

    ~VictoryInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;

private:
    /*
     ** 2022/11/27 TS:
     ** If it is the first time to complete the game,
     ** it will jump to credits interface.
     */
    void _SelfTerminate() override;

    /*
    ** 2022/11/27 TS:
    ** Since both victory and lost interface use this class,
    ** a flag is needed to tell them apart.
    */
    bool m_isVictory;
};

#endif
