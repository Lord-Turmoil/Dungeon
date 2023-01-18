/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : StandBehavior.cpp                         *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : December 8, 2022                          *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The stand has many behaviors.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/game/Dungeon.h"
#include "../../inc/game/Flashback.h"
#include "../../inc/game/Settings.h"

#include "../../inc/terrain/Space.h"

#include "../../inc/object/Stand.h"
#include "../../inc/object/StandBehavior.h"
#include "../../inc/object/WeaponComponent.h"
#include "../../inc/object/SoundComponent.h"

#include "../../inc/object/Portal.h"
#include "../../inc/object/MiscLibrary.h"

#include "../../inc/object/Hero.h"


static const Coordinate DIALOG_OFFSET = { -144, -140 };
static const int DIALOG_FONT_SIZE = 28;

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** StandBehavior
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
const std::string StandBehavior::DIALOG_RES_ID = "dialog";
const time_t StandBehavior::STAND_TIMEOUT = 5000;

StandBehavior::StandBehavior() : m_elapsedTime(0)
{
	// Bad Magic number. :(
	m_symbol.SetOffset(DIALOG_OFFSET);
	m_symbol.SetLayer(LAYER_HIGH);
}

void StandBehavior::Clone(StandBehavior* clone) const
{
	Behavior::Clone(clone);

	clone->m_elapsedTime = m_elapsedTime;
	clone->m_symbol = m_symbol;
}

void StandBehavior::OnEnter()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());

	m_symbol.SetCoord(stand->GetCoord());
	stand->GetSymbol()->SetSubSymbol(&m_symbol);

	m_elapsedTime = 0;
}

void StandBehavior::OnExit()
{
	static_cast<Stand*>(m_parent->GetGameObject())
		->GetSymbol()->SetSubSymbol(nullptr);
}

void StandBehavior::_AdjustDirection(bool isLeft)
{
	auto anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim();
	anim->SetDir(isLeft ? ANIM_LEFT : ANIM_RIGHT);
}

void StandBehavior::_ApplyFontAttribute()
{
	settextcolor(BLACK);
	settextstyle(DIALOG_FONT_SIZE, 0, INTERNAL_FONT_NAME);
	setbkmode(TRANSPARENT);
}

void StandBehavior::_Collide()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Dungeon* dungeon = static_cast<Dungeon*>(stand->GetScene());
	QuadTree* tree = dungeon->GetQuadTree();

	std::vector<GameObject*> candidates = tree->Query(stand);
	for (auto it = candidates.begin(); it != candidates.end(); it++)
	{
		if (*it != stand)
			Collider::Collide(stand, *it);
	}
}

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** StandIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
const time_t StandIdle::IDLE_DURATION = 5000;

StandIdle* StandIdle::Clone() const
{
	auto clone = new StandIdle();
	clone->_MakePrototype(false);

	StandBehavior::Clone(clone);

	return clone;
}

void StandIdle::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();
	double dist = Distance(hero->GetCoord(), stand->GetCoord());

	m_symbol.SetCoord(stand->GetCoord());

	// Hmm... it won't overflow, huh?
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	if (dist < stand->GetRadius())
	{
		if (stand->IsAvailable())
			m_parent->ChangeBehavior("Choose");
		else
		{
			if (m_elapsedTime > IDLE_DURATION)
				m_parent->ChangeBehavior("Greet");
		}
	}

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandIdle::OnEnter()
{
	auto anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim();

	anim->SetMotion(STAND_ANIM_ACTIVE);
	anim->SetDir(ANIM_LEFT);	// Actually it doesn't have direction.

	m_elapsedTime = 0;
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** StandChoose
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
const wchar_t StandChoose::PROMPT_SAVE[]	= L"Save progress? (V)";
const wchar_t StandChoose::PROMPT_FLASH[]	= L"Flashback? (F)       ";

StandChoose* StandChoose::Clone() const
{
	auto clone = new StandChoose();
	clone->_MakePrototype(false);

	StandBehavior::Clone(clone);

	return clone;
}

void StandChoose::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();
	double dist = Distance(hero->GetCoord(), stand->GetCoord());

	// m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	m_symbol.SetCoord(stand->GetCoord());

	if (dist < stand->GetRadius())
	{
		Event* evnt = Event::GetInstance();
		
		if (evnt->Sluggish(VK_V))
			m_parent->ChangeBehavior("Save");
		else if (evnt->Sluggish(VK_F))
			m_parent->ChangeBehavior("Flash");
		
		// m_elapsedTime = 0;
	}
	else
	{
		m_parent->ChangeBehavior("Idle");
	}

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandChoose::OnEnter()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());

	m_symbol.SetCoord(stand->GetCoord());
	stand->GetSymbol()->SetSubSymbol(&m_symbol);

	m_elapsedTime = 0;

	stand->GetComponent<SoundComponent>()->Play("choose");
}

void StandChoose::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 0, 288, 40);
	_ApplyFontAttribute();

	rect.SetPos(0, 4);
	drawtext(PROMPT_SAVE, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	rect.SetPos(0, 44);
	drawtext(PROMPT_FLASH, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}


/********************************************************************
** StandGreet
*/
const wchar_t StandGreet::PROMPT_GREET[] = L"Merry Christmas!";

StandGreet* StandGreet::Clone() const
{
	auto clone = new StandGreet();
	clone->_MakePrototype(false);

	StandBehavior::Clone(clone);

	return clone;
}

void StandGreet::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();
	
	m_symbol.SetCoord(stand->GetCoord());

	if (m_elapsedTime > STAND_TIMEOUT)
		m_parent->ChangeBehavior("Idle");

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandGreet::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 8, 288, 40);
	_ApplyFontAttribute();

	drawtext(PROMPT_GREET, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}


/********************************************************************
** StandConfirmSave
*/
const wchar_t StandSave::PROMPT_CONFIRM_SAVE_1[] = L"Save your progress  ";
const wchar_t StandSave::PROMPT_CONFIRM_SAVE_2[] = L"for 100 coins? (Y/N)";

StandSave* StandSave::Clone() const
{
	auto clone = new StandSave();
	clone->_MakePrototype(false);

	StandBehavior::Clone(clone);

	return clone;
}

void StandSave::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();
	double dist = Distance(hero->GetCoord(), stand->GetCoord());

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	m_symbol.SetCoord(stand->GetCoord());

	if (dist < stand->GetRadius())
	{
		Event* evnt = Event::GetInstance();

		if (evnt->Sluggish(VK_Y))
		{
			if (Settings::GetInstance()->Coin() < stand->SaveCost())
				m_parent->ChangeBehavior("Insufficient");
			else
			{
				_Save();
				m_parent->ChangeBehavior("Saved");
			}
		}
		else if (evnt->Sluggish(VK_N))
			m_parent->ChangeBehavior("Cancel");

		m_elapsedTime = 0;
	}
	else
	{
		if (m_elapsedTime > STAND_TIMEOUT)
			m_parent->ChangeBehavior("Idle");
	}

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandSave::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 0, 288, 40);
	_ApplyFontAttribute();

	rect.SetPos(0, 4);
	drawtext(PROMPT_CONFIRM_SAVE_1, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	rect.SetPos(0, 44);
	drawtext(PROMPT_CONFIRM_SAVE_2, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}

bool StandSave::_Save()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Flashback* flashback = stand->GetFlashback();
	Dungeon* dungeon = static_cast<Dungeon*>(stand->GetScene());
	Hero* hero = dungeon->GetHero();

	// Set hero.
	flashback->SetHeroName(hero->Name());
	flashback->SetHP(hero->GetHP());
	flashback->SetMP(hero->GetMP());
	flashback->SetArmor(hero->GetArmor());
	flashback->SetChi(hero->GetChi());
	flashback->SetWeaponList(hero->GetComponent<WeaponComponent>()->GetWeaponList());

	// Set progress.
	flashback->SetInfinite(dungeon->IsInfinite());
	flashback->SetChapter(dungeon->GetChapter());
	flashback->SetLevel(dungeon->GetLevel());

	// Set valid.
	flashback->IsValid(true);

	flashback->Save();

	return true;
}

/********************************************************************
** StandFlash
*/
const wchar_t StandFlash::PROMPT_CONFIRM_FLASH_1[] = L"Flash back for  ";
const wchar_t StandFlash::PROMPT_CONFIRM_FLASH_2[] = L"200 coins? (Y/N)";

StandFlash* StandFlash::Clone() const
{
	auto clone = new StandFlash();
	clone->_MakePrototype(false);

	StandBehavior::Clone(clone);

	return clone;
}

void StandFlash::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();
	double dist = Distance(hero->GetCoord(), stand->GetCoord());

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	m_symbol.SetCoord(stand->GetCoord());

	if (dist < stand->GetRadius())
	{
		Event* evnt = Event::GetInstance();

		if (evnt->Sluggish(VK_Y))
		{
			if (Settings::GetInstance()->Coin() < stand->FlashCost())
				m_parent->ChangeBehavior("Insufficient");
			else
			{
				if (_Flash())
					m_parent->ChangeBehavior("Flashed");
				else
					m_parent->ChangeBehavior("Error");
			}
		}
		else if (evnt->Sluggish(VK_N))
			m_parent->ChangeBehavior("Cancel");

		m_elapsedTime = 0;
	}
	else
	{
		if (m_elapsedTime > STAND_TIMEOUT)
			m_parent->ChangeBehavior("Idle");
	}

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandFlash::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 0, 288, 40);
	_ApplyFontAttribute();

	rect.SetPos(0, 4);
	drawtext(PROMPT_CONFIRM_FLASH_1, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	rect.SetPos(0, 44);
	drawtext(PROMPT_CONFIRM_FLASH_2, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}

bool StandFlash::_Flash()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Flashback* flashback = stand->GetFlashback();

	flashback->Load();

	if (!flashback->IsValid())
		return false;

	_GeneratePortal();

	return true;
}

void StandFlash::_GeneratePortal()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	// Flashback* flashback = stand->GetFlashback();
	Dungeon* dungeon = static_cast<Dungeon*>(stand->GetScene());
	Portal* portal = MiscLibrary::GetInstance()->
		GetMiscObject<Portal>("Portal")->Clone();

	portal->SetFlashback(stand->GetFlashback());
	portal->SetActive(true);
	portal->SetCoord(dungeon->GetArena()->GetCoord());
	portal->Initialize();

	dungeon->AddObject(portal);
}


/********************************************************************
** StandSaved
*/
const wchar_t StandSaved::PROMPT_SAVED[] = L"Progress saved.";

StandSaved* StandSaved::Clone() const
{
	auto clone = new StandSaved();
	clone->_MakePrototype(false);

	StandBehavior::Clone(clone);

	return clone;
}

void StandSaved::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	m_symbol.SetCoord(stand->GetCoord());

	if (m_elapsedTime > STAND_TIMEOUT)
		m_parent->ChangeBehavior("Idle");

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandSaved::OnEnter()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	stand->IsAvailable(false);

	m_symbol.SetCoord(stand->GetCoord());
	stand->GetSymbol()->SetSubSymbol(&m_symbol);

	m_elapsedTime = 0;

	stand->GetComponent<SoundComponent>()->Play("saved");

	Settings::GetInstance()->AddCoin(-stand->SaveCost());
}

void StandSaved::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 8, 288, 40);
	_ApplyFontAttribute();

	drawtext(PROMPT_SAVED, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}


/********************************************************************
** StandFlashed
*/
const wchar_t StandFlashed::PROMPT_FLASHED[] = L"The Portal is open.";

StandFlashed* StandFlashed::Clone() const
{
	auto clone = new StandFlashed();
	clone->_MakePrototype(false);

	StandBehavior::Clone(clone);

	return clone;
}

void StandFlashed::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	m_symbol.SetCoord(stand->GetCoord());

	if (m_elapsedTime > STAND_TIMEOUT)
		m_parent->ChangeBehavior("Idle");

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandFlashed::OnEnter()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	stand->IsAvailable(false);

	m_symbol.SetCoord(stand->GetCoord());
	stand->GetSymbol()->SetSubSymbol(&m_symbol);

	m_elapsedTime = 0;

	stand->GetComponent<SoundComponent>()->Play("flashed");

	Settings::GetInstance()->AddCoin(-stand->FlashCost());
}

void StandFlashed::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 8, 288, 40);
	_ApplyFontAttribute();

	drawtext(PROMPT_FLASHED, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}

/********************************************************************
** StandCancel
*/
const wchar_t StandCancel::PROMPT_CANCEL[] = L"Deal canceled.";

StandCancel* StandCancel::Clone() const
{
	auto clone = new StandCancel();
	clone->_MakePrototype(false);

	StandBehavior::Clone(clone);

	return clone;
}

void StandCancel::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	m_symbol.SetCoord(stand->GetCoord());

	if (m_elapsedTime > STAND_TIMEOUT)
		m_parent->ChangeBehavior("Idle");

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandCancel::OnEnter()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());

	m_symbol.SetCoord(stand->GetCoord());
	stand->GetSymbol()->SetSubSymbol(&m_symbol);

	m_elapsedTime = 0;

	stand->GetComponent<SoundComponent>()->Play("cancel");
}

void StandCancel::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 8, 288, 40);
	_ApplyFontAttribute();

	drawtext(PROMPT_CANCEL, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}

/********************************************************************
** StandInsufficient
*/
const wchar_t StandInsufficient::PROMPT_INSUFFICIENT[] = L"Insufficient funds.";

StandInsufficient* StandInsufficient::Clone() const
{
	auto clone = new StandInsufficient();
	clone->_MakePrototype(clone);

	StandBehavior::Clone(clone);

	return clone;
}

void StandInsufficient::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	m_symbol.SetCoord(stand->GetCoord());

	if (m_elapsedTime > STAND_TIMEOUT)
		m_parent->ChangeBehavior("Idle");

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandInsufficient::OnEnter()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());

	m_symbol.SetCoord(stand->GetCoord());
	stand->GetSymbol()->SetSubSymbol(&m_symbol);

	m_elapsedTime = 0;

	stand->GetComponent<SoundComponent>()->Play("insuf");
}

void StandInsufficient::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 8, 288, 40);
	_ApplyFontAttribute();

	drawtext(PROMPT_INSUFFICIENT, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}


/********************************************************************
** StandError
*/
const wchar_t StandError::PROMPT_ERROR[] = L"No Flashback!";

StandError* StandError::Clone() const
{
	auto clone = new StandError();
	clone->_MakePrototype(false);

	StandBehavior::Clone(clone);

	return clone;
}

void StandError::Update(Event* evnt)
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());
	Hero* hero = static_cast<Dungeon*>(stand->GetScene())->GetHero();

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	m_symbol.SetCoord(stand->GetCoord());

	if (m_elapsedTime > STAND_TIMEOUT)
		m_parent->ChangeBehavior("Idle");

	_Collide();
	_AdjustDirection(hero->GetCoord().x < stand->GetCoord().x);
}

void StandError::OnEnter()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());

	m_symbol.SetCoord(stand->GetCoord());
	stand->GetSymbol()->SetSubSymbol(&m_symbol);

	m_elapsedTime = 0;

	stand->GetComponent<SoundComponent>()->Play("error");
}

void StandError::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 8, 288, 40);
	_ApplyFontAttribute();

	drawtext(PROMPT_ERROR, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}
