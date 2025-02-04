/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : ErrorInterface.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 30, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Error interface.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/ui/ErrorInterface.h"

bool ErrorInterface::Load(XMLElement* node)
{
    m_name = "Error";
    m_pWidgetManager = new WidgetManager();

    Coordinate center;

    /*
    ** Background
    */
    StaticWidget* back = new StaticWidget();
    RectDrawer* plain = new RectDrawer();

    plain->SetCoord(0, 0)->SetSize(deviceInfo.clientWidth, deviceInfo.clientHeight);
    plain->Filled(true)->SetFillColor(RGB(41, 182, 246));
    back->SetDrawer(plain);

    back->SetName("plain");
    back->SetLayer(-1);
    m_pWidgetManager->AddWidget(back);

    /*
    ** Error prompt.
    */
    StaticWidget* error;
    TextDrawer* prompt;
    Coordinate size(deviceInfo.clientWidth, 60);

    error = new StaticWidget();
    prompt = new TextDrawer();
    center.Init(deviceInfo.clientWidth / 2, static_cast<int>(deviceInfo.clientHeight * 0.2));
    prompt->SetCoord(center)->SetSize(size.x, size.y)->SetFocus(size.x / 2, size.y / 2);
    prompt->SetFontSize(50)->SetFontColor(RGB(183, 28, 28))->SetFontFace("Fixedsys")->SetText("Fatal Error :(");
    error->SetDrawer(prompt)->SetLayer(0)->SetName("title");
    m_pWidgetManager->AddWidget(error);

    error = new StaticWidget();
    prompt = new TextDrawer();
    center.y += 60;
    prompt->SetCoord(center)->SetSize(size.x, size.y)->SetFocus(size.x / 2, size.y / 2);
    prompt->SetFontSize(20)
        ->SetFontColor(RGB(183, 28, 28))
        ->SetFontFace("Fixedsys")
        ->SetText(R"(Please check "log.txt" for more information.)");
    error->SetDrawer(prompt)->SetLayer(0)->SetName("promt1");
    m_pWidgetManager->AddWidget(error);

    error = new StaticWidget();
    prompt = new TextDrawer();
    center.y += 30;
    prompt->SetCoord(center)->SetSize(size.x, size.y)->SetFocus(size.x / 2, size.y / 2);
    prompt->SetFontSize(20)
        ->SetFontColor(RGB(183, 28, 28))
        ->SetFontFace("Fixedsys")
        ->SetText(R"(Or contact New Desire Studios.)");
    error->SetDrawer(prompt)->SetLayer(0)->SetName("promt2");
    m_pWidgetManager->AddWidget(error);

    error = new StaticWidget();
    prompt = new TextDrawer();
    center.y += 30;
    prompt->SetCoord(center)->SetSize(size.x, size.y)->SetFocus(size.x / 2, size.y / 2);
    prompt->SetFontSize(20)
        ->SetFontColor(RGB(183, 28, 28))
        ->SetFontFace("Fixedsys")
        ->SetText(R"(We're really sorry about this. (> <))");
    error->SetDrawer(prompt)->SetLayer(0)->SetName("promt3");
    m_pWidgetManager->AddWidget(error);

    error = new StaticWidget();
    prompt = new TextDrawer();
    center.y += 30;
    prompt->SetCoord(center)->SetSize(size.x, size.y)->SetFocus(size.x / 2, size.y / 2);
    prompt->SetFontSize(20)
        ->SetFontColor(RGB(183, 28, 28))
        ->SetFontFace("Fixedsys")
        ->SetText(R"(Email: tony-turmoil@qq.com)");
    error->SetDrawer(prompt)->SetLayer(0)->SetName("promt4");
    m_pWidgetManager->AddWidget(error);

    /*
    ** Exit button.
    */
    Button* quitButton = new Button();
    RectDrawer* rect;
    TextDrawer* text;
    RectTrigger* trigger;

    center.Init(deviceInfo.clientWidth / 2, static_cast<int>(deviceInfo.clientHeight * 0.7));

    // up
    rect = new RectDrawer();
    rect->SetCoord(center)->SetSize(100, 60)->SetFocus(54, 34);
    rect->Filled(true)->SetFillColor(RGB(100, 221, 23))->SetLineThickness(8)->SetLineColor(RGB(70, 154, 16));
    text = new TextDrawer();
    text->SetCoord(center)->SetSize(100, 60)->SetFocus(50, 30);
    text->SetText("Exit")->SetFontSize(40)->SetFontFace("Fixedsys")->SetFontColor(BLACK);
    rect->SetSubDrawer(text);
    quitButton->SetUpDrawer(rect);
    // hover
    rect = new RectDrawer();
    rect->SetCoord(center)->SetSize(100, 60)->SetFocus(54, 34);
    rect->Filled(true)->SetFillColor(RGB(118, 255, 3))->SetLineThickness(8)->SetLineColor(RGB(70, 154, 16));
    text = new TextDrawer();
    text->SetCoord(center)->SetSize(100, 60)->SetFocus(50, 30);
    text->SetText("Exit")->SetFontSize(40)->SetFontFace("Fixedsys")->SetFontColor(BLACK);
    rect->SetSubDrawer(text);
    quitButton->SetHoverDrawer(rect);
    // down
    rect = new RectDrawer();
    rect->SetCoord(center)->SetSize(100, 60)->SetFocus(54, 34);
    rect->Filled(true)->SetFillColor(RGB(82, 178, 2))->SetLineThickness(8)->SetLineColor(RGB(70, 154, 16));
    text = new TextDrawer();
    text->SetCoord(center)->SetSize(100, 60)->SetFocus(50, 30);
    text->SetText("Exit")->SetFontSize(40)->SetFontFace("Fixedsys")->SetFontColor(BLACK);
    rect->SetSubDrawer(text);
    quitButton->SetDownDrawer(rect);
    // trigger
    trigger = new RectTrigger();
    trigger->SetCoord(center)->SetSize(100, 60)->SetFocus(50, 30);
    quitButton->SetTrigger(trigger);

    quitButton->SetLayer(0);
    quitButton->SetName("quit");
    m_pWidgetManager->AddWidget(quitButton);

    return true;
}

void ErrorInterface::AddEvents()
{
    static_cast<Button*>((*m_pWidgetManager)["quit"])->OnClick(GetTerminator(this));
}
