/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : xml.h                                     *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9th, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The xml file class to load xml file. Since tinyxml2 does not support     *
 *   unicode, while Windows API has to use wide char, here I made a compromise*
 *   to use xml files only.                                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <string.h>

#include "../../inc/utility/xml.h"
#include "../../inc/utility/Auxilliary.h"

#include "../../inc/common/Logger.h"
#include "../../inc/common/Common.h"


/******************************************************************************
 * XMLFile::~XMLFile -- Destructor of the object.                             *
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
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
XMLFile::~XMLFile()
{
	UnLoad();
}


/******************************************************************************
 * XMLFile::Load -- Load XML file from memory.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   filename -- XML file name.                                        *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  Remember to unload the previous xml file.                       *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
bool XMLFile::Load(const char* filename)
{
	int ret;

	if (m_isLoaded)
		UnLoad();

	ret = m_doc.LoadFile(filename);
	if (ret != tinyxml2::XML_SUCCESS)
	{
		LOG_ERROR(CANNOT_LOAD_XML, filename, ret);
		return false;
	}

	m_isLoaded = true;
	m_filename = filename;

	return true;
}

bool XMLFile::Save()
{
	return Save(m_filename);
}

bool XMLFile::Save(const char* filename)
{
	if (!m_isLoaded)
		return false;

	int ret = m_doc.SaveFile(filename);
	if (ret != tinyxml2::XML_SUCCESS)
	{
		LOG_ERROR(CANNOT_SAVE_XML, filename, ret);
		return false;
	}

	return true;
}


/******************************************************************************
 * XMLFile::UnLoad -- Unload the current xml file.                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  If no file is loaded, return false.                               *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
bool XMLFile::UnLoad()
{
	m_doc.Clear();
	m_isLoaded = false;

	return true;
}


/******************************************************************************
 * XMLFile::GetRoot -- Get the root of the XML file.                          *
 *                                                                            *
 *    If the file doesn't have a root, then it won't even be loaded. So there *
 *    must be a root.                                                         *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return the root of the XML file. Return nullptr if no file is     *
 *          loaded.                                                           *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
XMLElement* XMLFile::GetRoot()
{
	if (!m_isLoaded)
		return nullptr;

	return m_doc.RootElement();
}


/******************************************************************************
 * XMLFile::GetElementByTagName -- Literally.                                 *
 *                                                                            *
 *    This one assumes that there's only one qualified.                       *
 *                                                                            *
 * INPUT:   node -- The parent node.                                          *
 *          tag  -- The tag name of the element.                              *
 *                                                                            *
 * OUTPUT:  Return the inquired elem. Return nullptr if the inquired elem     *
 *          doesn't exist.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
XMLElement* XMLFile::GetElementByTagName(const char* tag)
{
	return ::GetElementByTagName(GetRoot(), tag);
}


/******************************************************************************
 * XMLFile::GetElementByAttrName -- Literally.                                *
 *                                                                            *
 *    This one assumes that there's only one qualified.                       *
 *                                                                            *
 * INPUT:   node -- The parent node.                                          *
 *          attr -- The attribute to compare.                                 *
 *          name -- The qualified attr name.                                  *
 *                                                                            *
 * OUTPUT:  Return the inquired elem. Return nullptr if the inquired elem     *
 *          doesn't exist.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/09 Tony : Created.                                               *
 *============================================================================*/
XMLElement* XMLFile::GetElementByAttrName(const char* attr, const char* name)
{
	return ::GetElementByAttrName(GetRoot(), attr, name);
}


/******************************************************************************
 * XMLFile::GetElementsByTagName -- Literally.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   See above                                                         *
 *                                                                            *
 * OUTPUT:  Return results.                                                *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/10 Tony : Created.                                               *
 *============================================================================*/
std::vector<XMLElement*> XMLFile::GetElementsByTagName(const char* tag)
{
	return ::GetElementsByTagName(GetRoot(), tag);
}

std::vector<XMLElement*> XMLFile::GetElementsByAttrName(const char* attr, const char* name)
{
	return ::GetElementsByAttrName(GetRoot(), attr, name);
}


/******************************************************************************
 * funcName                                                                   *
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
XMLElement* GetElementByTagName(XMLElement* node, const char* tag)
{
	if (!node)
		return nullptr;

	XMLElement* elem = node->FirstChildElement();
	XMLElement* rv;
	while (elem)
	{
		if (_STR_SAME(elem->Name(), tag))
			return elem;

		rv = GetElementByTagName(elem, tag);
		if (rv)
			return rv;

		elem = elem->NextSiblingElement();
	}

	return nullptr;
}

XMLElement* GetElementByAttrName(XMLElement* node, const char* attr, const char* name)
{
	if (!node)
		return nullptr;

	XMLElement* elem = node->FirstChildElement();
	XMLElement* rv;
	while (elem)
	{
		if (elem->Attribute(attr))
		{
			if (_STR_SAME(elem->Attribute(attr), name))
				return elem;
		}

		rv = GetElementByAttrName(elem, attr, name);
		if (rv)
			return rv;

		elem = elem->NextSiblingElement();
	}

	return nullptr;
}

std::vector<XMLElement*> GetElementsByTagName(XMLElement* node, const char* tag)
{
	std::vector<XMLElement*> results;

	_GetElementsByTagName(node, tag, results);

	return results;
}

std::vector<XMLElement*> GetElementsByAttrName(XMLElement* node, const char* attr, const char* name)
{
	std::vector<XMLElement*> results;

	_GetElementsByAttrName(node, attr, name, results);

	return results;
}

void _GetElementsByTagName(XMLElement* node, const char* tag, std::vector<XMLElement*>& results)
{
	if (!node)
		return;

	XMLElement* elem = node->FirstChildElement();
	while (elem)
	{
		if (_STR_SAME(elem->Name(), tag))
			results.push_back(elem);

		_GetElementsByTagName(elem, tag, results);

		elem = elem->NextSiblingElement();
	}
}

void _GetElementsByAttrName(XMLElement* node, const char* attr, const char* name, std::vector<XMLElement*>& results)
{
	if (!node)
		return;

	XMLElement* elem = node->FirstChildElement();
	while (elem)
	{
		if (elem->Attribute(attr))
		{
			if (_STR_SAME(elem->Attribute(attr), name))
				results.push_back(elem);
		}

		_GetElementsByAttrName(elem, attr, name, results);

		elem = elem->NextSiblingElement();
	}
}
