/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Dungeon.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 13, 2022                             *
 *                                                                            *
 *                    Last Update : December 17, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   The core of the game.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Flashback.h"
#include "../../inc/game/Settings.h"

#include "../../inc/object/Boss.h"
#include "../../inc/object/Crate.h"
#include "../../inc/object/Enemy.h"
#include "../../inc/object/Hero.h"
#include "../../inc/object/Portal.h"
#include "../../inc/object/Stand.h"
#include "../../inc/object/Weapon.h"

#include "../../inc/object/Component.h"
#include "../../inc/object/Library.h"

#include "../../inc/terrain/Space.h"
#include "../../inc/terrain/Terrain.h"

#include "../../inc/ui/GameInterface.h"

/******************************************************************************
 * Dungeon::Dungeon -- Constructor of the object.                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
Dungeon::Dungeon()
    : m_pTerrain(nullptr), m_pSpace(nullptr), m_pArena(nullptr), m_pHero(nullptr), m_pBoss(nullptr), m_pPortal(nullptr),
      m_pStand(nullptr), m_enemyCount(0), m_chapter(0), m_level(0), m_isInfinite(false), m_isToLoadChapter(false),
      m_isToLevelUp(false), m_pGameInterface(nullptr)
{
    m_camera.Initialize(deviceInfo.GetDeviceRect());
}

/******************************************************************************
 * Dungeon::~Dungeon -- Destructor of the object.                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
Dungeon::~Dungeon()
{
    _ClearUp();
    Settings::GetInstance()->SaveConfig(); // Save coin.
}

/******************************************************************************
 * Dungeon::Update -- Update the game each frame.                             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::Update()
{
    m_target = m_camera.AbsoluteToRelative(Event::GetInstance()->Mouse());

    auto& objectPool = m_gameObjects.GetPool();

    // Update terrain.
    m_pTerrain->Update();
    m_pSpace = m_pTerrain->GetCurrentSpace();
    if (m_pSpace->Type() == SpaceType::SPC_ARENA)
    {
        m_pArena = static_cast<Arena*>(m_pSpace);
    }
    else
    {
        m_pArena = nullptr;
    }
    auto& brickPool = m_pSpace->GetBrickPool().GetPool();

    if (m_pArena) // Events only happen in arena.
    {
        auto& attr = m_pArena->Attribute();
        if (!attr.isCompleted)
        {
            if (m_enemyCount == 0)
            {
                if (attr.rounds > 0)
                {
                    if (m_pArena->IsActivated())
                    {
                        _GenerateEnemy();
                        attr.rounds--;
                    }
                }
                else
                {
                    attr.Complete();
                    _OnComplete();
                }
            }
            else
            {
                m_pArena->Clear();
                for (auto it = objectPool.begin(); it != objectPool.end(); it++)
                {
                    if (static_cast<Object*>(*it)->Type() == OBJ_ENEMY)
                    {
                        if (!static_cast<Enemy*>(*it)->IsDead())
                        {
                            (*it)->SetCoord(m_pArena->Revise((*it)->GetCoord()));
                            m_pArena->Entry((*it)->GetCoord());
                        }
                    }
                }
            }
            m_pHero->SetCoord(m_pArena->Revise(m_pHero->GetCoord()));
        }
        else if (attr.isEnd && (!m_pPortal->IsActive()))
        {
            m_pPortal->SetActive(true);
            m_pPortal->SetCoord(m_pArena->GetCoord());
            m_pPortal->Initialize();
        }
    }
    else // Nothing special if hero is in a bridge.
    {
        // On hold.
    }

    /*
    ** Initialize quadtree.
    ** By the way, initialize hero's indicator.
    */
    auto ind = m_pHero->GetComponent<IndicatorComponent>();

    m_quadTree.Clear();
    ind->ClearTarget();
    for (auto it = brickPool.begin(); it != brickPool.end(); it++)
    {
        m_quadTree.Insert(*it);
    }
    for (auto it = objectPool.begin(); it != objectPool.end(); it++)
    {
        m_quadTree.Insert(*it);

        if (static_cast<Object*>(*it)->Type() == OBJ_ENEMY)
        {
            if (!static_cast<Enemy*>(*it)->IsDead())
            {
                ind->SetTarget((*it)->GetCoord(), IND_ENEMY);
            }
        }
        else if (static_cast<Object*>(*it)->Type() == OBJ_CRATE)
        {
            if (!static_cast<Crate*>(*it)->IsOpen())
            {
                ind->SetTarget((*it)->GetCoord(), IND_CRATE);
            }
        }
    }

    // Update, bricks will be updated in terrain.
    m_isUpdating = true;
    m_gameObjects.Update(Event::GetInstance());
    m_isUpdating = false;

    // Update camera.
    m_camera.Follow(BlendValue(m_pHero->GetCoord(), m_target, 0.3));
    m_camera.Update();

    // Check if game over.
    if (!m_pHero->IsValid())
    {
        _LostTransit();
        return;
    }

    _UpdateObjectPool();

    if (m_isToLevelUp)
    {
        _LevelUp();
    }
    else if (m_pFlashback)
    {
        _LevelFlashback();
    }
}

/******************************************************************************
 * Dungeon::Draw -- Draw the game.                                            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::Draw()
{
    m_pTerrain->Draw(&m_camera);
    m_gameObjects.Draw(&m_camera);
    m_camera.Draw();
}

/******************************************************************************
 * Dungeon::Initialize -- Initialize the whole game.                          *
 *                                                                            *
 *    This is called before everything.                                       *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return succeeded or not.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
bool Dungeon::Initialize()
{
    // Global resources.
    if (!BulletLibrary::GetInstance()->Load(BULLET_XML))
    {
        return false;
    }
    if (!WeaponLibrary::GetInstance()->Load(WEAPON_XML))
    {
        return false;
    }
    if (!HeroLibrary::GetInstance()->Load(HERO_XML))
    {
        return false;
    }
    if (!MiscLibrary::GetInstance()->Load(MISC_XML))
    {
        return false;
    }

    m_isInfinite = Settings::GetInstance()->IsInfinite();

    m_chapter = m_level = 0;
    m_enemyCount = 0;

    m_pTerrain = new Terrain();
    m_pTerrain->SetParent(this);
    m_pSpace = nullptr;
    m_pArena = nullptr;

    Hero* heroProto = HeroLibrary::GetInstance()->GetHeroByName(Settings::GetInstance()->HeroName());
    if (!heroProto)
    {
        LOG_ERROR("Failed to load hero %s", Settings::GetInstance()->HeroName().c_str());
        return false;
    }
    m_pHero = heroProto->Clone();
    AddObject(m_pHero);

    Portal* portalProto = MiscLibrary::GetInstance()->GetMiscObject<Portal>("Portal");
    if (!portalProto)
    {
        LOG_ERROR("Failed to load Portal");
        return false;
    }
    m_pPortal = portalProto->Clone();
    AddObject(m_pPortal);

    Stand* standProto = MiscLibrary::GetInstance()->GetMiscObject<Stand>("Stand");
    if (!standProto)
    {
        LOG_ERROR("Failed to load Stand");
        return false;
    }
    m_pStand = standProto->Clone();
    AddObject(m_pStand);

    m_isToLoadChapter = true;
    if (!_Launch())
    {
        LOG_ERROR("Failed to launch game!");
        return false;
    }

    Update();

    _RETURN_STATE();
}

/******************************************************************************
 * Dungeon::AddEnemy -- AddEnemy to the scene.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
bool Dungeon::AddEnemy(Enemy* enemy)
{
    ColliderBoxComponent* box = enemy->GetComponent<ColliderBoxComponent>();

    enemy->SetCoord(m_pArena->FindBlank(box->GetBoxWidth(), box->GetBoxHeight()));
    m_pArena->Entry(enemy->GetCoord());
    enemy->Initialize();

    AddObject(enemy);

    m_enemyCount++;

    return true;
}

bool Dungeon::AddEnemy(Enemy* enemy, const Coordinate& center)
{
    ColliderBoxComponent* box = enemy->GetComponent<ColliderBoxComponent>();

    enemy->SetCoord(m_pArena->FindBlank(center, box->GetBoxWidth(), box->GetBoxHeight()));
    m_pArena->Entry(enemy->GetCoord());
    enemy->Initialize();

    AddObject(enemy);

    m_enemyCount++;

    return true;
}

/******************************************************************************
 * Dungeon::RemoveBoss -- Remove boss.                                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/25 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::RemoveBoss()
{
    if (m_pBoss)
    {
        m_pBoss = nullptr;
        m_enemyCount--;

        /*
        ** 2022/12/17 TS:
        ** Well, when Boss dies, the track will change back.
        */
        m_pGameInterface->PlayTrack(m_chapter % CHAPTER_NUM);
    }
}

/******************************************************************************
 * Dungeon::KillAllEnemy -- Kill all enemies at once.                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/25 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::KillAllEnemy()
{
    auto& p1 = m_gameObjects.GetPool();
    for (auto it = p1.begin(); it != p1.end(); it++)
    {
        if (static_cast<Object*>(*it)->Type() == OBJ_ENEMY)
        {
            static_cast<Enemy*>(*it)->Suicide();
        }
    }

    auto& p2 = m_pendingObjects.GetPool();
    for (auto it = p2.begin(); it != p2.end(); it++)
    {
        if (static_cast<Object*>(*it)->Type() == OBJ_ENEMY)
        {
            static_cast<Enemy*>(*it)->Suicide();
        }
    }
}

/******************************************************************************
 * Dungeon::_Launch -- Launch the game at every level.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  Return status.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *   2022/12/13 Tony : Added return value.                                    *
 *============================================================================*/
bool Dungeon::_Launch()
{
    if (m_isToLoadChapter)
    {
        if (!_LoadChapter())
        {
            return false;
        }
    }
    _InitLevel();
    m_isToLoadChapter = false; // This is bad... _InitLevel uses this flag.

    m_pHero->SetCoord(m_pSpace->GetCoord());
    m_pHero->Initialize();
    m_pHero->Update(nullptr);
    m_camera.Focus(m_pHero->GetCoord());
    m_target.Init(m_pHero->GetCoord());

    return true;
}

/******************************************************************************
 * Dungeon::_LevelUp -- Perform level up.                                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_LevelUp()
{
    m_isToLevelUp = false;

    m_level++;
    if (m_level >= LEVEL_NUM)
    {
        m_level = 0;
        m_chapter++;
        if (!IsInfinite() && (m_chapter >= CHAPTER_NUM))
        {
            _VictoryTransit();
            return;
        }
        m_isToLoadChapter = true;
    }

    _Launch();

    // _LevelUpTransit();
}

/******************************************************************************
 * Dungeon::_LevelFlashback -- Perform level flashback.                       *
 *                                                                            *
 *    This will... actually start a new game... And works like this.          *
 *    1. Reset hero status.                                                   *
 *    2. Reload resources if needed.                                          *
 *    3. Reset chapter and level.                                             *
 *    4. Then, just _Launch.                                                  *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/12/08 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_LevelFlashback()
{
    // Reset Hero.
    const std::string& heroName = m_pFlashback->GetHeroName();
    if (heroName != m_pHero->Name())
    {
        m_gameObjects.DeleteObject(m_pHero);
        Settings::GetInstance()->HeroName(heroName);
        m_pHero = HeroLibrary::GetInstance()->GetHeroByName(heroName)->Clone();
        AddObject(m_pHero);
    }
    else
    {
        m_pHero->Revitalize();
    }

    m_pHero->CostHP(m_pHero->GetMaxHP() - m_pFlashback->GetHP());
    m_pHero->CostMP(m_pHero->GetMaxMP() - m_pFlashback->GetMP());
    m_pHero->CostArmor(m_pHero->GetMaxArmor() - m_pFlashback->GetArmor());
    m_pHero->CostChi(m_pHero->GetMaxChi() - m_pFlashback->GetChi());

    m_pHero->GetComponent<WeaponComponent>()->Clear();
    auto& weaponList = m_pFlashback->GetWeaponList();
    WeaponLibrary* lib = WeaponLibrary::GetInstance();
    for (auto it = weaponList.begin(); it != weaponList.end(); it++)
    {
        m_pHero->PickUpWeapon(lib->GetWeaponByName(*it)->Clone());
    }

    // Reset level.
    m_isInfinite = m_pFlashback->GetInfinite();
    if (m_chapter % CHAPTER_NUM != m_pFlashback->GetChapter() % CHAPTER_NUM)
    {
        m_isToLoadChapter = true;
    }
    m_chapter = m_pFlashback->GetChapter();
    m_level = m_pFlashback->GetLevel();

    m_pFlashback = nullptr;

    _Launch();
}

/******************************************************************************
 * Dungeon::_LoadChapter -- Load resource of a chapter.                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *   2022/12/13 Tony : Added return value.                                    *
 *============================================================================*/
bool Dungeon::_LoadChapter()
{
    /*
    **	<Chapter>
    **		<Terrain>
    **		</Terrain>
    **		<EnemyLibrary>
    **		</EnemyLibrary>
    **	</Chapter>
    */
    _UnLoadChapter();

    XMLFile file;
    std::string filename;

    filename = CHAPTER_DIRECTORY;
    filename += "Chapter";
    filename += std::to_string(m_chapter % CHAPTER_NUM);
    filename += ".xml";

    if (!file.Load(filename.c_str()))
    {
        return false;
    }

    XMLElement* node;

    // Load bricks
    node = file.GetElementByTagName("Terrain");
    if (!m_pTerrain->Load(node))
    {
        return false;
    }

    // Load enemies.
    node = file.GetElementByTagName("EnemyLibrary");
    if (!EnemyLibrary::GetInstance()->Load(node))
    {
        return false;
    }

    return true;
}

/******************************************************************************
 * Dungeon::_UnLoadChapter -- UnLoad resource of a chapter.                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_UnLoadChapter()
{
    m_pTerrain->UnLoad();
    EnemyLibrary::GetInstance()->UnLoad();
}

/******************************************************************************
 * Dungeon::_InitLevel -- Initialize a level.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_InitLevel()
{
    RemoveObject(m_pHero);
    RemoveObject(m_pPortal);
    RemoveObject(m_pStand);

    m_gameObjects.Destroy(); // Bricks not here.
    m_pendingObjects.Destroy();

    m_pTerrain->Generate(m_level);

    // Here, current space is the start space.
    m_pSpace = m_pTerrain->GetCurrentSpace();
    m_pArena = static_cast<Arena*>(m_pSpace);
    m_quadTree.Initialize(m_pSpace->GetBorder());

    m_pPortal->SetActive(false);
    m_pPortal->SetCoord({ -1000, -1000 });

    _AddStand();

    AddObject(m_pHero);
    AddObject(m_pPortal);
    AddObject(m_pStand);

    _OnLevelChange();
}

/******************************************************************************
 * Dungeon::_ClearUp -- Clear up all resources.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_ClearUp()
{
    BulletLibrary::GetInstance()->UnLoad();
    WeaponLibrary::GetInstance()->UnLoad();
    HeroLibrary::GetInstance()->UnLoad();
    MiscLibrary::GetInstance()->UnLoad();

    _UnLoadChapter();

    _DELETE(m_pTerrain);

    m_gameObjects.Destroy();
    m_pendingObjects.Destroy();
}

/******************************************************************************
 * Dungeon::_GenerateEnemy -- Generate enemy.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_GenerateEnemy()
{
    auto& attr = m_pArena->Attribute();
    auto lib = EnemyLibrary::GetInstance();
    int difficulty = Settings::GetInstance()->Difficulty();
    int num;
    int size;

    int lower = difficulty;
    // 2022/12/13 TS: Commented
    // int upper = m_chapter * 2 + (int)(m_level * 1.5) + difficulty * difficulty
    // / 2; It's gonna be really hard, huh.
    int width = m_pArena->GetGraphWidth();
    int upper = dmin(m_chapter * m_chapter + static_cast<int>(m_level * 1.5) + difficulty * difficulty / 2,
                     static_cast<int>((width - 2) * (width - 2) * 0.6f));
    int total = Random(lower, dmax(upper, lower + 1));
    if (attr.isEnd && attr.rounds == 1)
    {
        total *= 2;
    }

    // level 1
    auto& pool1 = lib->GetEnemyByLevel(1);
    num = total / 2 + 1;
    size = static_cast<int>(pool1.size());
    for (int i = 0; i < num; i++)
    {
        AddEnemy(pool1[Random(size)]->Clone());
    }

    // level 2
    auto& pool2 = lib->GetEnemyByLevel(2);
    num = total / 3;
    if (num > 0)
    {
        size = static_cast<int>(pool2.size());
        if (size > 0)
        {
            for (int i = 0; i < num; i++)
            {
                AddEnemy(pool2[Random(size)]->Clone());
            }
        }
    }

    // unlock level 3 enemies
    if (m_chapter + m_level > 1)
    {
        auto& pool3 = lib->GetEnemyByLevel(3);
        num = total / 6;
        if (num > 0)
        {
            size = static_cast<int>(pool3.size());
            if (size > 0)
            {
                for (int i = 0; i < num; i++)
                {
                    AddEnemy(pool3[Random(size)]->Clone());
                }
            }
        }
    }

    if (attr.hasBoss && attr.rounds == 1)
    {
        Boss* boss = static_cast<Boss*>(lib->GetEnemyByLevel(-1)[0]->Clone());
        if (IsInfinite())
        {
            boss->Enhance(m_chapter * 0.2f);
        }
        m_pBoss = boss;
        AddEnemy(boss);
    }

    if (attr.isEnd && attr.rounds == 1)
    {
        _OnFinal();
    }
}

/******************************************************************************
 * Dungeon::GenerateCrate -- Generate crate.                                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/15 Tony : Created.                                               *
 *   2022/08/25 Tony : Feature updated.                                       *
 *============================================================================*/
void Dungeon::_GenerateCrate()
{
    Crate* crate = MiscLibrary::GetInstance()->GetMiscObject<Crate>("Crate")->Clone();
    auto box = crate->GetComponent<ColliderBoxComponent>();

    /*
    ** Since now crate needs scene info, so crate should generate
    ** after being added to the scene.
    */
    AddObject(crate);

    crate->SetCoord(m_pArena->FindBlank(box->GetBoxWidth(), box->GetBoxHeight()));
    crate->Generate();
}

/******************************************************************************
 * Dungeon::_AddStand -- Add stand in the starting arena.                     *
 *                                                                            *
 *    This should be called right on _InitLevel. Stand is placed              *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/12/09 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_AddStand()
{
    ColliderBoxComponent* box = m_pStand->GetComponent<ColliderBoxComponent>();

    m_pStand->Initialize();

    Coordinate coord = m_pArena->FindBlank(m_pArena->GetBorder().pos, box->GetBoxWidth(), box->GetBoxHeight());
    m_pStand->SetCoord(coord);
}

/******************************************************************************
 * Dungeon::_VictoryTransit -- Make transit after victory.                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_VictoryTransit()
{
    Application::GetInstance()->GetInterface("Victory")->Launch();
}

/******************************************************************************
 * Dungeon::_LostTransit -- Make transit when lost.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *   2022/11/22 Tony : Out time may change when lost.                         *
 *============================================================================*/
void Dungeon::_LostTransit()
{
    PlainInterface* intf = static_cast<PlainInterface*>(Application::GetInstance()->GetInterface("Game"));
    clock_t oldTime = intf->GetOutTime();

    intf->SetOutTime(oldTime * 10);
    Application::GetInstance()->GetInterface("Lost")->Launch();
    intf->SetOutTime(oldTime);
}

/******************************************************************************
 * Dungeon::_LevelUpTransit -- Transit when level up.                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/02 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_LevelUpTransit()
{
    Application::GetInstance()->GetInterface("Transit")->Attach();
}

/******************************************************************************
 * Dungeon::_OnLevelChange -- Change UI on level change.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/04 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_OnLevelChange()
{
    char buffer[32];
    StaticWidget* widget;

    if (IsInfinite())
    {
        sprintf_s(buffer, "Infinite   %d - %d", m_chapter + 1, m_level + 1);
    }
    else
    {
        sprintf_s(buffer, "Level   %d - %d", m_chapter + 1, m_level + 1);
    }
    widget = static_cast<StaticWidget*>(m_pGameInterface->GetWidget("s-level"));
    static_cast<TextDrawer*>(widget->GetDrawer())->SetText(buffer);

    if (IsInfinite())
    {
        sprintf_s(buffer, "> Infinite   %d - %d <", m_chapter + 1, m_level + 1);
    }
    else
    {
        sprintf_s(buffer, "> Level   %d - %d <", m_chapter + 1, m_level + 1);
    }
    widget = static_cast<StaticWidget*>(m_pGameInterface->GetWidget("b-level"));
    static_cast<TextDrawer*>(widget->GetDrawer())->SetText(buffer);
    widget->ResetTransition();

    if (m_isToLoadChapter)
    {
        m_pGameInterface->PlayTrack(m_chapter % CHAPTER_NUM);
    }
}

/******************************************************************************
 * Dungeon::_OnComplete -- On arena complete.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/04 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_OnComplete()
{
    static_cast<StaticWidget*>(m_pGameInterface->GetWidget("b-complete"))->ResetTransition();
    _GenerateCrate();
}

/******************************************************************************
 * Dungeon::_OnFinalChange -- Change UI on level change.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/04 Tony : Created.                                               *
 *============================================================================*/
void Dungeon::_OnFinal()
{
    static_cast<StaticWidget*>(m_pGameInterface->GetWidget("b-final"))->ResetTransition();
    if (m_pBoss)
    {
        /*
        ** 2022/12/17 TS:
        ** Well, for now, there is only one boss track. Just be this.
        */
        m_pGameInterface->PlayTrack(10);
    }
}
