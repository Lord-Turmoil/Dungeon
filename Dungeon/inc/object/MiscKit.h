/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : MiscKit.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 4, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Factory for miscellaneous objects.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _MISC_KIT_H_
#define _MISC_KIT_H_

#include <dungine.h>

class Object;

class MiscKit : public AbstractKit<Object>
{
public:
	MiscKit() {}
	virtual ~MiscKit() {}

	// Well, I think not all objects have a name. :(
	Object* LoadObject(XMLElement* node, std::string& objName);
	virtual Object* LoadObject(XMLElement* node);
};

#endif