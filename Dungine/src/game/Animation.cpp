/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Animation.cpp                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 9, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide animation for game objects.                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/common/Common.h"

#include "../../inc/device/Device.h"
#include "../../inc/device/Timer.h"

#include "../../inc/game/Animation.h"

#include "../../inc/utility/Straw.h"
#include "../../inc/utility/Parser.h"
#include "../../inc/utility/tinyxml.h"



/******************************************************************************
 * MonoMotion::MonoMotion -- Constructor of the object.                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node        -- XML node.                                          *
 *          pImage      -- The complete source image.                         *
 *          srcX        -- The beginning x coordinate.                        *
 *          srcY        -- The beginning y coordinate.                        *
 *          frameNum    -- Number of frames of this motion.                   *
 *          frameWidth  -- Width of each frame.                               *
 *          frameHeight -- Height of each frame.                              *
 *                                                                            *
 * OUTPUT:  Return succeeded or not.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
MonoMotion::MonoMotion(
	IMAGE* pImage,
	int srcX,
	int srcY,
	int frameNum,
	int frameWidth,
	int frameHeight)
{
	Load(pImage, srcX, srcY, frameNum, frameWidth, frameHeight);
}


/******************************************************************************
 * MonoMotion::Load -- Load mono motion.                                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node        -- XML node.                                          *
 *          pImage      -- The complete source image.                         *
 *          srcX        -- The beginning x coordinate.                        *
 *          srcY        -- The beginning y coordinate.                        *
 *          frameNum    -- Number of frames of this motion.                   *
 *          frameWidth  -- Width of each frame.                               *
 *          frameHeight -- Height of each frame.                              *
 *                                                                            *
 * OUTPUT:  Return succeeded or not.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
bool MonoMotion::Load(
	IMAGE* pImage,
	int srcX,
	int srcY,
	int frameNum,
	int frameWidth,
	int frameHeight)
{
	Device::GetInstance()->SetTargetImage(pImage);

	m_frame.resize(frameNum);
	for (int i = 0; i < frameNum; i++)
	{
		getimage(&m_frame[i], srcX, srcY, frameWidth, frameHeight);
		srcX += frameWidth;
	}

	Device::GetInstance()->SetTargetImage();

	return true;
}


/******************************************************************************
 * Motion::Motion -- Constructor of the object.                               *
 *                                                                            *
 *    The complete image where motion will be snipped from.                   *
 *                                                                            *
 * INPUT:   node        -- XML node.                                          *
 *          pImage      -- The complete source image.                         *
 *          srcX        -- The beginning x coordinate.                        *
 *          srcY        -- The beginning y coordinate.                        *
 *          frameWidth  -- Width of each frame.                               *
 *          frameHeight -- Height of each frame.                              *
 *                                                                            *
 * OUTPUT:  Return succeeded or not.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
Motion::Motion(
	XMLElement* node,
	IMAGE* pImage,
	int frameWidth,
	int frameHeight)
{
	Load(node, pImage, frameWidth, frameHeight);
}


/******************************************************************************
 * Motion::~Motion -- Destructor of the object.                               *
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
 *   2022/06/14 Tony : Created.                                               *
 *============================================================================*/
Motion::~Motion()
{
	for (auto it = m_pMonoMotion.begin(); it != m_pMonoMotion.end(); it++)
		delete (*it);
}


/******************************************************************************
 * Motion::Load -- Load a single motion.                                      *
 *                                                                            *
 *    The complete image where motion will be snipped from.                   *
 *                                                                            *
 * INPUT:   node        -- XML node.                                          *
 *          pImage      -- The complete source image.                         *
 *          srcX        -- The beginning x coordinate.                        *
 *          srcY        -- The beginning y coordinate.                        *
 *          frameWidth  -- Width of each frame.                               *
 *          frameHeight -- Height of each frame.                              *
 *                                                                            *
 * OUTPUT:  Return succeeded or not.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
bool Motion::Load(
	XMLElement* node,
	IMAGE* pImage,
	int frameWidth,
	int frameHeight)
{
/*
**	<Motion origin="" frame-num="" frame-speed="" dir-num="" ></Motion>
*/
	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("Motion");
	_RETURN_IF_ERROR();

	Coordinate origin;
	int srcX, srcY;
	int dirNum = 0;

	_PARSE_PRIVATE_ESSENTIAL("origin", origin, name, ParseCoord);
	_PARSE_ESSENTIAL("frame-num", m_frameNum, name, 0);
	_PARSE_ESSENTIAL("frame-speed", m_frameSpeed, name, 0);
	_PARSE_ESSENTIAL("dir-num", dirNum, name, 0);
	_PARSE_ESSENTIAL("loop", m_isLoop, name, false);
	_RETURN_IF_ERROR();

	srcX = origin.x;
	srcY = origin.y;
	for (int i = 0; i < dirNum; i++)
	{
		m_pMonoMotion.push_back(new MonoMotion(pImage, srcX, srcY, m_frameNum, frameWidth, frameHeight));
		srcY += frameHeight;
	}

	return true;
}


/******************************************************************************
 * Motion::GetFrame -- Get frame.                                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   frame -- Which frame to get.                                      *
 *          dir   -- Which direction to get.                                  *
 *                                                                            *
 * OUTPUT:  Return the frame image.                                           *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
IMAGE* Motion::GetFrame(int frame, AnimDirection dir)
{
	return m_pMonoMotion[dir]->GetFrame(frame);
}


/******************************************************************************
 * MotionSet::~MotionSet -- Destructor of the object.                         *
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
 *   2022/08/13 Tony : Created.                                               *
 *============================================================================*/
MotionSet::~MotionSet()
{
	for (auto it = m_pMotion.begin(); it != m_pMotion.end(); it++)
		delete (*it);
}


/******************************************************************************
 * MotionSet::Load -- Load motion set.                                        *
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
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
bool MotionSet::Load(XMLElement* node)
{
/*
**	<Animation motion-num="" frame-width="" frame-height="" src="">
**		<Motion origin="" frame-num="" frame-speed="" dir-num="" loop=""></Motion>
**	</Animation>
*/
	const char* name = node->Name();
	const char* attr;

	_CHECK_TAG("Animation");
	_RETURN_IF_ERROR();

	int frameWidth = 0;
	int frameHeight = 0;
	const char* src = nullptr;
	_PARSE_ESSENTIAL("motion-num", m_motionNum, name, 0);
	_PARSE_ESSENTIAL("frame-width", frameWidth, name, 0);
	_PARSE_ESSENTIAL("frame-height", frameHeight, name, 0);
	_PARSE_ESSENTIAL("src", src, name, nullptr);
	_RETURN_IF_ERROR();

	IMAGE image;
	if (!FetchImage(&image, src))
	{
		LOG_ERROR(INVALID_RESOURCE, src);
		return false;
	}

	XMLElement* tag = node->FirstChildElement();
	for (int i = 0; i < m_motionNum; i++)
	{
		if (!tag)
		{
			LOG_ERROR(MISSING_CHILD_ELEMENT, name);
			return false;
		}
		m_pMotion.push_back(new Motion(tag, &image, frameWidth, frameHeight));

		tag = tag->NextSiblingElement();
	}

	_RETURN_STATE();
}


/******************************************************************************
 * MotionSet::GetMotion -- Get a motion.                                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   motion -- which motion to get.                                    *
 *                                                                            *
 * OUTPUT:  Return the motion.                                                *
 *                                                                            *
 * WARNINGS:  NO check for validation!!!                                      *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/15 Tony : Created.                                               *
 *============================================================================*/
Motion* MotionSet::GetMotion(int motion)
{
	return m_pMotion[motion];
}



/******************************************************************************
 * Animation::Animation -- Constructor of the object.                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   pMotionSet                                                        *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
Animation::Animation() :
	m_pMotionSet(nullptr),
	m_pCurMotion(nullptr),
	m_frameNum(0),
	m_frameSpeed(0),
	m_isLoop(false),
	m_elapsedTime(0),
	m_curMotion(0),
	m_curFrame(0),
	m_curDir((AnimDirection)(0)),
	m_isOver(true)
{
}

Animation::Animation(MotionSet* pMotionSet) : m_pMotionSet(pMotionSet)
{
	Reset();
}


/******************************************************************************
 * Animation::~Animation -- Destructor of the object.                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  No need to delete motionset.                                    *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
Animation::~Animation()
{
}


/******************************************************************************
 * Animation::Initialize -- Initialize animation.                             *
 *                                                                            *
 *    This attaches a motionset to the animaion.                              *
 *                                                                            *
 * INPUT:   pMotionSet                                                        *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void Animation::Initialize(MotionSet* pMotionSet)
{
	m_pMotionSet = pMotionSet;
	Reset();
}


/******************************************************************************
 * Animation::Reset -- Reset to default motion and frame.                     *
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
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void Animation::Reset()
{
	m_pCurMotion = m_pMotionSet->GetMotion(0);
	m_frameNum = m_pCurMotion->m_frameNum;
	m_frameSpeed = m_pCurMotion->m_frameSpeed;
	m_isLoop = m_pCurMotion->m_isLoop;

	m_curMotion = 0;
	m_curFrame = 0;
	m_curDir = (AnimDirection)(0);

	m_elapsedTime = 0;

	m_isOver = false;
}


/******************************************************************************
 * Animation::Update -- Update animation on each frame.                       *
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
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void Animation::Update()
{
	if (m_isOver)
		return;

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	if (m_elapsedTime > m_frameSpeed)
	{
		m_curFrame++;
		if (m_curFrame >= m_frameNum)
		{
			if (m_isLoop)
				m_curFrame = 0;
			else
			{
				m_curFrame = m_frameNum - 1;
				m_isOver = true;
			}
		}
		m_elapsedTime = 0;
	}
}


/******************************************************************************
 * Animation::GetFrame -- Get current frame.                                  *
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
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
IMAGE* Animation::GetFrame()
{
	return m_pCurMotion->GetFrame(m_curFrame, m_curDir);
}


/******************************************************************************
 * Animation::SetMotion -- Set current motion.                                *
 *                                                                            *
 *    Direction doesn't need to update?                                       *
 *                                                                            *
 * INPUT:   motion                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
void Animation::SetMotion(int motion)
{
	if (motion == m_curMotion)
		return;

	m_pCurMotion = m_pMotionSet->GetMotion(motion);
	m_frameNum = m_pCurMotion->m_frameNum;
	m_frameSpeed = m_pCurMotion->m_frameSpeed;
	m_isLoop = m_pCurMotion->m_isLoop;

	m_curMotion = motion;
	m_curFrame = 0;

	m_elapsedTime = 0;

	m_isOver = false;
}