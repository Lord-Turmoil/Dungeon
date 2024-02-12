/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Camera.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 28, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The camera in game.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <vector>

#include "../common/Common.h"
#include "GameObject.h"


const double CAM_FAST = 0.1;
const double CAM_MEDIUM = 0.5;
const double CAM_SLOW = 0.9;


class Device;

/********************************************************************
** Although camera can derive from GameObject, I think it is not that
** necessary here.
*/
class Camera
{
public:
	Camera() : m_lag(CAM_SLOW), m_updated(true), m_device(nullptr) {}
	~Camera() {}

	void Initialize(const Rect& rect);
	
	void Update();

	void Follow(const Coordinate& target);
	void Focus(const Coordinate& target);

	void Capture(GameObject* obj);
	void Capture(Symbol* symbol);

	bool InRange(const Rect& rect);

	Coordinate AbsoluteToRelative(const Coordinate& coord);
	Coordinate RelativeToAbsolute(const Coordinate& coord);

public:
	void Draw() { m_updated = false; }

	void SetLag(double lag) { m_lag = lag; }
	double GetLag() const { return m_lag; }

	Rect GetArea() const { return m_area; }

protected:
	Rect m_filmArea;		// fixed on initialization
	Rect m_area;			// current area.
	
	Coordinate m_center;	// current center of curArea
	Coordinate m_target;	// current target

	/*
	** This is determined after Update, and is the same
	** to all objects.
	*/
	Coordinate m_offset;

	double m_lag;

	/*
	** 2022/08/06 TS:
	** This is to prevent double-capture, which causes the
	** symbol to be translated multiple times. :(
	*/
	bool m_updated;

	std::vector<GameObject*> m_sightedObjects;

	Device* m_device;
};

#endif