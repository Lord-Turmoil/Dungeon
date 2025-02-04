/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : HeroState.h                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 26, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   For hero's state.                                                        *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _HERO_STATE_H_
#define _HERO_STATE_H_

#include <dungine.h>

/********************************************************************
** Hero's state:
**   None
**   Hurt
**   Skill
*/
class HeroState : public State
{
public:
    HeroState()
    {
    }

    ~HeroState() override
    {
    }

    const char* Name() const override = 0;

    HeroState* Clone() const override = 0;
    virtual void Clone(HeroState* clone) const;

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

class HeroNone : public HeroState
{
    const char* Name() const override
    {
        return "None";
    }

    HeroNone* Clone() const override;

    virtual void Clone(HeroNone* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }
};

class HeroHurt : public HeroState
{
public:
    const char* Name() const override
    {
        return "Hurt";
    }

    HeroHurt* Clone() const override;

    virtual void Clone(HeroHurt* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;
};

class YonySkill : public HeroState
{
public:
    const char* Name() const override
    {
        return "Skill";
    }

    YonySkill* Clone() const override;

    virtual void Clone(YonySkill* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class SpecterSkill : public HeroState
{
public:
    SpecterSkill() : m_minAlpha(100), m_maxAlpha(200)
    {
    }

    const char* Name() const override
    {
        return "Skill";
    }

    SpecterSkill* Clone() const override;

    virtual void Clone(SpecterSkill* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;

private:
    static const int m_DELTA_ALPHA;

    int m_curAlpha;
    int m_minAlpha;
    int m_maxAlpha;
    int m_delta;
};

class BlackHandSkill : public HeroState
{
public:
    const char* Name() const override
    {
        return "Skill";
    }

    BlackHandSkill* Clone() const override;

    virtual void Clone(BlackHandSkill* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
    void OnExit() override;

private:
    virtual void _ThrowFlame();

    static const int m_FLAME_NUM;
    static const int m_FLAME_RADIUS;
    static const double m_DELTA;
};

#endif
