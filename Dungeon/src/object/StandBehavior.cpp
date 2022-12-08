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

#include "../../inc/object/Stand.h"
#include "../../inc/object/StandBehavior.h"

#include "../../inc/object/Hero.h"


static const Coordinate DIALOG_OFFSET = { -72, 144 };

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
	_RenderDialog();
}

void StandBehavior::Clone(StandBehavior* clone) const
{
	Behavior::Clone(clone);

	clone->m_elapsedTime = m_elapsedTime;
	clone->m_symbol = m_symbol;
}

void StandBehavior::_ApplyFontAttribute()
{
	settextcolor(BLACK);
	settextstyle(18, 0, INTERNAL_FONT_NAME);
	setbkcolor(TRANSPARENT_COLOR);
}


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** StandIdle
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
const time_t StandIdle::IDLE_DURATION = 30000;

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

	// Hmm... it won't overflow, huh?
	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	if (dist < stand->GetRadius())
	{
		if (stand->IsActive())
			m_parent->ChangeBehavior("Choose");
		else
		{
			if (m_elapsedTime >= IDLE_DURATION)
				m_parent->ChangeBehavior("Greet");
		}
	}
}

void StandIdle::OnEnter()
{
	auto anim = m_parent->GetGameObject()->GetComponent<AnimComponent>()
		->GetAnim();

	anim->SetMotion(STAND_ANIM_ACTIVE);
	anim->SetDir(ANIM_LEFT);	// Actually it doesn't have direction.
}

void StandIdle::OnExit()
{
	// Clear elapsed time for interval, I guess.
	m_elapsedTime = 0;
}


/********************************************************************
** StandChoose
*/
const wchar_t StandChoose::PROMPT_SAVE[]	= L"Save progress? (V)";
const wchar_t StandChoose::PROMPT_FLASH[]	= L"Flashback? (F)    ";

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

	m_elapsedTime += Timer::GetInstance()->GetDeltaTimestamp();

	if (dist < stand->GetRadius())
	{
		m_elapsedTime = 0;

	}
}

void StandChoose::OnEnter()
{
	Stand* stand = static_cast<Stand*>(m_parent->GetGameObject());

	m_symbol.SetCoord(stand->GetCoord());
	stand->GetSymbol()->SetSubSymbol(&m_symbol);

	m_elapsedTime = 0;
}

void StandChoose::OnExit()
{
	static_cast<Stand*>(m_parent->GetGameObject())
		->GetSymbol()->SetSubSymbol(nullptr);
}

void StandChoose::_RenderDialog()
{
	ImageResource* res = LoadResource<ImageResource>(DIALOG_RES_ID);
	IMAGE* image = new IMAGE(*(res->GetResource()));

	Device::GetInstance()->SetTargetImage(image);

	Rect rect(0, 0, 144, 20);
	_ApplyFontAttribute();

	rect.SetPos(0, 2);
	drawtext(PROMPT_SAVE, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
	rect.SetPos(0, 23);
	drawtext(PROMPT_FLASH, rect.GetEasyXRect(), DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

	Device::GetInstance()->SetTargetImage();

	m_symbol.SetImage(image);
}


/********************************************************************
** StandGreet
*/
class StandGreet : public StandBehavior
{
public:
	StandGreet();
	virtual ~StandGreet() {}

	virtual const char* Name() const { return "Greet"; }

	virtual StandGreet* Clone() const;
	virtual void Clone(StandGreet* clone) const {}

	virtual void Update(Event* evnt);

protected:
	virtual void _RenderDialog();
};

/********************************************************************
** StandConfirmSave
*/
class StandSave : public StandBehavior
{
public:
	StandSave();
	virtual ~StandSave() {}

	virtual const char* Name() const { return "Save"; }

	virtual StandSave* Clone() const;
	virtual void Clone(StandSave* clone) const {}

	virtual void Update(Event* evnt);

protected:
	virtual void _RenderDialog();
};

/********************************************************************
** StandFlash
*/
class StandFlash : public StandBehavior
{
public:
	StandFlash();
	virtual ~StandFlash() {}

	virtual const char* Name() const { return "Flash"; }

	virtual StandFlash* Clone() const;
	virtual void Clone(StandFlash* clone) const {}

	virtual void Update(Event* evnt);

protected:
	virtual void _RenderDialog();
};

/********************************************************************
** StandSaved
*/
class StandSaved : public StandBehavior
{
public:
	StandSaved();
	virtual ~StandSaved() {}

	virtual const char* Name() const { return "Saved"; }

	virtual StandSaved* Clone() const;
	virtual void Clone(StandSaved* clone) const {}

	virtual void Update(Event* evnt);

protected:
	virtual void _RenderDialog();
};

/********************************************************************
** StandFlashed
*/
class StandFlashed : public StandBehavior
{
public:
	StandFlashed();
	virtual ~StandFlashed() {}

	virtual const char* Name() const { return "Flashed"; }

	virtual StandFlashed* Clone() const;
	virtual void Clone(StandFlashed* clone) const {}

	virtual void Update(Event* evnt);

protected:
	virtual void _RenderDialog();
};

/********************************************************************
** StandInsufficient
*/
class StandInsufficient : public StandBehavior
{
public:
	StandInsufficient();
	virtual ~StandInsufficient() {}

	virtual const char* Name() const { return "Insufficient"; }

	virtual StandInsufficient* Clone() const;
	virtual void Clone(StandInsufficient* clone) const {}

	virtual void Update(Event* evnt);

protected:
	virtual void _RenderDialog();
};

/********************************************************************
** StandComplete
*/
class StandComplete : public StandBehavior
{
public:
	StandComplete();
	virtual ~StandComplete() {}

	virtual const char* Name() const { return "Complete"; }

	virtual StandComplete* Clone() const;
	virtual void Clone(StandComplete* clone) const {}

	virtual void Update(Event* evnt);

protected:
	virtual void _RenderDialog();
};

#endif