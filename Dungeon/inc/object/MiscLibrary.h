/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : MiscLibrary.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 4, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For miscellaneous objects.                                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _MISC_LIBRARY_H_
#define _MISC_LIBRARY_H_

#include <dungine.h>

class Object;

class MiscLibrary : private AbstractLibrary<Object>, public Singleton<MiscLibrary>
{
	friend class Singleton<MiscLibrary>;
public:
	bool Load(const char* filename);

	virtual bool Load(XMLElement* node);
	virtual void UnLoad();

	// GetObject is defined in wingdi.h...
	template<typename T>
	T* GetMiscObject(const std::string& name)
	{
		return static_cast<T*>(GetItem(name));
	}

private:

};
#endif
