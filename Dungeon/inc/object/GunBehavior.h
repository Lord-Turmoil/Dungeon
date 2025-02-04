/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : GunBehavior.h                             *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Gun behavior.                                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _GUN_BEHAVIOR_H_
#define _GUN_BEHAVIOR_H_

#include "WeaponBehavior.h"

/********************************************************************
** Gun behavior:
**   Ready
*/
class GunBehavior : public WeaponBehavior
{
public:
    GunBehavior()
    {
    }

    ~GunBehavior() override
    {
    }

    const char* Name() const override = 0;

    GunBehavior* Clone() const override = 0;
    virtual void Clone(GunBehavior* clone) const;

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

class GunReady : public GunBehavior
{
public:
    const char* Name() const override
    {
        return "Ready";
    }

    GunReady* Clone() const override;

    virtual void Clone(WeaponReady* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

#endif
