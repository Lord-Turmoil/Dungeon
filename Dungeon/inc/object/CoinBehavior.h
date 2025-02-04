/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : CoinBehavior.h                            *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 14, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Coin behavior.                                                         *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _COIN_BEHAVIOR_H_
#define _COIN_BEHAVIOR_H_

#include <dungine.h>

class CoinBehavior : public Behavior
{
public:
    CoinBehavior()
    {
    }

    ~CoinBehavior() override
    {
    }

    const char* Name() const override = 0;

    CoinBehavior* Clone() const override = 0;
    virtual void Clone(CoinBehavior* clone) const;

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
    enum CoinAnimTag
    {
        COIN_ANIM_FLY,
        COIN_ANIM_ACTIVE
    };
};

class CoinFly final : public CoinBehavior
{
public:
    CoinFly()
    {
    }

    ~CoinFly() override
    {
    }

    const char* Name() const override
    {
        return "Fly";
    }

    CoinFly* Clone() const override;

    virtual void Clone(CoinFly* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class CoinActive final : public CoinBehavior
{
public:
    CoinActive()
    {
    }

    ~CoinActive() override
    {
    }

    const char* Name() const override
    {
        return "Active";
    }

    CoinActive* Clone() const override;

    virtual void Clone(CoinActive* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class CoinPerish final : public CoinBehavior
{
public:
    CoinPerish()
    {
    }

    ~CoinPerish() override
    {
    }

    const char* Name() const override
    {
        return "Perish";
    }

    CoinPerish* Clone() const override;

    virtual void Clone(CoinPerish* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }

    void OnEnter() override;
};

#endif
