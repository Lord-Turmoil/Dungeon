/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Coordinate.cpp                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 3, 2022                               *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Some operation on Coordinate.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/common/Common.h"
#include "../../inc/utility/Parser.h"


/******************************************************************************
 * ParseCoord -- Parse coord from a string.                                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   val       -- The target coordinate.                               *
 *          attribute -- The C-style string.                                  *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/03 Tony : Created.                                               *
 *============================================================================*/
bool ParseCoord(void* val, const char* attribute)
{
	if (!attribute)
		return false;
	Coordinate* coord = (Coordinate*)val;

	if (!ParseInt(&(coord->y), ParseInt(&(coord->x), attribute)))
		return false;

	return true;
}