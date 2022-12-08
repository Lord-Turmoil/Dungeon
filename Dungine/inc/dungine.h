/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : dungine.h                                 *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 31, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide all header files of Dungine.                                     *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _DUNGINE_H_
#define _DUNGINE_H_
 

// If this is defined, there will be no log info.
// #define DGE_NO_LOG


// Common
#include "common/Common.h"
#include "common/Logger.h"


// Device
#include "device/Color.h"
#include "device/Device.h"
#include "device/Explorer.h"
#include "device/Speaker.h"
#include "device/Timer.h"


// UI module
#include "ui/AbstractInterface.h"
#include "ui/Application.h"
#include "ui/Event.h"
#include "ui/Transition.h"
#include "ui/Transitor.h"
#include "ui/Widget.h"
#include "ui/WidgetManger.h"


// Game
#include "game/AbstractComponent.h"
#include "game/Collider.h"
#include "game/GameObject.h"
#include "game/ImageObject.h"
#include "game/Scene.h"
#include "game/StandardComponent.h"


// Utility
#include "utility/Auxilliary.h"
#include "utility/DMath.h"
#include "utility/Parser.h"
#include "utility/Pipe.h"
#include "utility/QuadTree.h"
#include "utility/tinyxml.h"
#include "utility/xml.h"


// Template
#include "template/AbstractKit.h"
#include "template/AbstractLibrary.h"
#include "template/Prototype.h"
#include "template/Singleton.h"



// Static Library
#ifdef DUNGINE_DEBUG
#pragma comment(lib, "lib/Debug/dungine_b.lib")
#pragma comment(lib, "lib/Debug/tinyxml2_b.lib")
#else
#pragma comment(lib, "lib/Release/dungine.lib")
#pragma comment(lib, "lib/Release/tinyxml2.lib")
#endif

#pragma comment(lib,"lib/fmod_vc.lib")


#endif