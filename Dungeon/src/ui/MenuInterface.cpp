/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : MenuInterface.cpp                         *
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

#include "../../inc/ui/MenuInterface.h"
#include "../../inc/common/Defines.h"
#include "../../inc/game/Settings.h"
#include "../../inc/ui/Cursor.h"

#include <cstdlib> // system()

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** LogoInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool LogoInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Logo");
    _RETURN_IF_ERROR();

    TimeInterface::Load(node);

    _RETURN_STATE();
}

void LogoInterface::AddEvents()
{
    static_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("skip"))->OnTriggered(std::bind(_OnSkip));
}

void LogoInterface::_SelfTerminate()
{
    Application::GetInstance()->GetInterface("Main")->Launch();
}

// Callback
void LogoInterface::_OnSkip()
{
    Application::GetInstance()->GetInterface("Main")->Launch();
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** MainInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool MainInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Main");
    _RETURN_IF_ERROR();

    PlainInterface::Load(node);

    _RETURN_STATE();
}

void MainInterface::AddEvents()
{
    static_cast<Button*>(m_pWidgetManager->GetWidget("exit"))->OnClick(GetTerminator(this));
    static_cast<Button*>(m_pWidgetManager->GetWidget("play"))->OnClick(std::bind(_OnPlay));
    static_cast<Button*>(m_pWidgetManager->GetWidget("setting"))->OnClick(std::bind(_OnSetting));
    static_cast<Button*>(m_pWidgetManager->GetWidget("help"))->OnClick(std::bind(_OnHelp));
    static_cast<Button*>(m_pWidgetManager->GetWidget("about"))->OnClick(std::bind(_OnAbout));
    static_cast<Button*>(m_pWidgetManager->GetWidget("version"))->OnClick(std::bind(_OnVersion));
}

void MainInterface::_Initialize()
{
    static_cast<AudioPlayer*>(m_pWidgetManager->GetWidget("bgm"))->Play();
}

void MainInterface::_Destroy()
{
    static_cast<AudioPlayer*>(m_pWidgetManager->GetWidget("bgm"))->Stop();
}

// Callbacks
void MainInterface::_OnPlay()
{
    Application::GetInstance()->GetInterface("Select")->Attach();
}

void MainInterface::_OnSetting()
{
    Application::GetInstance()->GetInterface("Setting")->Attach();
}

void MainInterface::_OnHelp()
{
    Application::GetInstance()->GetInterface("Help")->Attach();
}

void MainInterface::_OnAbout()
{
    Application::GetInstance()->GetInterface("About")->Attach();
}

void MainInterface::_OnVersion()
{
    Application::GetInstance()->GetInterface("Version")->Attach();
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** SettingInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool SettingInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Setting");
    _RETURN_IF_ERROR();

    PlainInterface::Load(node);
    _RETURN_IF_ERROR();

    _LoadSettings();

    _RETURN_STATE();
}

void SettingInterface::AddEvents()
{
    // Difficulty
    static_cast<Button*>(m_pWidgetManager->GetWidget("minus"))
        ->OnClick(std::bind(&SettingInterface::_OnDifficultyDecrease, this));
    static_cast<Button*>(m_pWidgetManager->GetWidget("plus"))
        ->OnClick(std::bind(&SettingInterface::_OnDifficultyIncrease, this));

    // Fullscreen
    static_cast<Button*>(m_pWidgetManager->GetWidget("left"))
        ->OnClick(std::bind(&SettingInterface::_OnFullscreenChange, this));
    static_cast<Button*>(m_pWidgetManager->GetWidget("right"))
        ->OnClick(std::bind(&SettingInterface::_OnFullscreenChange, this));

    // Volume
    static_cast<Slider*>(m_pWidgetManager->GetWidget("sound"))
        ->OnChange(std::bind(_OnSoundVolume, std::placeholders::_1));
    static_cast<Slider*>(m_pWidgetManager->GetWidget("music"))
        ->OnChange(std::bind(_OnMusicVolume, std::placeholders::_1));

    // Back
    static_cast<Button*>(m_pWidgetManager->GetWidget("back"))->OnClick(GetDetacher(this));
}

void SettingInterface::_Initialize()
{
    _LoadSettings();
}

void SettingInterface::_Destroy()
{
    _SaveSettings();
}

void SettingInterface::_SaveSettings()
{
    Settings::GetInstance()->SaveSettings();
}

void SettingInterface::_LoadSettings()
{
    Settings* settings = Settings::GetInstance();

    _SetDifficulty(settings->Difficulty());
    _SetFullscreen(settings->IsFullscreen());

    static_cast<Slider*>(m_pWidgetManager->GetWidget("sound"))->SetValue(settings->SoundVolume());
    static_cast<Slider*>(m_pWidgetManager->GetWidget("music"))->SetValue(settings->MusicVolume());
}

void SettingInterface::_SetDifficulty(int difficulty)
{
    StaticWidget* textWidget = static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("diff"));
    TextDrawer* text = static_cast<TextDrawer*>(textWidget->GetDrawer());

    if (difficulty < EASY)
    {
        difficulty = INSANE;
    }
    else if (difficulty > INSANE)
    {
        difficulty = EASY;
    }

    Settings::GetInstance()->Difficulty(difficulty);
    text->SetText(DIFFICULTY_TEXT[difficulty]);
}

void SettingInterface::_SetFullscreen(bool fullscreen)
{
    StaticWidget* textWidget = static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("full"));
    TextDrawer* text = static_cast<TextDrawer*>(textWidget->GetDrawer());

    Settings::GetInstance()->IsFullscreen(fullscreen);
    text->SetText(YES_NO_TEXT[static_cast<int>(fullscreen)]);
}

// Callbacks
void SettingInterface::_OnDifficultyIncrease()
{
    _SetDifficulty(Settings::GetInstance()->Difficulty() + 1);
}

void SettingInterface::_OnDifficultyDecrease()
{
    _SetDifficulty(Settings::GetInstance()->Difficulty() - 1);
}

void SettingInterface::_OnFullscreenChange()
{
    _SetFullscreen(!Settings::GetInstance()->IsFullscreen());
}

void SettingInterface::_OnMusicVolume(double volume)
{
    Speaker::GetInstance()->SetMusicVolume(volume);
    Settings::GetInstance()->MusicVolume(volume);
}

void SettingInterface::_OnSoundVolume(double volume)
{
    Speaker::GetInstance()->SetSoundVolume(volume);
    Settings::GetInstance()->SoundVolume(volume);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** SelectInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool SelectInterface::m_selected = false;

bool SelectInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Select");
    _RETURN_IF_ERROR();

    PlainInterface::Load(node);

    _RETURN_STATE();
}

void SelectInterface::AddEvents()
{
    static_cast<Button*>(m_pWidgetManager->GetWidget("back"))->OnClick(GetDetacher(this));
    static_cast<Button*>(m_pWidgetManager->GetWidget("start"))->OnClick(std::bind(_OnStart));

    RadioList* radioList = static_cast<RadioList*>(m_pWidgetManager->GetWidget("hero"));
    radioList->GetRadio("yony")->OnCheck(std::bind(_OnYony, std::placeholders::_1));
    // radioList->GetRadio("spec")->OnCheck(std::bind(_OnSpecter,
    // std::placeholders::_1));
    radioList->GetRadio("blck")->OnCheck(std::bind(_OnBlackHand, std::placeholders::_1));

    static_cast<RadioBox*>(m_pWidgetManager->GetWidget("mode"))
        ->OnCheck(std::bind(_OnChangeMode, std::placeholders::_1));

    static_cast<Button*>(m_pWidgetManager->GetWidget("buy-blck"))
        ->OnClick(std::bind(&SelectInterface::_OnPurchaseBlackHand, this));
}

void SelectInterface::_Initialize()
{
    _ResetState();
    _InitWidget();
    Update(nullptr);
}

void SelectInterface::_ResetState()
{
    RadioList* radioList = static_cast<RadioList*>(m_pWidgetManager->GetWidget("hero"));
    radioList->GetRadio("yony")->SetState(false);
    // radioList->GetRadio("spec")->SetState(false);
    radioList->GetRadio("blck")->SetState(false);

    static_cast<RadioBox*>(m_pWidgetManager->GetWidget("mode"))->SetState(Settings::GetInstance()->IsInfinite());

    Settings::GetInstance()->HeroName("");

    m_selected = false;
}

void SelectInterface::_InitWidget()
{
    auto settings = Settings::GetInstance();

    // Set coin board.
    auto coin = static_cast<StaticWidget*>(m_pWidgetManager->GetWidget("coin-tip"));
    char buffer[16];
    sprintf_s(buffer, "%d", Settings::GetInstance()->Coin());
    static_cast<TextDrawer*>(coin->GetDrawer())->SetText(buffer);

    // Set hero
    RadioList* radioList = static_cast<RadioList*>(m_pWidgetManager->GetWidget("hero"));
    Button* buy = static_cast<Button*>(m_pWidgetManager->GetWidget("buy-blck"));
    RadioBox* blck = radioList->GetRadio("blck");
    if (settings->IsAvailable("BlackHand"))
    {
        blck->Activate(true);
        buy->Activate(false);
    }
    else
    {
        blck->Activate(false);
        buy->Activate(true);
        if (settings->CanBeAvailable("BlackHand"))
        {
            buy->Disable(false);
        }
        else
        {
            buy->Disable(true);
        }
    }
}

void SelectInterface::_OnPurchaseBlackHand()
{
    auto settings = Settings::GetInstance();

    if (settings->MakeAvailable("BlackHand"))
    {
        _InitWidget();
        settings->SaveConfig();

#ifdef DUNGEON_DEBUG
        LOG_MESSAGE("Black Hand purchased");
#endif
    }
}

// Callbacks
void SelectInterface::_OnYony(bool isSelected)
{
    if (isSelected)
    {
        Settings::GetInstance()->HeroName("Yony");
        m_selected = true;
    }
}

void SelectInterface::_OnSpecter(bool isSelected)
{
    if (isSelected)
    {
        Settings::GetInstance()->HeroName("Specter");
        m_selected = true;
    }
}

void SelectInterface::_OnBlackHand(bool isSelected)
{
    if (isSelected)
    {
        Settings::GetInstance()->HeroName("BlackHand");
        m_selected = true;
    }
}

void SelectInterface::_OnChangeMode(bool isSelected)
{
    Settings::GetInstance()->IsInfinite(isSelected);
}

void SelectInterface::_OnStart()
{
    if (m_selected)
    {
        Application::GetInstance()->GetInterface("Game")->Launch();
    }
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** HelpInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool HelpInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Help");
    _RETURN_IF_ERROR();

    PlainInterface::Load(node);

    _RETURN_STATE();
}

void HelpInterface::AddEvents()
{
    static_cast<Button*>(m_pWidgetManager->GetWidget("back"))->OnClick(GetDetacher(this));
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** PauseInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool PauseInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Pause");
    _RETURN_IF_ERROR();

    PlainInterface::Load(node);

    _RETURN_STATE();
}

void PauseInterface::AddEvents()
{
    static_cast<Button*>(m_pWidgetManager->GetWidget("resume"))->OnClick(std::bind(&PauseInterface::_OnResume, this));
    static_cast<Button*>(m_pWidgetManager->GetWidget("setting"))->OnClick(std::bind(_OnSetting));
}

void PauseInterface::_OnResume()
{
    m_pSupIntf->ClearSubInterface();
    SetCursorStyle(CUR_ATTACK);
}

void PauseInterface::_OnSetting()
{
    Application::GetInstance()->GetInterface("In-Setting")->Attach();
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** QuitInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool QuitInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Quit");

    PlainInterface::Load(node);

    _RETURN_STATE();
}

void QuitInterface::AddEvents()
{
    static_cast<Button*>(m_pWidgetManager->GetWidget("confirm"))->OnClick(std::bind(&QuitInterface::_OnConfirm, this));
    static_cast<Button*>(m_pWidgetManager->GetWidget("cancel"))->OnClick(std::bind(&QuitInterface::_OnCancel, this));
}

void QuitInterface::_OnConfirm()
{
    m_pSupIntf->Detach();
}

void QuitInterface::_OnCancel()
{
    m_pSupIntf->ClearSubInterface();
    SetCursorStyle(CUR_ATTACK);
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** AboutInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
std::string AboutInterface::m_studioUrl; // studio url
std::string AboutInterface::m_githubUrl; // github url

bool AboutInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("About");
    _RETURN_IF_ERROR();

    PlainInterface::Load(node);
    _RETURN_IF_ERROR();

    XMLElement* links = GetElementByTagName(node, "Links");
    if (!links)
    {
        LOG_ERROR(MISSING_TAG, "Links", name);
        return false;
    }
    _LoadLink(links, "Studio", m_studioUrl);
    _LoadLink(links, "Github", m_githubUrl);
    _RETURN_IF_ERROR();

    _RETURN_STATE();
}

void AboutInterface::AddEvents()
{
    static_cast<Button*>(m_pWidgetManager->GetWidget("back"))->OnClick(GetDetacher(this));
    static_cast<Button*>(m_pWidgetManager->GetWidget("url-studio"))
        ->OnClick(std::bind(_OnOpenUrl, m_studioUrl.c_str()));
    static_cast<Button*>(m_pWidgetManager->GetWidget("url-github"))
        ->OnClick(std::bind(_OnOpenUrl, m_githubUrl.c_str()));
}

void AboutInterface::_OnOpenUrl(std::string url)
{
    url = "start " + url;

    std::system(url.c_str());
}

bool AboutInterface::_LoadLink(XMLElement* root, const char* link, std::string& url)
{
    XMLElement* node = GetElementByTagName(root, link);
    if (!node)
    {
        LOG_ERROR(MISSING_TAG, link, root);
        return false;
    }
    const char* text = node->GetText();
    if (!text)
    {
        LOG_ERROR("Missing link of \"%s\"", node->Name());
        return false;
    }
    url = text;

    return true;
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** VictoryInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
bool VictoryInterface::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;

    _CHECK_TAG("Victory");
    _RETURN_IF_ERROR();

    TimeInterface::Load(node);
    _RETURN_IF_ERROR();

    _PARSE("victory", m_isVictory, name, false);

    _RETURN_STATE();
}

void VictoryInterface::AddEvents()
{
    static_cast<KeyboardDetector*>(m_pWidgetManager->GetWidget("skip"))
        ->OnTriggered(std::bind(&VictoryInterface::_SelfTerminate, this));
}

/*
** 2022/11/27 TS:
** Special behavior for victory interface.
*/
void VictoryInterface::_SelfTerminate()
{
    // Non-victory interface behavior.
    if (!m_isVictory)
    {
        Detach();
        return;
    }

    // Victory interface behavior.
    if (Settings::GetInstance()->ShowCredits())
    {
        Application::GetInstance()->GetInterface("Credits")->Launch();
        Settings::GetInstance()->ShowCredits(false);
        Settings::GetInstance()->SaveConfig();
    }
    else
    {
        Detach();
    }
}
