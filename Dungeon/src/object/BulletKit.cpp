/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BulletKit.pp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 22, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Factory of bullets.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/Bullet.h"
#include "../../inc/object/BulletKit.h"


 /******************************************************************************
 * BulletKit::LoadObject -- Load bullet.                                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *          pScene -- Scene the object belongs to.                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/22 Tony : Created.                                               *
 *============================================================================*/
Bullet* BulletKit::LoadObject(XMLElement* node)
{
	const char* name = node->Name();
	Bullet* bullet = nullptr;

	if (_STR_SAME(name, "Bullet"))
		bullet = new Bullet(nullptr);

	if (bullet)
		bullet->Load(node);
	else
		LOG_ERROR(UNKNOWN_TAG, name);

	return bullet;
}