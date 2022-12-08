/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Pipe.cpp                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : December 8, 2022                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For XML data reading and writing.                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/device/Color.h"

#include "../../inc/utility/Auxilliary.h"
#include "../../inc/utility/Parser.h"
#include "../../inc/utility/Pipe.h"


/******************************************************************************
 * Pipe::Link -- Link Pipe to the file.                                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   filename -- The file of Pipe.                                     *
 *                                                                            *
 * OUTPUT:  Return true if the file is valid.                                 *
 *                                                                            *
 * WARNINGS:  It won't check the content yet.                                 *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
bool Pipe::Link(const char* filename)
{
	if (IsValidDirectory(filename))
	{
		m_filename = filename;
		return true;
	}

	return false;
}


/******************************************************************************
 * Pipe::_IsLinked -- Check if file linked.                                   *
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
bool Pipe::_IsLinked()
{
	if (m_filename)
		return true;

	LOG_ERROR("No file linked to Pipe");

	return false;
}


/******************************************************************************
 * Pipe::_LoadFile -- Load file.                                              *
 *                                                                            *
 *    This will load the linked XML file to the given XMLFile.                *
 *                                                                            *
 * INPUT:   file -- The target file to load.                                  *
 *                                                                            *
 * OUTPUT:  Load status.                                                      *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/12/08 Tony : Created.                                               *
 *============================================================================*/
bool Pipe::_LoadFile(XMLFile& file)
{
	if (!_IsLinked())
		return false;

	if (!file.Load(m_filename))
	{
		LOG_ERROR(FAILED_TO_LOAD, m_filename);
		return false;
	}

	return true;
}


/******************************************************************************
 * Pipe::LoadEntry -- Load a setting entry.                                   *
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
 *   2022/11/24 Tony : Added COLORREF support.                                *
 *============================================================================*/
void Pipe::_LoadEntry(XMLFile& file, int* val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	if (!entry)
		throw EntryError(tag);

	const char* str = entry->GetText();
	if (!(str && ParseAttribute(val, str, 0)))
		throw EntryError(tag);
}

void Pipe::_LoadEntry(XMLFile& file, double* val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	if (!entry)
		throw EntryError(tag);

	const char* str = entry->GetText();
	if (!(str && ParseAttribute(val, str, 1.0)))
		throw EntryError(tag);
}

void Pipe::_LoadEntry(XMLFile& file, bool* val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	if (!entry)
		throw EntryError(tag);

	const char* str = entry->GetText();
	if (!(str && ParseAttribute(val, str, true)))
		throw EntryError(tag);
}

void Pipe::_LoadEntry(XMLFile& file, std::string* val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	if (!entry)
		throw EntryError(tag);

	const char* str = entry->GetText();
	if (!str)
		*val = "";
	else
		*val = str;
}

void Pipe::_LoadEntry(XMLFile& file, COLORREF* val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	if (!entry)
		throw EntryError(tag);

	const char* str = entry->GetText();
	if (!(str && ParsePrivateAttribute(val, str, ParseColor)))
		throw EntryError(tag);
}


/******************************************************************************
 * Pipe::_SaveEntry -- Save a setting entry.                                  *
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
 *   2022/11/24 Tony : Added COLORREF support.                                *
 *============================================================================*/
void Pipe::_SaveEntry(XMLFile& file, int val, const char* tag)
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

void Pipe::_SaveEntry(XMLFile& file, double val, const char* tag)
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

void Pipe::_SaveEntry(XMLFile& file, bool val, const char* tag)
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

void Pipe::_SaveEntry(XMLFile& file, const std::string& val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);

	if (!entry)
	{
		entry = file.Doc().NewElement(tag);
		file.GetRoot()->InsertEndChild(entry);
	}
	entry->SetText(val.c_str());
}

void Pipe::_SaveEntry(XMLFile& file, COLORREF val, const char* tag)
{
	XMLElement* entry = file.GetElementByTagName(tag);
	char buffer[32];

	// Convert BGR to RGB.
	sprintf_s(buffer, "#%x", BGRtoRGB(val));

	if (!entry)
	{
		entry = file.Doc().NewElement(tag);
		file.GetRoot()->InsertEndChild(entry);
	}
	entry->SetText(buffer);
}
