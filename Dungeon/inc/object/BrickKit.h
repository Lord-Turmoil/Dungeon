/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BrickKit.h                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 18, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Use factory pattern to generate bricks.                                  *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _BRICK_KIT_H_
#define _BRICK_KIT_H_

#include <vector>

#include "Brick.h"

class Dungeon;

/********************************************************************
** Used to generate cloned bricks.
** Later, it can be upgraded to generate various bricks.
*/
class BrickKit
{
public:
    BrickKit()
    {
    }

    ~BrickKit();

    bool Load(XMLElement* node);
    void UnLoad();

    Wall* GetWall();
    Gate* GetGate();
    IMAGE* GetFloorImage();

private:
    bool _LoadWall(XMLElement* node);
    bool _LoadGate(XMLElement* node);
    bool _LoadFloorImage(XMLElement* node);

private:
    // Some resources as prototype.
    std::vector<Wall*> m_walls;
    std::vector<Gate*> m_gates;
    std::vector<ImageObject*> m_floors;
};

#endif
