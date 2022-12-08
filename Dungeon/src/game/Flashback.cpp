/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Flashback.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : December 8, 2022                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For game flashback. Hmm... Save current progress. :)                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Defines.h"

#include "../../inc/game/Flashback.h"


/******************************************************************************
 * Flashback::FlashBack -- Constructor of the object.                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/12/08 Tony : Created.                                               *
 *============================================================================*/
Flashback::Flashback() : m_isValid(false)
{
	Link(FLASHBACK_XML);
}

/******************************************************************************
* Flashback::FlashBack -- Load load flashback.                               *
*                                                                            *
*    Just the literal meaning.                                               *
*                                                                            *
* INPUT:   none                                                              *
*                                                                            *
* OUTPUT:  none                                                              *
*                                                                            *
* WARNINGS:  none                                                            *
*                                                                            *
* HISTORY:                                                                   *
*   2022/12/08 Tony : Created.                                               *
*============================================================================*/
bool Flashback::Load()
{
	XMLFile file;
	if (!_LoadFile(file))
		return false;

	try
	{
		_LoadEntry(file, &m_isValid, "Valid");
		if (m_isValid)
		{
			_LoadEntry(file, &m_chapter, "Chapter");
			_LoadEntry(file, &m_level, "Level");

			_LoadEntry(file, &m_heroName, "Hero");
			_LoadEntry(file, &m_hp, "HP");
			_LoadEntry(file, &m_mp, "MP");
			_LoadEntry(file, &m_armor, "Armor");
			_LoadEntry(file, &m_chi, "Chi");

			_LoadWeaponList(file);
		}
	}
	catch (EntryError)
	{
		Save();
	}

	return true;
}


/******************************************************************************
* Flashback::FlashBack -- Save flashback.                                    *
*                                                                            *
*    Just the literal meaning.                                               *
*                                                                            *
* INPUT:   none                                                              *
*                                                                            *
* OUTPUT:  none                                                              *
*                                                                            *
* WARNINGS:  none                                                            *
*                                                                            *
* HISTORY:                                                                   *
*   2022/12/08 Tony : Created.                                               *
*============================================================================*/
bool Flashback::Save()
{
	XMLFile file;
	if (!_LoadFile(file))
		return false;

	_SaveEntry(file, &m_isValid, "Valid");
	if (!m_isValid)
		return true;

	_SaveEntry(file, &m_chapter, "Chapter");
	_SaveEntry(file, &m_level, "Level");

	_SaveEntry(file, &m_heroName, "Hero");
	_SaveEntry(file, &m_hp, "HP");
	_SaveEntry(file, &m_mp, "MP");
	_SaveEntry(file, &m_armor, "Armor");
	_SaveEntry(file, &m_chi, "Chi");

	_SaveWeaponList(file);

	return true;
}


/******************************************************************************
* Flashback::_LoadWeaponList -- Load weapon list.                            *
*                                                                            *
*    Just the literal meaning.                                               *
*                                                                            *
* INPUT:   file                                                              *
*                                                                            *
* OUTPUT:  none                                                              *
*                                                                            *
* WARNINGS:  none                                                            *
*                                                                            *
* HISTORY:                                                                   *
*   2022/12/08 Tony : Created.                                               *
*============================================================================*/
void Flashback::_LoadWeaponList(XMLFile& file)
{
	XMLElement* entry = file.GetElementByTagName("Weapons");
	if (!entry)
		throw EntryError("Weapons");

	// Haha... auto. :P
	auto tagList = GetElementsByTagName(entry, "Weapon");
	const char* name;
	m_weaponList.clear();
	for (auto tag : tagList)
	{
		name = tag->GetText();
		if (!name)
			throw EntryError("Missing weapon name");
		m_weaponList.push_back(name);
	}
}


/******************************************************************************
* Flashback::_SaveWeaponList -- Save weapon list.                            *
*                                                                            *
*    Just the literal meaning.                                               *
*                                                                            *
* INPUT:   file                                                              *
*                                                                            *
* OUTPUT:  none                                                              *
*                                                                            *
* WARNINGS:  none                                                            *
*                                                                            *
* HISTORY:                                                                   *
*   2022/12/08 Tony : Created.                                               *
*============================================================================*/
void Flashback::_SaveWeaponList(XMLFile& file)
{
	XMLElement* entry = file.GetElementByTagName("Weapons");
	if (!entry)
		throw EntryError("Weapons");

	XMLElement* tag;
	entry->DeleteChildren();
	for (auto it = m_weaponList.begin(); it != m_weaponList.end(); it++)
	{
		tag = entry->InsertNewChildElement("Weapon");
		tag->SetText(it->c_str());
	}
}
