/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : ComponentKit.cpp                          *
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

#include "../../inc/object/ComponentKit.h"
#include "../../inc/object/Component.h"


/******************************************************************************
 * ComponentKit::LoadComponent -- Load component with custom components.      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return the loaded component.                                      *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
AbstractComponent* ComponentKit::LoadComponent(XMLElement* node)
{
	if (!node)
		return nullptr;

	const char* name = node->Name();
	AbstractComponent* rv = nullptr;

	if (_STR_SAME(name, "WeaponSlot"))
		rv = new WeaponComponent();
	else if (_STR_SAME(name, "Sound"))
		rv = new SoundComponent();
	else if (_STR_SAME(name, "Indicator"))
		rv = new IndicatorComponent();

	if (rv)
	{
		rv->Load(node);
		return rv;
	}
	else
		return StandardComponentKit::LoadComponent(node);
}