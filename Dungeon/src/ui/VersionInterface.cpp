/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : VersionInterface.h                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 26, 2022                           *
 *                                                                            *
 *                    Last Update : November 26, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Version interface.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/ui/VersionInterface.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** VersionInterface
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
VersionInterface::VersionInterface()
{
    m_curPage = m_pages.end();
}

VersionInterface::~VersionInterface()
{
    for (auto it = m_pages.begin(); it != m_pages.end(); it++)
    {
        delete (*it);
    }
}

bool VersionInterface::Load(XMLElement* node)
{
    const char* name = node->Name();

    _CHECK_TAG("Version");
    _RETURN_IF_ERROR();

    PlainInterface::Load(node);
    _RETURN_IF_ERROR();

    WidgetManager* mgr;
    XMLElement* page = node->FirstChildElement("Page");
    while (page)
    {
        mgr = new WidgetManager();
        mgr->Load(page);
        m_pages.push_back(mgr);

        page = page->NextSiblingElement();
    }

    _OnRewind();

    _RETURN_STATE();
}

void VersionInterface::Update(Event* evnt)
{
    if (m_pSubIntf)
    {
        m_pSubIntf->Update(evnt);
    }
    else
    {
        m_pWidgetManager->Update(evnt);
        _UpdatePage(evnt);
        if (evnt)
        {
            _ProcessInput(*evnt);
        }
    }
}

void VersionInterface::Draw()
{
    m_pWidgetManager->Draw();
    _DrawPage();
    if (m_pSubIntf)
    {
        m_pSubIntf->Draw();
    }
}

void VersionInterface::Draw(IMAGE* pDestImage)
{
    m_pWidgetManager->Draw();
    _DrawPage();
    if (m_pSubIntf)
    {
        m_pSubIntf->Draw();
    }

    Device::GetInstance()->Render(pDestImage);
}

void VersionInterface::_UpdatePage(Event* evnt)
{
    if (m_curPage != m_pages.end())
    {
        (*m_curPage)->Update(evnt);
    }
}

void VersionInterface::_DrawPage()
{
    if (m_curPage != m_pages.end())
    {
        (*m_curPage)->Draw();
    }
}

void VersionInterface::_PageUp()
{
    if ((m_curPage != m_pages.end()) && (m_curPage > m_pages.begin()))
    {
        m_curPage--;
    }
    _UpdateControl();
}

void VersionInterface::_PageDown()
{
    if ((m_curPage != m_pages.end()) && (m_curPage + 1 < m_pages.end()))
    {
        m_curPage++;
    }
    _UpdateControl();
}

void VersionInterface::_UpdateControl()
{
    if (m_curPage == m_pages.end())
    {
        static_cast<Button*>(m_pWidgetManager->GetWidget("up"))->Activate(false);
        static_cast<Button*>(m_pWidgetManager->GetWidget("down"))->Activate(false);

        return;
    }

    if (m_curPage == m_pages.begin())
    {
        static_cast<Button*>(m_pWidgetManager->GetWidget("up"))->Activate(false);
    }
    else
    {
        static_cast<Button*>(m_pWidgetManager->GetWidget("up"))->Activate(true);
    }

    if (m_curPage == m_pages.end() - 1)
    {
        static_cast<Button*>(m_pWidgetManager->GetWidget("down"))->Activate(false);
    }
    else
    {
        static_cast<Button*>(m_pWidgetManager->GetWidget("down"))->Activate(true);
    }
}

void VersionInterface::AddEvents()
{
    static_cast<Button*>(m_pWidgetManager->GetWidget("back"))->OnClick(GetDetacher(this));
    static_cast<Button*>(m_pWidgetManager->GetWidget("credits"))->OnClick(std::bind(_OnCredits));
    static_cast<Button*>(m_pWidgetManager->GetWidget("up"))->OnClick(std::bind(&VersionInterface::_PageUp, this));
    static_cast<Button*>(m_pWidgetManager->GetWidget("down"))->OnClick(std::bind(&VersionInterface::_PageDown, this));
}

void VersionInterface::_Initialize()
{
    _OnRewind();
}

void VersionInterface::_OnRewind()
{
    m_curPage = m_pages.begin();
    _UpdateControl();
}

void VersionInterface::_OnCredits()
{
    Application::GetInstance()->GetInterface("Credits")->Attach();
}
