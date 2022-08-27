/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : ComponentKit.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 26, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide base component for the game.                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _COMPONENT_KIT_H_
#define _COMPONENT_KIT_H_

#include <dungine.h>

class ComponentKit : public StandardComponentKit
{
public:
	ComponentKit() {}
	virtual ~ComponentKit() {}

	virtual AbstractComponent* LoadComponent(XMLElement* node);
};

#endif