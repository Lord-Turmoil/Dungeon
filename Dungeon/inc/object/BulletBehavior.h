/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BulletBehavior.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Behavior of basic bullets.                                               *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _BULLET_BEHAVIOR_H_
#define _BULLET_BEHAVIOR_H_

#include <dungine.h>


class BulletBehavior : public Behavior
{
public:
	BulletBehavior() {}
	virtual ~BulletBehavior() {}

	virtual const char* Name() const = 0;

	virtual BulletBehavior* Clone() const = 0;
	virtual void Clone(BulletBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	enum BulletAnimTag
	{
		BULLET_ANIM_FLY = 0,
		BULLET_ANIM_EXPLODE,
		BULLET_ANIM_CORRUPT,
	};
};

class BulletNone : public BulletBehavior
{
public:
	virtual const char* Name() const { return "None"; }

	virtual BulletNone* Clone() const;
	virtual void Clone(BulletNone* clone) const {}

	virtual void Update(Event* evnt) {}
};

class BulletFly : public BulletBehavior
{
public:
	virtual const char* Name() const { return "Fly"; }

	virtual BulletFly* Clone() const;
	virtual void Clone(BulletFly* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();

protected:
	// Damage game objects.
	virtual bool _Explode(GameObject* obj);
	virtual void _AdjustDirection();

	double m_dist;	// distance covered
	std::vector<GameObject*> m_candidates;
};

class BulletExplode : public BulletBehavior
{
public:
	virtual const char* Name() const { return "Explode"; }

	virtual BulletExplode* Clone() const;
	virtual void Clone(BulletExplode* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

class BulletCorrupt : public BulletBehavior
{
public:
	virtual const char* Name() const { return "Corrupt"; }

	virtual BulletCorrupt* Clone() const;
	virtual void Clone(BulletCorrupt* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};


/*
** I'd like to name all objects' invalid state "Perish",
** since "Die" may be a valid behavior.
*/
class BulletPerish : public BulletBehavior
{
public:
	virtual const char* Name() const { return "Perish"; }

	virtual BulletPerish* Clone() const;
	virtual void Clone(BulletPerish* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
};

#endif