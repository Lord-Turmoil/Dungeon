/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Transition.h                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 7, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For the transition effect of the widgets. Similar to the CSS trasform    *
 *   style.                                                                   *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _TRANSITION_H_
#define _TRANSITION_H_

#include "../common/Common.h"


class VisualWidget;
class XMLElement;


enum class TransitionType
{
	TRANS_ALPHA,
	TRANS_COORD,
	TRANS_SCALE,

	TRANS_NUM
};

/*
** ease-in-ease-out can be realized by put ease-in and 
** ease-out together manually. :P
*/
enum class TransitionStyle
{
	TRANS_STYLE_EASE_IN,
	TRANS_STYLE_EASE_OUT,
	TRANS_STYLE_LINEAR,

	TRANS_STYLE_NUM
};


/********************************************************************
** The transition style of the widget. For now, it only support
** linear transition. :(
** Warning, m_duration may cause divided by zero!
** Transition must be reset before applied to widget. However, it is
** done automatically after loaded from xml.
** Unfortunately, it not wise to use template here. :(
*/
class Transition
{
public:
	Transition(TransitionType type) : m_type(type), m_style(TransitionStyle::TRANS_STYLE_LINEAR),
		m_duration(0), m_delay(0), m_elapsedTime(0), m_pWidget(nullptr),
		m_loop(false), m_reverse(false), m_isReversed(false),
		m_isWaiting(false), m_isOver(true) {}
	virtual ~Transition() {}

	TransitionType Type() const { return m_type; }

	void Reset();
	void Terminate();
	void Stop();

	void Update();
	virtual bool Load(XMLElement* node);

	Transition* SetWidget(VisualWidget* widget)
	{
		m_pWidget = widget;
		return this;
	}
	VisualWidget* GetWidget() const { return m_pWidget; }

	Transition* SetDuration(clock_t duration)
	{
		m_duration = max(duration, 0);
		if (m_duration == 0)
			m_isOver = true;
		return this;
	}
	clock_t GetDuration() const { return m_duration; }

	Transition* SetDelay(clock_t delay)
	{
		m_delay = max(delay, 0);
		if (m_delay == 0)
			m_isWaiting = false;
		return this;
	}
	clock_t GetDelay(clock_t delay) { return m_delay; }

	virtual Transition* SetStyle(TransitionStyle style) = 0;

	Transition* Loop(bool loop)
	{
		m_loop = loop;
		return this;
	}

	Transition* Reverse(bool reverse)
	{
		m_reverse = reverse;
		return this;
	}

protected:
	virtual void _Update() = 0;
	virtual void _Reset() = 0;
	virtual void _Terminate() = 0;
	virtual void _Finalize() = 0;
	virtual void _AdjustProperty(XMLElement* node) = 0;

	/*
	** This will reverse the process of the transition.
	*/
	virtual void _Reverse();

	TransitionType m_type;
	TransitionStyle m_style;

	/*
	** The total time of the transition, delay time not included.
	** All time here is in milliseconds.
	*/
	clock_t m_duration;

	/*
	** How many milliseconds there are before start.
	*/
	clock_t m_delay;

	/*
	** How much time passed after start. It will be reset to 0
	** after delay.
	*/
	clock_t m_elapsedTime;

	/*
	** The widget it attatched to.
	*/
	VisualWidget* m_pWidget;

	/*
	** Whether this transition should perform again and again.
	*/
	bool m_loop : 1;

	/*
	** Whether reverse the direction after one time, and if
	** current is reversed.
	*/
	bool m_reverse : 1;
	bool m_isReversed : 1;

	/*
	** Since the transition may start after a delay, so it might
	** be waiting to begin.
	*/
	bool m_isWaiting : 1;

	/*
	** Whether the transition is over or not.
	*/
	bool m_isOver : 1;
};


/********************************************************************
** This is for the widget's coordinate transition.
*/
class CoordTransition : public Transition
{
public:
	CoordTransition() : Transition(TransitionType::TRANS_COORD) {}
	~CoordTransition() {}

	virtual bool Load(XMLElement* node);

	virtual CoordTransition* SetStyle(TransitionStyle style);

	CoordTransition* SetBeginCoord(const Coordinate& coord)
	{
		m_begin = coord;
		return this;
	}
	const Coordinate& GetBeginCoord() const { return m_begin; }

	CoordTransition* SetEndCoord(const Coordinate& coord)
	{
		m_end = coord;
		return this;
	}
	const Coordinate& GetEndCoord() const { return m_end; }

private:
	virtual void _Update();
	virtual void _Reset();
	virtual void _Terminate();

	virtual void _Finalize();
	virtual void _AdjustProperty(XMLElement* node);

	Coordinate m_begin;
	Coordinate m_end;
	Coordinate (*m_blender)(Coordinate, Coordinate, double);
};


/********************************************************************
** This is for the widget's alpha transition.
** 2022/06/01 Tony: BUG FOUND
**   This misfunction when sub-drawer exists. Parent drawer won't be
**   affected because it is drawn first. However, the sub-drawer will
**   mis-choose the parent drawer's alpha color as background color,
**   so... it will get unexpected color result. :(
** 2022/06/02 Tony: BUG FIXED
**   ... The macro for alpha color conversion is incorrect... :P
*/
class AlphaTransition : public Transition
{
public:
	AlphaTransition() : Transition(TransitionType::TRANS_ALPHA) {}
	~AlphaTransition() {}

	virtual bool Load(XMLElement* node);

	virtual AlphaTransition* SetStyle(TransitionStyle style);

	AlphaTransition* SetBeginAlpha(int alpha)
	{
		m_begin = alpha;
		return this;
	}
	int GetBeginAlpha() const { return m_begin; }

	AlphaTransition* SetEndAlpha(int alpha)
	{
		m_end = alpha;
		return this;
	}
	int GetEndAlpha() const { return m_end; }

private:
	virtual void _Update();
	virtual void _Reset();
	virtual void _Terminate();

	virtual void _Finalize();
	virtual void _AdjustProperty(XMLElement* node) {}

	/*
	** These two are alpha info. :)
	*/
	int m_begin;
	int m_end;
	int (*m_blender)(int, int, double);
};


/********************************************************************
** This is for the widgets' scale transition.
** Hmm... Since trigger doesn't support this for now, don't add it
** to widgets that have MouseTrigger. :(
*/
class ScaleTransition : public Transition
{
public:
	ScaleTransition() : Transition(TransitionType::TRANS_SCALE) {}
	~ScaleTransition() {}

	virtual bool Load(XMLElement* node);

	virtual ScaleTransition* SetStyle(TransitionStyle style);

	ScaleTransition* SetBeginScale(double scale)
	{
		m_begin = scale;
		return this;
	}
	ScaleTransition* SetEndScale(double scale)
	{
		m_end = scale;
		return this;
	}
	double GetBeginScale() const { return m_begin; }
	double GetEndScale() const { return m_end; }

private:
	virtual void _Update();
	virtual void _Reset();
	virtual void _Terminate();

	virtual void _Finalize();
	virtual void _AdjustProperty(XMLElement* node) {}

	/*
	** These two are alpha info. :)
	*/
	double m_begin;
	double m_end;
	double (*m_blender)(double, double, double);
};

Transition* LoadTransition(XMLElement* node);

#endif