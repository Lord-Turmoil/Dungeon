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

#ifndef _XML_H_
#define _XML_H_

#include <vector>

#include "tinyxml.h"


/********************************************************************
** There is a embarrassing situation that... Windows API supports
** wide char while tinyxml only support char... :(
*/
class XMLFile
{
public:
	XMLFile() : m_filename(nullptr), m_isLoaded(false) {}
	~XMLFile();

	/*
	** Load the file from memory. This will close the previous file.
	*/
	bool Load(const char* filename);
	bool Save();
	bool Save(const char* filename);

	/*
	** Unload the current file.
	*/
	bool UnLoad();

	/*
	** Get the root, or a specific node of the XML file.
	*/
	XMLElement* GetRoot();

	XMLElement* GetElementByTagName(const char* tag);
	XMLElement* GetElementByAttrName(const char* attr, const char* name);

	std::vector<XMLElement*> GetElementsByTagName(const char* tag);
	std::vector<XMLElement*> GetElementsByAttrName(const char* attr, const char* name);

public:
	XMLDoc& Doc() { return m_doc; }
	bool IsLoaded() const { return m_isLoaded; }

private:
	XMLDoc m_doc;

	const char* m_filename;

	/*
	** The straw's current state, whether a file has been loaded
	** or not.
	*/
	bool m_isLoaded;
};

XMLElement* GetElementByTagName(XMLElement* node, const char* tag);
XMLElement* GetElementByAttrName(XMLElement* node, const char* attr, const char* name);
std::vector<XMLElement*> GetElementsByTagName(XMLElement* node, const char* tag);
std::vector<XMLElement*> GetElementsByAttrName(XMLElement* node, const char* attr, const char* name);
void _GetElementsByTagName(XMLElement* node, const char* tag, std::vector<XMLElement*>& results);
void _GetElementsByAttrName(XMLElement* node, const char* attr, const char* name, std::vector<XMLElement*>& results);

#endif