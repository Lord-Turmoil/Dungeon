/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : BulletLibrary.h                           *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 19, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Bullet library.                                                          *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _BULLET_LIBRARY_H_
#define _BULLET_LIBRARY_H_

#include <dungine.h>

class Bullet;

class BulletLibrary : private AbstractLibrary<Bullet>, public Singleton<BulletLibrary>
{
    friend class Singleton<BulletLibrary>;

public:
    bool Load(const char* filename);

    bool Load(XMLElement* node) override;
    void UnLoad() override;

    Bullet* AddBullet(Bullet* bullet);
    Bullet* GetBulletByName(const std::string& name);

private:
    BulletLibrary()
    {
    }

    ~BulletLibrary() override
    {
    }
};

#endif
