/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Pipe.h                                    *
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

#ifndef _PIPE_H_
#define _PIPE_H_

#include <easyx.h>

#include "xml.h"


/********************************************************************
** This is the base class for all classes dealing with XML data
*/
class Pipe
{
public:
	Pipe() : m_filename(nullptr) {}
	~Pipe() {}

	bool Link(const char* filename);
	
	virtual bool Load() = 0;
	virtual bool Save() = 0;

protected:
	bool _IsLinked();
	bool _LoadFile(XMLFile& file);

	void _LoadEntry(XMLFile& file, int* val, const char* tag);
	void _LoadEntry(XMLFile& file, double* val, const char* tag);
	void _LoadEntry(XMLFile& file, bool* val, const char* tag);
	void _LoadEntry(XMLFile& file, std::string* val, const char* tag);
	void _LoadEntry(XMLFile& file, COLORREF* val, const char* tag);

	void _SaveEntry(XMLFile& file, int val, const char* tag);
	void _SaveEntry(XMLFile& file, double val, const char* tag);
	void _SaveEntry(XMLFile& file, bool val, const char* tag);
	void _SaveEntry(XMLFile& file, const std::string& val, const char* tag);
	void _SaveEntry(XMLFile& file, COLORREF val, const char* tag);

private:
	const char* m_filename;
};


/********************************************************************
** I'm not very familiar with try-cath machanism, so... give it a
** try? :)
*/
class EntryError
{
public:
	EntryError(const char* entry) : m_entry(entry) {}

	const char* what() const { return m_entry; }

private:
	const char* m_entry;
};

#endif