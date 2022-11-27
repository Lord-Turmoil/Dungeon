/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Defines.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 14, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Some defines...                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _DEFINES_H_
#define _DEFINES_H_

#ifdef _DEBUG
#define DUNGEON_DEBUG
#endif

#include <dungine.h>

#define BIT(bit) ((1) << (bit))

enum Layer : int
{
	LAYER_FLOOR		= LAYER_BACK,
	LAYER_BRICK		= LAYER_NORMAL,
	LAYER_FIGURE	= LAYER_NORMAL,
	LAYER_WEAPON	= LAYER_NORMAL,
	LAYER_BULLET	= LAYER_MIDDLE,
	LAYER_BUFF		= LAYER_MIDDLE + 1,
	LAYER_MINI_MAP	= LAYER_HIGH
};


enum CollisionID : int
{
	COLL_ID_AIR = 0,
	COLL_ID_WALL,
	COLL_ID_WALL_OPEN,
	COLL_ID_HERO_BULLET,
	COLL_ID_HERO_FLAME,
	COLL_ID_ENEMY_BULLET,
	COLL_ID_ENEMY_FLAME,
	COLL_ID_HERO,
	COLL_ID_SPECTER,
	COLL_ID_ENEMY,
	
	COLL_ID_NUM
};

const CollisionType COLLISION_MAP[COLL_ID_NUM][COLL_ID_NUM] = {
	{ COLL_NONE,	COLL_NONE,		COLL_NONE,		COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,		COLL_NONE,	COLL_NONE		},
	{ COLL_NONE,	COLL_NONE,		COLL_NONE,		COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,		COLL_NONE,	COLL_NONE		},	
	{ COLL_NONE,	COLL_NONE,		COLL_NONE,		COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,		COLL_NONE,	COLL_NONE		},
	{ COLL_NONE,	COLL_INELASTIC,	COLL_INELASTIC,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,		COLL_NONE,	COLL_ELASTIC	},
	{ COLL_NONE,	COLL_CHECK,		COLL_CHECK,		COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,		COLL_NONE,	COLL_CHECK		},
	{ COLL_NONE,	COLL_INELASTIC,	COLL_INELASTIC,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_ELASTIC,	COLL_NONE,	COLL_NONE		},
	{ COLL_NONE,	COLL_CHECK,		COLL_CHECK,		COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_CHECK,		COLL_NONE,	COLL_NONE		},
	{ COLL_NONE,	COLL_ELASTIC,	COLL_NONE,		COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_ELASTIC,	COLL_NONE,	COLL_ELASTIC	},
	{ COLL_NONE,	COLL_INELASTIC,	COLL_NONE,		COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,		COLL_NONE,	COLL_NONE		},
	{ COLL_NONE,	COLL_ELASTIC,	COLL_INELASTIC,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_NONE,	COLL_ELASTIC,	COLL_NONE,	COLL_ELASTIC	} };


enum DifficultyLevel : int
{
	EASY = 0,
	NORMAL,
	HARD,
	INSANE,

	DIFFICULTY_NUM
};

const char* const DIFFICULTY_TEXT[DIFFICULTY_NUM] = {
	"Easy",
	"Normal",
	"Hard",
	"Insane" };

const char* const BOOLEAN_TEXT[2] = {
	"False",
	"True" };

const char* const YES_NO_TEXT[2] = {
	"No",
	"Yes" };

/******************************************************************************
*                             File Sturcture                                  *
* --------------------------------------------------------------------------- *
* res\                      -- Game resources.                                *
*     images\		        -- Images.                                        *
*     sounds\		        -- Sounds.                                        *
*     fonts\                -- Fonts.                                         *
*     data\				    -- Game data.                                     *
*         chapter\          -- Chapter info.                                  *
*             Chapter0.xml                                                    *
*             ChapterX.xml                                                    *
*         ui\               -- UI layout.                                     *
*             Logo.xml      -- Logo interface.                                *
*             Xxxx.xml      -- Xxxx interface.                                *
*         lib\              -- Object library reference.                      *
*             Hero.xml      -- Figure's resource property.                    *
*             Weapon.xml    -- Weapon's resource property.                    *
*             Bullet.xml    -- Bullet's resource property.                    *
*             Misc.xml      -- Miscellaneous objects.                         *
*         Settings.xml      -- Game settings.                                 *
*         Resource.xml      -- Resource configuration.                        *
* screenshot\               -- Screenshots.                                   *
* log.txt                   -- Dungine's log info.                            *
*******************************************************************************/
const char HERO_XML[]			= R"(res\data\lib\Hero.xml)";
const char WEAPON_XML[]			= R"(res\data\lib\Weapon.xml)";
const char BULLET_XML[]			= R"(res\data\lib\Bullet.xml)";
const char MISC_XML[]			= R"(res\data\lib\Misc.xml)";		// miscellaneous
const char SETTINGS_XML[]		= R"(res\data\Settings.xml)";
const char RESOURCE_XML[]		= R"(res\data\Resource.xml)";

const char CHAPTER_DIRECTORY[]	= R"(res\data\chapter\)";
const char UI_DIRECTORY[]		= R"(res\data\ui\)";

const wchar_t FONT_PATH[] = LR"(res\fonts\8bitoperator_jve.ttf)";
const wchar_t FONT_NAME[] = L"8bitoperator JVE Regular";

#endif