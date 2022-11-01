/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : AbstractKit.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 22, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Base class of factory for object creation.                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ABSTRACT_KIT_H_
#define _ABSTRACT_KIT_H_

template<typename ObjType>
class AbstractKit
{
public:
	AbstractKit() {}
	virtual ~AbstractKit() {}

	/*
	** The constructor of the object need to know the parent scene,
	** but the scene is not a prototype. :(
	*/
	virtual ObjType* LoadObject(XMLElement* node) = 0;
};

#endif