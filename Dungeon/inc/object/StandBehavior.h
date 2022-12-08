/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : StandBehavior.h                           *
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

#ifndef _STAND_BEHAVIOR_H_
#define _STAND_BEHAVIOR_H_

#include <dungine.h>

/********************************************************************
** The behavior relationships are as follows.
** Insufficient
**   |- Choose
**   |    |- ConfirmSave
**   |    |    |- Saved
**   |    |    |- Canceled
*    |    |    |- Insufficient
**   |    |- ConfirmFlash
**   |         |- Flashed
**   |         |- Canceled
**   |         |- Insufficient
**   |- Greeting
*/

class StandBehavior : public Behavior
{
public:
	StandBehavior();
	virtual ~StandBehavior() {}

	virtual const char* Name() const = 0;

	virtual StandBehavior* Clone() const = 0;
	virtual void Clone(StandBehavior* clone) const;

	virtual bool Load(XMLElement* node) { return true; }

	virtual void Update(Event* evnt) = 0;

	virtual void OnEnter() {}
	virtual void OnExit() {}

protected:
	virtual void _ApplyFontAttribute();
	virtual void _RenderDialog() {}

protected:
	enum StandAnimTag
	{
		STAND_ANIM_ACTIVE
	};

	static const std::string DIALOG_RES_ID;
	static const time_t STAND_TIMEOUT;

	time_t m_elapsedTime;
	Symbol m_symbol;		// dialog image
};


/********************************************************************
** StandIdle
*/
class StandIdle : public StandBehavior
{
public:
	StandIdle() {}
	virtual ~StandIdle() {}

	virtual const char* Name() const { return "Idle"; }

	virtual StandIdle* Clone() const;
	virtual void Clone(StandIdle* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();

protected:
	virtual void _RenderDialog() {}

private:
	static const time_t IDLE_DURATION;
};

/********************************************************************
** StandChoose
*/
class StandChoose : public StandBehavior
{
public:
	StandChoose() {}
	virtual ~StandChoose() {}

	virtual const char* Name() const { return "Choose"; }

	virtual StandChoose* Clone() const;
	virtual void Clone(StandChoose* clone) const {}

	virtual void Update(Event* evnt);

	virtual void OnEnter();
	virtual void OnExit();

protected:
	virtual void _RenderDialog();

private:
	static const wchar_t PROMPT_SAVE[];
	static const wchar_t PROMPT_FLASH[];
};

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

	virtual void OnEnter();
	virtual void OnExit();

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

	virtual void OnEnter();
	virtual void OnExit();

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

	virtual void OnEnter();
	virtual void OnExit();

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

	virtual void OnEnter();
	virtual void OnExit();

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

	virtual void OnEnter();
	virtual void OnExit();

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

	virtual void OnEnter();
	virtual void OnExit();

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

	virtual void OnEnter();
	virtual void OnExit();

protected:
	virtual void _RenderDialog();
};

#endif