/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : MeleeBehavior.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Melee behavior.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _MELEE_BEHAVIOR_H_
#define _MELEE_BEHAVIOR_H_

#include "WeaponBehavior.h"

/********************************************************************
** Melee behavior:
**   Ready
**   Fire
*/
class MeleeBehavior : public WeaponBehavior
{
public:
    MeleeBehavior()
    {
    }

    ~MeleeBehavior() override
    {
    }

    const char* Name() const override = 0;

    MeleeBehavior* Clone() const override = 0;
    virtual void Clone(MeleeBehavior* clone) const;

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
    virtual void _Slash();

    /*
    ** 2022/11/25 TS:
    ** If melee can slash bullet during the whole fire progress?
    */
    virtual void _SlashBullet();
};

class MeleeReady : public MeleeBehavior
{
public:
    const char* Name() const override
    {
        return "Ready";
    }

    MeleeReady* Clone() const override;

    virtual void Clone(MeleeReady* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class MeleeFire : public MeleeBehavior
{
public:
    const char* Name() const override
    {
        return "Fire";
    }

    MeleeFire* Clone() const override;

    virtual void Clone(MeleeFire* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

#endif
