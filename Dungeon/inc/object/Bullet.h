/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Bullet.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Bullet in the game.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _BULLET_H_
#define _BULLET_H_

#include <dungine.h>
#include <string>

#include "Object.h"

const double MAX_RANGE = 10000.0;

/********************************************************************
** Well, bullet just have the same behavior, so just one class is OK.
*/
class Bullet : public Object
{
public:
    Bullet(Scene* scene)
        : Object(OBJ_BULLET, scene), m_damage(0), m_range(MAX_RANGE), m_force(0.0), m_AOERadius(0.0), m_isGood(true)
    {
        m_symbol.SetLayer(LAYER_BULLET);
    }

    ~Bullet() override
    {
    }

    const std::string& Name() const
    {
        return m_name;
    }

    Bullet* Clone() const override;
    virtual void Clone(Bullet* clone) const;

    bool Load(XMLElement* node) override;

    // Bullet must be activated to switch to fly behavior.
    void Activate();
    void Corrupt();

public:
    void SetDirection(const Vector& dir)
    {
        m_dir = dir;
    }

    const Vector& GetDirection() const
    {
        return m_dir;
    }

    void SetRange(double range)
    {
        m_range = range;
    }

    double GetRange() const
    {
        return m_range;
    }

    void SetForce(double force)
    {
        m_force = force;
    }

    double GetForce() const
    {
        return m_force;
    }

    void SetAOERadius(double radius)
    {
        m_AOERadius = radius;
    }

    double GetAOERadius() const
    {
        return m_AOERadius;
    }

    void SetDamage(int damage)
    {
        m_damage = damage;
    }

    int GetDamage() const
    {
        return m_damage;
    }

    void SetGood(bool isGood)
    {
        m_isGood = isGood;
    }

    bool IsGood() const
    {
        return m_isGood;
    }

    bool IsDirectional() const
    {
        return m_isDirectional;
    }

    bool IsPenetrable() const
    {
        return m_isPenetrable;
    }

protected:
    void _InitBehavior(XMLElement* node = nullptr) override;

    std::string m_name;

    int m_damage;

    Vector m_dir;

    /*
    ** Range of the object, bullet will "corrupt" - another way
    ** to explode. Default range is infinit.
    */
    double m_range;

    /*
    ** Force of the bullet, it will affect the hurt object.
    */
    double m_force;

    double m_AOERadius; // Area of effect

    /*
    ** If explode or corrupt, the bullet will be not good.
    */
    bool m_isGood;
    bool m_isDirectional; // Whether the image need rotate or not.
    bool m_isPenetrable;  // Whether penetrate enemy or not.
};

#endif
