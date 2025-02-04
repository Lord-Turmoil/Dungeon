/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : CreditsInterface.h                        *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 17, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Credits interface.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "Interface.h"

/********************************************************************
** Credits interface have to load credits text and generate layout
** automatically.
*/
class CreditsInterface : public TimeInterface
{
public:
    CreditsInterface() : m_drawer(nullptr), m_curDrawer(nullptr)
    {
    }

    ~CreditsInterface() override
    {
    }

    bool Load(XMLElement* node) override;

    void AddEvents() override;

protected:
    void _Initialize() override;
    void _Destroy() override;

private:
    enum CreditsType
    {
        CRE_TITLE = 0,
        CRE_CHAPTER,
        CRE_SECTION,
        CRE_ITEM,
        CRE_COPYRIGHT,

        CRE_NUM
    };

private:
    bool _LoadCredits(XMLElement* node);
    void _LoadItem(XMLElement* node, CreditsType type);

    void _InitializeProperty(XMLElement* node);
    void _InitializeProperty(XMLElement* node, CreditsType type);
    void _InitializeWidget();

private:
    int m_marginTop[CRE_NUM];
    int m_lineHeight[CRE_NUM];
    int m_fontSize[CRE_NUM];
    COLORREF m_fontColor[CRE_NUM];
    Coordinate m_size[CRE_NUM];
    std::string m_fontFace[CRE_NUM];

    clock_t m_creditsDuration;
    Coordinate m_beginPos;
    Coordinate m_endPos;
    Coordinate m_curPos;
    TextDrawer* m_drawer;
    TextDrawer* m_curDrawer;
};
