/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Dungeon.h                                 *
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
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include <queue>

#include <dungine.h>


// each chapter has 5 level
extern int CHAPTER_NUM;
extern int LEVEL_NUM;

const int MAX_ENEMY_ROUNDS = 5;
const int MIN_ENENY_ROUNDS = 1;

class Terrain;
class Space;
class Arena;

class Object;
class Enemy;
class Hero;
class Boss;
class Portal;
class Stand;

class GameInterface;

class Flashback;

class Dungeon : public Scene
{
public:
	Dungeon();
	virtual ~Dungeon();

	virtual void Update();
	virtual void Draw();

	// Initialize before the whole game.
	bool Initialize();

public:
	void SetGameInterface(GameInterface* intf) { m_pGameInterface = intf; }
	Coordinate GetMouse() { return m_target; }

	void SetLevelUp() { m_isToLevelUp = true; }

	int GetChapter() const { return m_chapter; }
	int GetLevel() const { return m_level; }

	void SetFlashback(Flashback* pFlashback) { m_pFlashback = pFlashback; }
	
public:
	bool AddEnemy(Enemy* enemy);
	bool AddEnemy(Enemy* enemy, const Coordinate& center);
	void RemoveEnemy() { m_enemyCount--; }
	void RemoveBoss();
	void KillAllEnemy();

public:
	Terrain* GetTerrain() { return m_pTerrain; }
	Arena* GetArena() { return m_pArena; }
	Hero* GetHero() { return m_pHero; }
	Boss* GetBoss() { return m_pBoss; }
	QuadTree* GetQuadTree() { return &m_quadTree; }
	int GetEnemyCount() { return m_enemyCount; }

protected:
	void _Launch();
	void _LevelUp();
	void _LevelFlashback();
	
	void _LoadChapter();
	void _UnLoadChapter();
	void _InitLevel();

	void _ClearUp();

protected:
	void _GenerateEnemy();
	void _GenerateCrate();

	void _AddStand();

protected:
	// Interface transition.
	void _VictoryTransit();
	void _LostTransit();
	void _LevelUpTransit();

	void _OnLevelChange();
	void _OnComplete();
	void _OnFinal();

protected:
	// The target that hero fires at. (On hold)
	Coordinate m_target;

	QuadTree m_quadTree;
	Terrain* m_pTerrain;
	Space* m_pSpace;
	Arena* m_pArena;	// current arena, nullptr if is bridge

	Hero* m_pHero;
	Boss* m_pBoss;
	Portal* m_pPortal;
	Stand* m_pStand;

	int m_enemyCount;
	
	int m_chapter;
	int m_level;

	bool m_isToLoadChapter;	// Denotes whether to load chapter resource.
	bool m_isToLevelUp;

	Flashback* m_pFlashback;

	GameInterface* m_pGameInterface;
};


#endif