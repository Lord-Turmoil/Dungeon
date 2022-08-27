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
 *                    Last Update : August 15, 2022                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The core of the game.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Settings.h"

#include "../../inc/object/Hero.h"
#include "../../inc/object/Portal.h"
#include "../../inc/object/Crate.h"
#include "../../inc/object/Enemy.h"
#include "../../inc/object/Boss.h"

#include "../../inc/object/Component.h"
#include "../../inc/object/Library.h"

#include "../../inc/terrain/Terrain.h"
#include "../../inc/terrain/Space.h"

#include"../../inc/ui/GameInterface.h"


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
Dungeon::Dungeon() :
	m_pTerrain(nullptr),
	m_pSpace(nullptr),
	m_pArena(nullptr),
	m_pHero(nullptr),
	m_pBoss(nullptr),
	m_pPortal(nullptr),
	m_pGameInterface(nullptr),
	m_isToLevelUp(false),
	m_enemyCount(0),
	m_chapter(0),
	m_level(0)
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
	Settings::GetInstance()->SaveConfig();	// Save coin.
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
		m_pArena = static_cast<Arena*>(m_pSpace);
	else
		m_pArena = nullptr;
	auto& brickPool = m_pSpace->GetBrickPool().GetPool();

	if (m_pArena)	// Events only happen in arena.
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
					if (static_cast<Object*>(*it)->Type() == ObjectType::OBJ_ENEMY)
					{
						if (!static_cast<Enemy*>(*it)->IsDead())
							m_pArena->Entry((*it)->GetCoord());
					}
				}
			}
		}
		else if (attr.isEnd && (!m_pPortal->IsActive()))
		{
			m_pPortal->SetActive(true);
			m_pPortal->SetCoord(m_pArena->GetCoord());
			m_pPortal->Initialize();
		}
	}
	else		// Nothing special if hero is in a bridge.
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
		m_quadTree.Insert(*it);
	for (auto it = objectPool.begin(); it != objectPool.end(); it++)
	{
		m_quadTree.Insert(*it);
		if (static_cast<Object*>(*it)->Type() == ObjectType::OBJ_ENEMY)
		{
			if (!static_cast<Enemy*>(*it)->IsDead())
				ind->SetTarget((*it)->GetCoord(), IND_ENEMY);
		}
		else if (static_cast<Object*>(*it)->Type() == ObjectType::OBJ_CRATE)
		{
			if (!static_cast<Crate*>(*it)->IsOpen())
				ind->SetTarget((*it)->GetCoord(), IND_CRATE);
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
		_LevelUp();
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
	BulletLibrary::GetInstance()->Load(BULLET_XML);
	WeaponLibrary::GetInstance()->Load(WEAPON_XML);
	HeroLibrary::GetInstance()->Load(HERO_XML);
	MiscLibrary::GetInstance()->Load(MISC_XML);

	m_chapter = m_level = 0;
	m_enemyCount = 0;

	m_pTerrain = new Terrain();
	m_pTerrain->SetParent(this);
	m_pSpace = nullptr;
	m_pArena = nullptr;

	m_pHero = HeroLibrary::GetInstance()
		->GetHeroByName(Settings::GetInstance()->HeroName())->Clone();
	m_pPortal = MiscLibrary::GetInstance()->
		GetMiscObject<Portal>("Portal")->Clone();

	_Launch();

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
			static_cast<Enemy*>(*it)->Suicide();
	}

	auto& p2 = m_pendingObjects.GetPool();
	for (auto it = p2.begin(); it != p2.end(); it++)
	{
		if (static_cast<Object*>(*it)->Type() == OBJ_ENEMY)
			static_cast<Enemy*>(*it)->Suicide();
	}
}


/******************************************************************************
 * Dungeon::_Launch -- Launch the game at every level.                        *
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
void Dungeon::_Launch()
{
	if (m_level == 0)
		_LoadChapter();

	_InitLevel();

	m_pHero->SetCoord(m_pSpace->GetCoord());
	m_pHero->Initialize();
	m_pHero->Update(nullptr);
	m_camera.Focus(m_pHero->GetCoord());
	m_target.Init(m_pHero->GetCoord());
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
		if (m_chapter >= CHAPTER_NUM)
		{
			_VictoryTransit();
			return;
		}
	}

	_Launch();

	// _LevelUpTransit();
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
 *============================================================================*/
void Dungeon::_LoadChapter()
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
	filename += std::to_string(m_chapter);
	filename += ".xml";

	if (!file.Load(filename.c_str()))
		return;

	XMLElement* node;

	// Load bricks
	node = file.GetElementByTagName("Terrain");
	m_pTerrain->Load(node);

	// Load enemies.
	node = file.GetElementByTagName("EnemyLibrary");
	EnemyLibrary::GetInstance()->Load(node);
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
	m_gameObjects.RemoveObject(m_pHero);
	m_gameObjects.RemoveObject(m_pPortal);
	m_gameObjects.Destroy();	// Bricks not here.
	m_pendingObjects.Destroy();

	m_pTerrain->Generate(m_level);

	// Here, current space is the start space.
	m_pSpace = m_pTerrain->GetCurrentSpace();
	m_quadTree.Initialize(m_pSpace->GetBorder());

	m_pPortal->SetActive(false);
	m_pPortal->SetCoord({ -1000, -1000 });

	AddObject(m_pHero);
	AddObject(m_pPortal);

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
	int upper = m_chapter * 2 + (int)(m_level * 1.5) + difficulty * difficulty / 2;
	int total = Random(lower, dmax(upper, lower + 1));
	if (attr.isEnd && attr.rounds == 1)
	{
		total *= 2;
		_OnFinal();
	}

	// level 1
	auto& pool1 = lib->GetEnemyByLevel(1);
	num = total / 2 + 1;
	size = (int)pool1.size();
	for (int i = 0; i < num; i++)
		AddEnemy(pool1[Random(size)]->Clone());

	// level 2
	auto& pool2 = lib->GetEnemyByLevel(2);
	num = total / 3;
	if (num > 0)
	{
		size = (int)pool2.size();
		if (size > 0)
		{
			for (int i = 0; i < num; i++)
				AddEnemy(pool2[Random(size)]->Clone());
		}
	}

	// level 3
	if (m_level > 1)
	{
		auto& pool3 = lib->GetEnemyByLevel(3);
		num = total / 6;
		if (num > 0)
		{
			size = (int)pool3.size();
			if (size > 0)
			{
				for (int i = 0; i < num; i++)
					AddEnemy(pool3[Random(size)]->Clone());
			}
		}
	}

	if (attr.hasBoss && attr.rounds == 1)
	{
		Boss* boss = static_cast<Boss*>(lib->GetEnemyByLevel(-1)[0]->Clone());
		m_pBoss = boss;
		AddEnemy(boss);
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
 *============================================================================*/
void Dungeon::_LostTransit()
{
	Application::GetInstance()->GetInterface("Lost")->Launch();
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

	sprintf_s(buffer, "Level   %d - %d", m_chapter + 1, m_level + 1);
	widget = static_cast<StaticWidget*>(m_pGameInterface->GetWidget("s-level"));
	static_cast<TextDrawer*>(widget->GetDrawer())->SetText(buffer);

	sprintf_s(buffer, "> Level   %d - %d <", m_chapter + 1, m_level + 1);
	widget = static_cast<StaticWidget*>(m_pGameInterface->GetWidget("b-level"));
	static_cast<TextDrawer*>(widget->GetDrawer())->SetText(buffer);
	widget->ResetTransition();

	if (m_level == 0)
		m_pGameInterface->PlayTrack(m_chapter);
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
	static_cast<StaticWidget*>(m_pGameInterface->GetWidget("b-complete"))
		->ResetTransition();
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
	static_cast<StaticWidget*>(m_pGameInterface->GetWidget("b-final"))
		->ResetTransition();
}