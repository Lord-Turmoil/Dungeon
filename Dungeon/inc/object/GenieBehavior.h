/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GenieBehavior.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 19, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Genie's behavior.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _GENIE_BEHAVIOR_H_
#define _GENIE_BEHAVIOR_H_

#include "EnemyBehavior.h"

/********************************************************************
** Genie's behaviors.
** Init    -- Need take care of weapon.
** Idle    -- To Patrol or Engage.
** Patrol  -- To Idle or Engage.
** Engage  -- To Attack or Retreat.
** Retreat -- To Patrol.
** Attack  -- To Rest.
** Rest    -- Need take care of weapon.
** Dead    -- Need take care of weapon.
** Perish  -- The same as enemy.
*/
class GenieInit : public EnemyInit
{
public:
    GenieInit* Clone() const override;
    virtual void Clone(GenieInit* clone) const;

    void OnEnter() override;
    void OnExit() override;
};

class GenieIdle : public EnemyIdle
{
public:
    GenieIdle* Clone() const override;
    virtual void Clone(GenieIdle* clone) const;

    void Update(Event* evnt) override;
};

class GeniePatrol : public EnemyBehavior
{
public:
    const char* Name() const override
    {
        return "Patrol";
    }

    GeniePatrol* Clone() const override;
    virtual void Clone(GeniePatrol* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;

private:
    Coordinate m_target;
};

class GenieEngage : public EnemyEngage
{
public:
    GenieEngage* Clone() const override;
    virtual void Clone(GenieEngage* clone) const;

    void Update(Event* evnt) override;
};

class GenieRetreat : public EnemyBehavior
{
public:
    const char* Name() const override
    {
        return "Retreat";
    }

    GenieRetreat* Clone() const override;
    virtual void Clone(GenieRetreat* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class GenieAttack : public EnemyAttack
{
public:
    GenieAttack* Clone() const override;
    virtual void Clone(GenieAttack* clone) const;

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;
};

class GenieRest : public EnemyRest
{
public:
    GenieRest* Clone() const override;
    virtual void Clone(GenieRest* clone) const;

    void Update(Event* evnt) override;
};

class GenieDead : public EnemyDead
{
public:
    GenieDead* Clone() const override;
    virtual void Clone(GenieDead* clone) const;

    void OnEnter() override;
};

#endif
