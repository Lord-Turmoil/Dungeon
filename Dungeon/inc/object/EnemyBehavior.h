/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyBehavior.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 27, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Behaviors of enemies.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENEMY_BEHAVIOR_H_
#define _ENEMY_BEHAVIOR_H_

#include <dungine.h>

// Gap of decision.
const clock_t DECISION_GAP = 1000L;

const double DIST_MARGIN = 5.0;

class Enemy;
class Arena;
class Dungeon;

/********************************************************************
** Enemy behaviors:
**   Init    -- Play initialization animation.
**   Patrol  -- Quite far from the hero.	 (Harmless)
**   Idle    -- Sometimes may take a break.  (Harmless)
**   Attack  -- Spotted the hero and engage. (Aggressive)
**   Retreat -- Too close to the hero.       (Aggressive)
**   Dead    -- Play dead animation.
**   Perish  -- Become invalid.
**
** Some behaviors may change slightly with different difficulty
** level.
*/

class EnemyBehavior : public Behavior
{
public:
    EnemyBehavior() : m_elapsedTime(0L)
    {
    }

    ~EnemyBehavior() override
    {
    }

    /*
     ** Get the name of the behavior. I think... C-Style string is
     ** enough.
     */
    const char* Name() const override = 0;

    EnemyBehavior* Clone() const override = 0;
    virtual void Clone(EnemyBehavior* clone) const;

    bool Load(XMLElement* node) override
    {
        return true;
    }

    void Update(Event* evnt) override = 0;

    void OnEnter() override
    {
    }

    void OnExit() override
    {
    }

protected:
    bool _Alert();
    void _Move(const Vector& dir);
    void _Evade(int level);

    virtual void _Collide();
    virtual void _OnDefeat(Dungeon* dungeon, Enemy* enemy);

    clock_t m_elapsedTime;

protected:
    enum EnemyAnimTag
    {
        ENEMY_ANIM_INIT = 0,
        ENEMY_ANIM_IDLE,
        ENEMY_ANIM_MOVE,
        ENEMY_ANIM_ATTACK,
        ENEMY_ANIM_DEAD
    };
};

class EnemyInit : public EnemyBehavior
{
public:
    const char* Name() const override
    {
        return "Init";
    }

    EnemyInit* Clone() const override;
    virtual void Clone(EnemyInit* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;
};

class EnemyIdle : public EnemyBehavior
{
public:
    const char* Name() const override
    {
        return "Idle";
    }

    EnemyIdle* Clone() const override;
    virtual void Clone(EnemyIdle* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class EnemyEngage : public EnemyBehavior
{
public:
    const char* Name() const override
    {
        return "Engage";
    }

    EnemyEngage* Clone() const override;
    virtual void Clone(EnemyEngage* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class EnemyAttack : public EnemyBehavior
{
public:
    const char* Name() const override
    {
        return "Attack";
    }

    EnemyAttack* Clone() const override;
    virtual void Clone(EnemyAttack* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class EnemyRest : public EnemyBehavior
{
public:
    const char* Name() const override
    {
        return "Rest";
    }

    EnemyRest* Clone() const override;
    virtual void Clone(EnemyRest* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    clock_t m_duration;
};

class EnemyDead : public EnemyBehavior
{
public:
    const char* Name() const override
    {
        return "Dead";
    }

    EnemyDead* Clone() const override;
    virtual void Clone(EnemyDead* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class EnemyPerish : public EnemyBehavior
{
public:
    const char* Name() const override
    {
        return "Perish";
    }

    EnemyPerish* Clone() const override;

    virtual void Clone(EnemyPerish* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }

    void OnEnter() override;
};

#endif
