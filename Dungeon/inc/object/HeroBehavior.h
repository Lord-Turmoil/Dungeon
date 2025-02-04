/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroBehavior.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 23, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Behavior of hero. Generally, they are the same.                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _HERO_BEHAVIOR_H_
#define _HERO_BEHAVIOR_H_

#include <dungine.h>

/********************************************************************
** Hero's behavior:
**   Init   -- Play initialization animation.
**   Move   -- Normal and under control.
**   Dead   -- Play dead animation.
**   Perish -- Become invalid.
*/
class HeroBehavior : public Behavior
{
public:
    HeroBehavior()
    {
    }

    ~HeroBehavior() override
    {
    }

    const char* Name() const override = 0;

    HeroBehavior* Clone() const override = 0;
    virtual void Clone(HeroBehavior* clone) const;

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
    virtual void _Collide();

protected:
    // INIT and DEAD only has one direction.
    enum HeroAnimTag
    {
        HERO_ANIM_INIT = 0,
        HERO_ANIM_IDLE,
        HERO_ANIM_MOVE,
        HERO_ANIM_DEAD
    };
};

/********************************************************************
** First show up in the battlefield.
*/
class HeroInit : public HeroBehavior
{
public:
    const char* Name() const override
    {
        return "Init";
    }

    HeroInit* Clone() const override;

    virtual void Clone(HeroInit* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;
};

class HeroMove : public HeroBehavior
{
public:
    const char* Name() const override
    {
        return "Move";
    }

    HeroMove* Clone() const override;

    virtual void Clone(HeroMove* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    void _Move(Event* evnt);
    void _Act(Event* evnt);
};

class HeroDead : public HeroBehavior
{
public:
    const char* Name() const override
    {
        return "Dead";
    }

    HeroDead* Clone() const override;

    virtual void Clone(HeroDead* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class HeroPerish : public HeroBehavior
{
public:
    const char* Name() const override
    {
        return "Perish";
    }

    HeroPerish* Clone() const override;

    virtual void Clone(HeroPerish* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }

    void OnEnter() override;
};

#endif
