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
 * Overview:                                                                 *
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
**   |    |    |- Cancel
*    |    |    |- Insufficient
**   |    |- ConfirmFlash
*    |         |- Error(No Valid Flashback)
**   |         |- Flashed
**   |         |- Cancel
**   |         |- Insufficient
**   |- Greeting
*/

class StandBehavior : public Behavior
{
public:
    StandBehavior();

    ~StandBehavior() override
    {
    }

    const char* Name() const override = 0;

    StandBehavior* Clone() const override = 0;
    virtual void Clone(StandBehavior* clone) const;

    bool Load(XMLElement* node) override
    {
        return true;
    }

    void Update(Event* evnt) override = 0;

    void OnEnter() override;
    void OnExit() override;

protected:
    void _AdjustDirection(bool isLeft);
    virtual void _ApplyFontAttribute();

    virtual void _RenderDialog()
    {
    }

    /*
    ** The stand is too large, that it gets out of the border
    ** range of hero, which cause the latter not detect it
    ** correctly. So it should detect collision actively.
    */
    virtual void _Collide();

protected:
    enum StandAnimTag
    {
        STAND_ANIM_ACTIVE
    };

    static const std::string DIALOG_RES_ID;
    static const time_t STAND_TIMEOUT;

    time_t m_elapsedTime;
    Symbol m_symbol; // dialog image
};

/********************************************************************
** StandIdle
*/
class StandIdle : public StandBehavior
{
public:
    StandIdle()
    {
        _RenderDialog();
    }

    ~StandIdle() override
    {
    }

    const char* Name() const override
    {
        return "Idle";
    }

    StandIdle* Clone() const override;

    virtual void Clone(StandIdle* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

    void OnExit() override
    {
    }

protected:
    void _RenderDialog() override
    {
    }

private:
    static const time_t IDLE_DURATION;
};

/********************************************************************
** StandChoose
*/
class StandChoose : public StandBehavior
{
public:
    StandChoose()
    {
        _RenderDialog();
    }

    ~StandChoose() override
    {
    }

    const char* Name() const override
    {
        return "Choose";
    }

    StandChoose* Clone() const override;

    virtual void Clone(StandChoose* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    void _RenderDialog() override;

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
    StandGreet()
    {
        _RenderDialog();
    }

    ~StandGreet() override
    {
    }

    const char* Name() const override
    {
        return "Greet";
    }

    StandGreet* Clone() const override;

    virtual void Clone(StandGreet* clone) const
    {
    }

    void Update(Event* evnt) override;

protected:
    void _RenderDialog() override;

private:
    static const wchar_t PROMPT_GREET[];
};

/********************************************************************
** StandConfirmSave
*/
class StandSave : public StandBehavior
{
public:
    StandSave()
    {
        _RenderDialog();
    }

    ~StandSave() override
    {
    }

    const char* Name() const override
    {
        return "Save";
    }

    StandSave* Clone() const override;

    virtual void Clone(StandSave* clone) const
    {
    }

    void Update(Event* evnt) override;

protected:
    void _RenderDialog() override;

private:
    static const wchar_t PROMPT_CONFIRM_SAVE_1[];
    static const wchar_t PROMPT_CONFIRM_SAVE_2[];

    bool _Save();
};

/********************************************************************
** StandFlash
*/
class StandFlash : public StandBehavior
{
public:
    StandFlash()
    {
        _RenderDialog();
    }

    ~StandFlash() override
    {
    }

    const char* Name() const override
    {
        return "Flash";
    }

    StandFlash* Clone() const override;

    virtual void Clone(StandFlash* clone) const
    {
    }

    void Update(Event* evnt) override;

protected:
    void _RenderDialog() override;

private:
    static const wchar_t PROMPT_CONFIRM_FLASH_1[];
    static const wchar_t PROMPT_CONFIRM_FLASH_2[];

    bool _Flash();
    void _GeneratePortal();
};

/********************************************************************
** StandSaved
*/
class StandSaved : public StandBehavior
{
public:
    StandSaved()
    {
        _RenderDialog();
    }

    ~StandSaved() override
    {
    }

    const char* Name() const override
    {
        return "Saved";
    }

    StandSaved* Clone() const override;

    virtual void Clone(StandSaved* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    void _RenderDialog() override;

private:
    static const wchar_t PROMPT_SAVED[];

    void _CostCoin();
};

/********************************************************************
** StandFlashed
*/
class StandFlashed : public StandBehavior
{
public:
    StandFlashed()
    {
        _RenderDialog();
    }

    ~StandFlashed() override
    {
    }

    const char* Name() const override
    {
        return "Flashed";
    }

    StandFlashed* Clone() const override;

    virtual void Clone(StandFlashed* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    void _RenderDialog() override;

private:
    static const wchar_t PROMPT_FLASHED[];
};

/********************************************************************
** StandCancel
*/
class StandCancel : public StandBehavior
{
public:
    StandCancel()
    {
        _RenderDialog();
    }

    ~StandCancel() override
    {
    }

    const char* Name() const override
    {
        return "Cancel";
    }

    StandCancel* Clone() const override;

    virtual void Clone(StandCancel* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    void _RenderDialog() override;

private:
    static const wchar_t PROMPT_CANCEL[];
};

/********************************************************************
** StandInsufficient
*/
class StandInsufficient : public StandBehavior
{
public:
    StandInsufficient()
    {
        _RenderDialog();
    }

    ~StandInsufficient() override
    {
    }

    const char* Name() const override
    {
        return "Insufficient";
    }

    StandInsufficient* Clone() const override;

    virtual void Clone(StandInsufficient* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    void _RenderDialog() override;

private:
    static const wchar_t PROMPT_INSUFFICIENT[];
};

/********************************************************************
** StandError
*/
class StandError : public StandBehavior
{
public:
    StandError()
    {
        _RenderDialog();
    }

    ~StandError() override
    {
    }

    const char* Name() const override
    {
        return "Error";
    }

    StandError* Clone() const override;

    virtual void Clone(StandError* clone) const
    {
    }

    void Update(Event* evnt) override;

    void OnEnter() override;

protected:
    void _RenderDialog() override;

private:
    static const wchar_t PROMPT_ERROR[];
};

#endif
