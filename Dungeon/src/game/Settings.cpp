/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Settings.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 10, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Game settings is a singleton.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <dungine.h>

#include "../../inc/game/Settings.h"

// Hmm... well, just load these two externally.
int CHAPTER_NUM;
int LEVEL_NUM;


/******************************************************************************
 * Settings::Link -- Link settings to the file.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   filename -- The file of settings.                                 *
 *                                                                            *
 * OUTPUT:  Return true if the file is valid.                                 *
 *                                                                            *
 * WARNINGS:  It won't check the content yet.                                 *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
bool Settings::Link(const char* filename)
{
	if (IsValidDirectory(filename))
	{
		m_filename = filename;
		return true;
	}

	return false;
}


/******************************************************************************
 * Settings::Load -- Load settings from the linked file.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return succeeded or not.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
bool Settings::Load()
{
	if (!_IsLinked())
		return false;

	XMLFile file;
	if (!file.Load(m_filename))
	{
		LOG_ERROR(FAILED_TO_LOAD, "Settings");
		return false;
	}

/*
**	<Settings>
**		<Difficulty></Difficulty>
**		...
**	</Settings>
*/
	try
	{
		_LoadEntry(file, &m_difficulty, "Difficulty");
		_LoadEntry(file, &m_soundVolume, "SoundVolume");
		_LoadEntry(file, &m_musicVolume, "MusicVolume");
		_LoadEntry(file, &m_isFullscreen, "Fullscreen");

		_LoadEntry(file, &m_coin, "Coin");

		_LoadEntry(file, &CHAPTER_NUM, "ChapterNum");
		_LoadEntry(file, &LEVEL_NUM, "LevelNum");

		_LoadHeroInfo(file);
	}
	catch (EntryError)
	{
		Save();
	}

	return true;
}


/******************************************************************************
 * Settings::Save -- Save settings to the linked file.                        *
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
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
bool Settings::Save()
{
	return SaveSettings() && SaveConfig();
}

bool Settings::SaveSettings()
{
	if (!_IsLinked())
		return false;

	XMLFile file;
	if (!file.Load(m_filename))
	{
		LOG_ERROR(FAILED_TO_LOAD, "Settings");
		return false;
	}

	_SaveEntry(file, m_difficulty, "Difficulty");
	_SaveEntry(file, m_isFullscreen, "Fullscreen");
	_SaveEntry(file, m_soundVolume, "SoundVolume");
	_SaveEntry(file, m_musicVolume, "MusicVolume");

	file.Save();

	return true;
}

bool Settings::SaveConfig()
{
	if (!_IsLinked())
		return false;

	XMLFile file;
	if (!file.Load(m_filename))
	{
		LOG_ERROR(FAILED_TO_LOAD, "Settings");
		return false;
	}

	_SaveEntry(file, m_coin, "Coin");
	_SaveHeroInfo(file);

	file.Save();

	return true;
}


/******************************************************************************
 * Settings::IsAvailable -- Check if hero is available or not.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/18 Tony : Created.                                               *
 *============================================================================*/
bool Settings::IsAvailable(const std::string& name) const
{
	auto it = m_heroInfo.find(name);
	if (it == m_heroInfo.end())
		return false;

	return it->second.isAvailable;
}


/******************************************************************************
 * Settings::CanBeAvailable -- If the hero can be available or not.           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/18 Tony : Created.                                               *
 *============================================================================*/
bool Settings::CanBeAvailable(const std::string& name) const
{
	auto it = m_heroInfo.find(name);
	if (it == m_heroInfo.end())
		return false;
	HeroInfo info = it->second;

	return info.isAvailable ? true : (m_coin >= info.price);
}


/******************************************************************************
 * Settings::MakeAvailable -- Try to make hero available.                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/18 Tony : Created.                                               *
 *============================================================================*/
bool Settings::MakeAvailable(const std::string& name)
{
	auto it = m_heroInfo.find(name);
	if (it == m_heroInfo.end())
		return false;
	HeroInfo& info = it->second;

	if (info.isAvailable)
		return true;
	if (m_coin < info.price)
		return false;

	m_coin -= info.price;
	info.isAvailable = true;

	return true;
}


/******************************************************************************
 * Settings::Settings -- Constructor of the object.                           *
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
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
Settings::Settings() :
	m_difficulty(0),
	m_coin(0),
	m_soundVolume(1.0),
	m_musicVolume(1.0),
	m_filename(nullptr),
	m_isFullscreen(true)
{
}


/******************************************************************************
 * Settings::_IsLinked -- Check if file linked.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return the result. :?                                             *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
bool Settings::_IsLinked()
{
	if (m_filename)
		return true;

	LOG_ERROR("No file linked to Settings");

	return false;
}


/******************************************************************************
 * Settings::LoadEntry -- Load a setting entry.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node -- The parent node.                                          *
 *          val                                                               *
 *          tag  -- Tag name.                                                 *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  Hmm... exception may throw.                                     *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
void Settings::_LoadEntry(XMLFile& file, int* val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	if (!entry)
		throw EntryError(tag);

	const char* str = entry->GetText();
	if (!(str && ParseAttribute(val, str, 0)))
		throw EntryError(tag);
}

void Settings::_LoadEntry(XMLFile& file, double* val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	if (!entry)
		throw EntryError(tag);

	const char* str = entry->GetText();
	if (!(str && ParseAttribute(val, str, 1.0)))
		throw EntryError(tag);
}

void Settings::_LoadEntry(XMLFile& file, bool* val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	if (!entry)
		throw EntryError(tag);

	const char* str = entry->GetText();
	if (!(str && ParseAttribute(val, str, true)))
		throw EntryError(tag);
}

void Settings::_LoadHeroInfo(XMLFile& file)
{
	XMLElement* entry = file.GetElementByTagName("Hero");
	if (!entry)
		throw EntryError("Hero");

	XMLElement* node = entry->FirstChildElement();
	const char* name;
	const char* attr;
	HeroInfo info;
	while (node)
	{
		name = node->Name();
		_PARSE("available", info.isAvailable, name, false);
		_PARSE("price", info.price, name, false);
		m_heroInfo.emplace(name, info);

		node = node->NextSiblingElement();
	}
}


/******************************************************************************
 * Settings::_SaveEntry -- Save a setting entry.                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node -- The parent node.                                          *
 *          val                                                               *
 *          tag  -- Tag name.                                                 *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
void Settings::_SaveEntry(XMLFile& file, int val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	char buffer[32];

	sprintf_s(buffer, "%d", val);

	if (!entry)
	{
		entry = file.Doc().NewElement(tag);
		entry->SetText(buffer);
		file.GetRoot()->InsertEndChild(entry);
	}
	else
		entry->SetText(buffer);
}

void Settings::_SaveEntry(XMLFile& file, double val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	char buffer[32];

	sprintf_s(buffer, "%.2f", val);

	if (!entry)
	{
		entry = file.Doc().NewElement(tag);
		entry->SetText(buffer);
		file.GetRoot()->InsertEndChild(entry);
	}
	else
		entry->SetText(buffer);
}

void Settings::_SaveEntry(XMLFile& file, bool val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	char buffer[32];

	sprintf_s(buffer, "%s", val ? "true" : "false");

	if (!entry)
	{
		entry = file.Doc().NewElement(tag);
		file.GetRoot()->InsertEndChild(entry);
	}
	entry->SetText(buffer);
}

void Settings::_SaveHeroInfo(XMLFile& file)
{
	XMLElement* entry = file.GetElementByTagName("Hero");
	XMLElement* node;
	std::string name;

	for (auto it = m_heroInfo.begin(); it != m_heroInfo.end(); it++)
	{
		node = entry->FirstChildElement(it->first.c_str());
		if (!node)
		{
			node = file.Doc().NewElement(it->first.c_str());
			entry->InsertEndChild(node);
		}
		node->SetAttribute("available", it->second.isAvailable);
		node->SetAttribute("price", it->second.price);
	}
}