/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : IndicatorComponent.h                      *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 16, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   To indicate target direction.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _INDICATOR_COMPONENT_H_
#define _INDICATOR_COMPONENT_H_

#include <dungine.h>


enum IndicatorType
{
	IND_CRATE = 0,
	IND_ENEMY,

	IND_NONE
};

/********************************************************************
** Indicator will indicate the direction of the nearest enemy, or
** the nearest crate. Not that good. :[ Can only be used on hero.
*/
class IndicatorComponent : public AbstractComponent
{
public:
	IndicatorComponent(int updateOrder = 0) : AbstractComponent(updateOrder),
		m_type(IND_NONE), m_dist(INF_DOUBLE), m_images{ nullptr, nullptr }
	{
		m_symbol.SetStatic(true);
	}
	virtual ~IndicatorComponent();

	static const char* StaticName() { return "Indicator"; }
	virtual const char* Name() { return StaticName(); }

	virtual IndicatorComponent* Clone() const;
	virtual void Clone(IndicatorComponent* clone) const {}

	virtual bool Load(XMLElement* node);

	virtual void Update(Event* evnt);

public:
	void ClearTarget() { m_type = IND_NONE; }
	void SetTarget(const Coordinate& target, IndicatorType type);

private:
	IndicatorType m_type;	// Indicator's current type.

	Coordinate m_coord;		// Indicator's center coordinate.
	Coordinate m_offset;
	Coordinate m_target;	// The target to point at.
	double m_dist;			// Current distance to target.

	Symbol m_symbol;
	ImageObject* m_images[2];	// store two types of indicators
};

#endif