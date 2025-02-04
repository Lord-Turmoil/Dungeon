/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : MiscLibrary.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 4, 2022                            *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For miscellaneous objects.                                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/object/MiscLibrary.h"
#include "../../inc/object/MiscKit.h"
#include "../../inc/object/Object.h"

/******************************************************************************
 * MiscLibrary::Load -- Load misc library.                                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   filename                                                          *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/04 Tony : Created.                                               *
 *============================================================================*/
bool MiscLibrary::Load(const char* filename)
{
    XMLFile file;

    file.Load(filename);

    Load(file.GetRoot());

    file.UnLoad();

    _RETURN_STATE();
}

/******************************************************************************
 * MiscLibrary::Load -- Load misc library.                                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/04 Tony : Created.                                               *
 *============================================================================*/
bool MiscLibrary::Load(XMLElement* node)
{
    /*
    **	<MiscLibrary>
    **		<ObjectName ...>...</ObjectName>
    **		<ObjectName ...>...</ObjectName>
    **		<ObjectName ...>...</ObjectName>
    **	</MiscLibrary>
    */
    const char* name = node->Name();

    _CHECK_TAG("MiscLibrary");
    _RETURN_IF_ERROR();

    MiscKit kit;
    XMLElement* elem = node->FirstChildElement();
    Object* obj;
    std::string objName;
    while (elem)
    {
        obj = kit.LoadObject(elem, objName);
        AddItem(objName, obj);

        elem = elem->NextSiblingElement();
    }

    _RETURN_STATE();
}

/******************************************************************************
 * MiscLibrary::UnLoad -- Unload all resources.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/04 Tony : Created.                                               *
 *============================================================================*/
void MiscLibrary::UnLoad()
{
    for (auto it = m_pool.begin(); it != m_pool.end(); it++)
    {
        delete it->second;
    }
    m_pool.clear();
}
