/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : EnemyState.h                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 28, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   State of enemy.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ENEMY_STATE_H_
#define _ENEMY_STATE_H_

#include <dungine.h>

/********************************************************************
** Enemy's state:
**   None
**   Hurt
**   Skill
*/
class EnemyState : public State
{
public:
    EnemyState()
    {
    }

    ~EnemyState() override
    {
    }

    const char* Name() const override = 0;

    EnemyState* Clone() const override = 0;
    virtual void Clone(EnemyState* clone) const;

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
};

class EnemyNone : public EnemyState
{
    const char* Name() const override
    {
        return "None";
    }

    EnemyNone* Clone() const override;

    virtual void Clone(EnemyNone* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }
};

class EnemyHurt : public EnemyState
{
public:
    const char* Name() const override
    {
        return "Hurt";
    }

    EnemyHurt* Clone() const override;

    virtual void Clone(EnemyHurt* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;
};

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Skill
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class DBKSkill : public EnemyState
{
public:
    const char* Name() const override
    {
        return "Skill";
    }

    DBKSkill* Clone() const override;

    virtual void Clone(DBKSkill* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;

private:
    static const int m_FLAME_NUM;
    static const int m_FLAME_RADIUS;
    static const double m_DELTA;

    void _ThrowFlame();
};

#endif
