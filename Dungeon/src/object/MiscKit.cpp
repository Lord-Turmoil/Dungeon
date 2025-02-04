/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : MiscKit.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 4, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Factory for miscellaneous objects.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/MiscKit.h"

#include "../../inc/object/Buff.h"
#include "../../inc/object/Crate.h"
#include "../../inc/object/Portal.h"
#include "../../inc/object/Stand.h"

Object* MiscKit::LoadObject(XMLElement* node)
{
    const char* name = node->Name();
    Object* obj = nullptr;

    if (_STR_SAME(name, "Portal"))
    {
        obj = new Portal(nullptr);
    }

    if (obj)
    {
        obj->Load(node);
    }
    else
    {
        LOG_ERROR(UNKNOWN_TAG, name);
    }

    return obj;
}

Object* MiscKit::LoadObject(XMLElement* node, std::string& objName)
{
    const char* name = node->Name();
    Object* obj = nullptr;

    if (_STR_SAME(name, "Portal"))
    {
        obj = new Portal(nullptr);
    }
    else if (_STR_SAME(name, "Energy"))
    {
        obj = new Energy(nullptr);
    }
    else if (_STR_SAME(name, "Coin"))
    {
        obj = new Coin(nullptr);
    }
    else if (_STR_SAME(name, "HPDrug"))
    {
        obj = new HPDrug(nullptr);
    }
    else if (_STR_SAME(name, "MPDrug"))
    {
        obj = new MPDrug(nullptr);
    }
    else if (_STR_SAME(name, "Crate"))
    {
        obj = new Crate(nullptr);
    }
    else if (_STR_SAME(name, "Stand"))
    {
        obj = new Stand(nullptr);
    }

    if (obj)
    {
        obj->Load(node);
        objName = name;
    }
    else
    {
        LOG_ERROR(UNKNOWN_TAG, name);
    }

    return obj;
}
