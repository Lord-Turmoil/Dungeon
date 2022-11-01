/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Macros.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 3, 2022                               *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   All macros.                                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _MACROS_H_
#define _MACROS_H_

#define _DUNGEON_ENGINE_

/********************************************************************
** Debug and log info.
*/
#ifdef _DEBUG
#define DUNGINE_DEBUG
#endif

/********************************************************************
** Project version info.
*/
// #define DGE_INTERNAL_VERSION
#define DGE_RELEASE_VERSION


#ifdef DGE_RELEASE_VERSION
#define DUNGINE_NO_LOG
#else
#define DGE_SHOW_CONSOLE
#endif

// #define DGE_EXTRA_WARNING

#ifdef DGE_SHOW_CONSOLE
#define DGE_ENABLE_CONSOLE_LOG
#endif


/********************************************************************
** If this is defined, then the delay will be more accurate but may
** take more CPU resource.
*/
// #define HIGH_PRECISION_DELAY

#define NO_FPS


// If this is defined, then roundrect trigger will perform loose check.
// #define LOOSE_ROUND_RECT_CHECK

// #define DGE_TRACK_RESOURCE

/*
** warning C4390 : ';' : empty controlled statement found; is this the intent ?
*/
#pragma warning(disable: 4390)

#endif