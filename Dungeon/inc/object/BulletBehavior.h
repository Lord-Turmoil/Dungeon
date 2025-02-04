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
 * Overview:                                                                 *
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
    BulletBehavior()
    {
    }

    ~BulletBehavior() override
    {
    }

    const char* Name() const override = 0;

    BulletBehavior* Clone() const override = 0;
    virtual void Clone(BulletBehavior* clone) const;

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
    const char* Name() const override
    {
        return "None";
    }

    BulletNone* Clone() const override;

    virtual void Clone(BulletNone* clone) const
    {
    }

    void Update(Event* evnt) override
    {
    }
};

class BulletFly : public BulletBehavior
{
public:
    const char* Name() const override
    {
        return "Fly";
    }

    BulletFly* Clone() const override;

    virtual void Clone(BulletFly* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    // Damage game objects.
    virtual bool _Explode(GameObject* obj);
    virtual void _AdjustDirection();

    double m_dist; // distance covered
    std::vector<GameObject*> m_candidates;
};

class BulletExplode : public BulletBehavior
{
public:
    const char* Name() const override
    {
        return "Explode";
    }

    BulletExplode* Clone() const override;

    virtual void Clone(BulletExplode* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

class BulletCorrupt : public BulletBehavior
{
public:
    const char* Name() const override
    {
        return "Corrupt";
    }

    BulletCorrupt* Clone() const override;

    virtual void Clone(BulletCorrupt* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

/*
** I'd like to name all objects' invalid state "Perish",
** since "Die" may be a valid behavior.
*/
class BulletPerish : public BulletBehavior
{
public:
    const char* Name() const override
    {
        return "Perish";
    }

    BulletPerish* Clone() const override;

    virtual void Clone(BulletPerish* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;
};

#endif
