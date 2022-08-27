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
 * Over View:                                                                 *
 *   Credits interface.                                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "Interface.h"

/********************************************************************
** Credits interface have to load credits text and generate layout
** automatically.
*/
class CreditsInterface : public TimeInterface
{
public:
	CreditsInterface() : m_drawer(nullptr), m_curDrawer(nullptr) {}
	virtual ~CreditsInterface() {}

	virtual bool Load(XMLElement* node);

	virtual void AddEvents();

protected:
	virtual void _Initialize();
	virtual void _Destroy();

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