/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Widget.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 17, 2022                             *
 *                                                                            *
 *                    Last Update : November 25, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   All concrete widgets.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <functional>
#include <map>

#include "AbstractWidget.h"
#include "Buzzer.h"
#include "Drawer.h"
#include "Trigger.h"

/********************************************************************
** An ordinary button.
*/
class Button : public VisualWidget
{
public:
    Button();
    ~Button() override;

    void Update(Event* evnt = nullptr) override;
    void Draw() override;
    bool Load(XMLElement* node) override;

    Button* SetLayer(int layer) override;
    Button* SetCoord(const Coordinate& coord) override;
    Button* Translate(const Coordinate& offset) override;
    Button* SetAlpha(int alpha) override;
    Button* SetScale(double scale) override;
    Button* SetRotationAngle(double angle) override;

    /*
    ** Button now has four states,
    ** Up       -- Normal.
    ** Down     -- Pressed(mouse must on the button).
    ** Hover    -- Mouse is over the button.
    ** Disabled -- Button is inactivated.
    */
    Button* SetUpDrawer(Drawer* drawer);
    Button* SetDownDrawer(Drawer* drawer);
    Button* SetHoverDrawer(Drawer* drawer);
    Button* SetDisabledDrawer(Drawer* drawer);

    Drawer* GetUpDrawer()
    {
        return m_pUpDrawer;
    }

    Drawer* GetDownDrawer()
    {
        return m_pDownDrawer;
    }

    Drawer* GetHoverDrawer()
    {
        return m_pHoverDrawer;
    }

    Drawer* GetDisabledDrawer()
    {
        return m_pDisabledDrawer;
    }

    /*
    ** The four states share a common trigger.
    */
    Button* SetTrigger(Trigger* trigger);

    Trigger* GetTrigger()
    {
        return m_pTrigger;
    }

    Button* SetHoverSound(Buzzer* buzzer);

    Buzzer* GetHoverSound()
    {
        return m_pHoverSound;
    }

    Button* SetClickSound(Buzzer* buzzer);

    Buzzer* GetClickSound()
    {
        return m_pClickSound;
    }

    Button* Disable(bool isDisabled)
    {
        m_isDisabled = isDisabled;
        return this;
    }

    bool IsDisabled() const
    {
        return m_isDisabled;
    }

    /*
    ** The callback function of the button. It will
    ** be called... Just the literal meaning. :)
    ** sizeof(function<void(void)>) = 64... ?
    */
    Button* OnClick(std::function<void(void)> callback)
    {
        m_pOnClick = callback;
        return this;
    };

    bool IsPressed() const
    {
        return m_isPressed;
    }

    bool IsHover() const
    {
        return m_isHover;
    }

private:
    void _ProcessInput(Event& evnt) override;

    /*
    ** Four states.
    */
    Drawer* m_pUpDrawer;
    Drawer* m_pDownDrawer;
    Drawer* m_pHoverDrawer;
    Drawer* m_pDisabledDrawer;

    Buzzer* m_pHoverSound;
    Buzzer* m_pClickSound;

    /*
    ** The trigger binded to it.
    */
    Trigger* m_pTrigger;

    std::function<void(void)> m_pOnClick;

    /*
    ** Current state.
    */
    bool m_isHover : 1;
    bool m_isPressed : 1;
    bool m_isDisabled : 1;
};

/********************************************************************
** A fancy drag bar. To set volume or else. It has a button as the
** drag block.
** Notice that block should center its coordinate. For now, it must
** be assigned manually. :(
*/
class Slider : public VisualWidget
{
public:
    Slider();
    ~Slider() override;

    void Update(Event* evnt = nullptr) override;
    void Draw() override;
    bool Load(XMLElement* node) override;

    Slider* SetLayer(int layer) override;
    Slider* SetCoord(const Coordinate& coord) override;
    Slider* Translate(const Coordinate& offset) override;
    Slider* SetAlpha(int alpha) override;
    Slider* SetScale(double scale) override;
    Slider* SetRotationAngle(double angle) override;

    /*
    ** Drag bar consists of a block and a bar. Be aware that the
    ** block is a button whose position must center its coordinate.
    ** So the block should be set individually outside the dragbar.
    ** And the bar should better not be a circle. :)
    */
    Slider* SetBlock(Button* block);
    Slider* SetBarDrawer(Drawer* drawer);

    Button* GetBlock()
    {
        return m_pBlock;
    }

    Drawer* GetBarDrawer()
    {
        return m_pBarDrawer;
    }

    Slider* SetBarTrigger(Trigger* trigger);

    Trigger* GetBarTrigger()
    {
        return m_pBarTrigger;
    }

    /*
    ** When the block is dragged, or the bar is pressed(not clicked).
    */
    Slider* OnChange(std::function<void(double)> callback)
    {
        m_pOnChange = callback;
        return this;
    }

    /*
    ** When the drag bar loses focus. This feature is now
    ** on hold
    *//*
	Slider* OnBlur(std::function<void(double)> callback)
	{
		m_pOnBlur = callback;
		return this;
	}
	*/

    void SetValue(double value);

    double GetValue() const
    {
        return m_value;
    }

private:
    void _AdjustBarCoord();
    void _ProcessInput(Event& evnt) override;

    Button* m_pBlock;
    Drawer* m_pBarDrawer;

    Trigger* m_pBarTrigger;

    std::function<void(double)> m_pOnChange;

    /*
    std::function<void(double)> m_pOnBlur;
    */

    int m_barWidth;
    int m_barHeight;

    /*
    ** value is between 0 and 1.
    */
    double m_value;
};

/********************************************************************
** The most fancy progress bar. It doesn't interact with users.
*/
class ProgressBar : public VisualWidget
{
public:
    ProgressBar();
    ~ProgressBar() override;

    void Update(Event* evnt = nullptr) override;
    void Draw() override;
    bool Load(XMLElement* node) override;

    ProgressBar* SetLayer(int layer) override;
    ProgressBar* SetCoord(const Coordinate& coord) override;
    ProgressBar* Translate(const Coordinate& offset) override;
    ProgressBar* SetAlpha(int alpha) override;
    ProgressBar* SetScale(double scale) override;
    ProgressBar* SetRotationAngle(double angle) override;

    /*
    ** A progress bar is composed of a bar and a stuff...:?
    */
    ProgressBar* SetBarDrawer(Drawer* drawer);
    ProgressBar* SetStuffDrawer(Drawer* drawer);

    Drawer* GetBarDrawer()
    {
        return m_pBarDrawer;
    }

    Drawer* GetStuffDrawer()
    {
        return m_pStuffDrawer;
    }

    ProgressBar* SetPadding(int padding);

    int GetPadding() const
    {
        return m_padding;
    }

    void SetValue(double value)
    {
        value = max(value, 0.0);
        value = min(value, 1.0);
        m_value = value;
    }

    double GetValue(double value) const
    {
        return m_value;
    }

private:
    void _ProcessInput(Event& evnt) override
    {
    } // No interaction.

    Drawer* m_pBarDrawer;
    Drawer* m_pStuffDrawer;

    double m_value;

    int m_barWidth;
    int m_barHeight;

    /*
    ** Padding is... actually the border width of the
    ** bar, but I like this name. :)
    */
    int m_padding;
};

/********************************************************************
** Emm, radio button. If it can play alone or be added to a hmm...
** RadioList.
*/
class RadioList;

class RadioBox : public VisualWidget
{
public:
    RadioBox();
    ~RadioBox() override;

    void Update(Event* evnt = nullptr) override;
    void Draw() override;
    bool Load(XMLElement* node) override;

    RadioBox* SetLayer(int layer) override;
    RadioBox* SetCoord(const Coordinate& coord) override;
    RadioBox* Translate(const Coordinate& offset) override;
    RadioBox* SetAlpha(int alpha) override;
    RadioBox* SetScale(double scale) override;
    RadioBox* SetRotationAngle(double angle) override;

    RadioBox* SetParentList(RadioList* parent)
    {
        m_parent = parent;
        return this;
    }

    RadioList* GetParentList()
    {
        return m_parent;
    }

    /*
    ** Radio box is composed of two parts, the box and the
    ** stuff. The box has two state, normal and hover. And
    ** the stuff will only display when the radio is checked.
    */
    RadioBox* SetBox(Button* box);
    RadioBox* SetMarkDrawer(Drawer* drawer);

    Button* GetBox()
    {
        return m_pBox;
    }

    Drawer* GetMarkDrawer()
    {
        return m_pMarkDrawer;
    }

    /*
    ** This will be called every time the radio box is checked,
    ** whether is turned on or off. It will sent its state after
    ** the event to the callback.
    */
    RadioBox* OnCheck(std::function<void(bool)> callback)
    {
        m_pOnCheck = callback;
        return this;
    }

    void SetState(bool isChecked);

    bool GetState() const
    {
        return m_isChecked;
    };

private:
    void _ProcessInput(Event& evnt) override;

    Button* m_pBox;
    Drawer* m_pMarkDrawer;

    RadioList* m_parent;

    std::function<void(bool)> m_pOnCheck;

    /*
    ** Whether the radio can be canceled from selection.
    ** If a radio is attached to a list, it shouldn't be
    ** be canceled, or no choice will be selected.
    */
    bool m_isCancelable : 1;
    bool m_isPressed : 1;
    bool m_isChecked : 1;
};

/********************************************************************
** RadioList contains some radioboxes and will update others if one
** is checked. If a radio box is added to a radio list, it should
** no longer be managed by WidgetManager. It is a tiny manager. :)
*/
class RadioList : public VisualWidget
{
public:
    RadioList();
    ~RadioList() override;

    void Update(Event* evnt = nullptr) override;
    void Draw() override;
    bool Load(XMLElement* node) override;
    RadioList* ResetTransition() override;

    RadioList* SetLayer(int layer) override;
    RadioList* SetCoord(const Coordinate& coord) override;
    RadioList* Translate(const Coordinate& offset) override;
    RadioList* SetAlpha(int alpha) override;
    RadioList* SetScale(double scale) override;
    RadioList* SetRotationAngle(double angle) override;

    RadioBox* AddRadio(const std::string& name, RadioBox* radio);
    RadioBox* GetRadio(const std::string& name);

    RadioBox* operator[](const std::string& name)
    {
        return m_radios[name];
    }

    /*
    ** If no parameter, it will check the first radio by default.
    */
    void Check();
    void Check(RadioBox* radio);

private:
    /*
     ** RadioList itself doesn't have to process input. This
     ** is done in Update() by its child radios.
     */
    void _ProcessInput(Event& evnt) override
    {
    }

    std::map<std::string, RadioBox*> m_radios;
};

/********************************************************************
** Text area still only supports single line input... :(
** TODO: THIS WILL BE CONSIDERED LATER...
*/
#if 0
class TextArea : public VisualWidget
{
public:
	TextArea();
	virtual ~TextArea();

	virtual void ProcessInput();
	virtual void Draw();
	virtual bool Load(XMLElement* node);

	virtual VisualWidget* SetCoord(const Coordinate& coord);

	virtual VisualWidget* SetAlpha(int alpha);

	TextArea* SetText(const std::wstring& text)
	{
		m_text = text;
		return this;
	}
	const std::wstring& GetText() const { return m_text; }

	TextArea* SetCursorColor(COLORREF color);
	TextArea* SetPadding(int normalPadding, int selectedPadding);

	/*
	** Although here is... Drawer, but only support rectangle
	** and round rectangle. :[
	*/
	TextArea* SetBoxDrawer(Drawer* drawer);
	Drawer* GetBoxDrawer() { return m_pBoxDrawer; }

	TextArea* OnChange(const std::function<void(const std::wstring&)>& callback)
	{
		m_pOnChange = callback;
		return this;
	}
	TextArea* OnBlur(const std::function<void(const std::wstring&)>& callback)
	{
		m_pOnBlur = callback;
		return this;
	}

	TextArea* SetTextStyle(
		int fontHeight = 0,
		int fontWidth = 0,
		COLORREF fontColor = WHITE);

private:
	/*
	** Generate cursor drawer, text drawer and trigger based on the
	** box drawer that is assigned manually.
	*/
	void _Generate();

	std::wstring m_text;

	Drawer* m_pBoxDrawer;

	/*
	** Cursor drawer and text drawer are generated by
	** the class itself.
	*/
	RectDrawer* m_pCursorDrawer;
	TextDrawer* m_pTextDrawer;

	/*
	** Trigger will also be generated by the class itself.
	*/
	Trigger* m_pTrigger;

	std::function<void(const std::wstring&)> m_pOnChange;
	std::function<void(const std::wstring&)> m_pOnBlur;

	int m_width;
	int m_height;

	/*
	** If is selected, then it will have a slightly different
	** style. :P
	*/
	int m_normalPadding;
	int m_selectedPadding;

	/*
	** Last time when the cursor change its visibility. It need
	** the timing module to update.
	*/
	clock_t m_oldCursorTime;

	bool m_isSelected : 1;

	/*
	** Whether to show the cursor or not. This is for the
	** blink effect.
	*/
	bool m_showCursor : 1;
};
#endif // #if 0 TextArea

/********************************************************************
** Static widget is static(> <) and has no interaction with users.
** But with the help of Transform class, it can become dynamic. :)
*/
class StaticWidget : public VisualWidget
{
public:
    StaticWidget();
    ~StaticWidget() override;

    void Update(Event* evnt = nullptr) override;
    void Draw() override;
    bool Load(XMLElement* node) override;

    StaticWidget* SetLayer(int layer) override;
    StaticWidget* SetCoord(const Coordinate& coord) override;
    StaticWidget* Translate(const Coordinate& offset) override;
    StaticWidget* SetAlpha(int alpha) override;
    StaticWidget* SetScale(double scale) override;
    StaticWidget* SetRotationAngle(double angle) override;

    StaticWidget* SetDrawer(Drawer* drawer);

    Drawer* GetDrawer() const
    {
        return m_pDrawer;
    }

private:
    void _ProcessInput(Event& evnt) override
    {
    }

    Drawer* m_pDrawer;
};

/********************************************************************
** Detector only detects keyboard event.
*/
class KeyboardDetector : public AbstractWidget
{
public:
    KeyboardDetector();
    ~KeyboardDetector() override;

    void Update(Event* evnt) override;
    bool Load(XMLElement* node) override;

    KeyboardDetector* SetTrigger(KeyboardTrigger* trigger);

    KeyboardTrigger* GetTrigger()
    {
        return m_pTrigger;
    }

    KeyboardDetector* OnTriggered(std::function<void(void)> callback)
    {
        m_pOnTriggered = callback;
        return this;
    }

private:
    void _ProcessInput(Event& evnt) override;

    std::function<void(void)> m_pOnTriggered;
    KeyboardTrigger* m_pTrigger;
    bool m_isTriggered;
};

/********************************************************************
** AudioPlayer only plays audio stream. Unfortunately, it is likely
** to hold its resource through the entire game life.
*/
class PlayList;

class AudioPlayer : public AbstractWidget
{
public:
    AudioPlayer();
    ~AudioPlayer() override;

    void Update(Event* evnt) override;
    bool Load(XMLElement* node) override;

    void Play();
    void Pause(bool isPaused);
    void Stop();

protected:
    PlayList* m_playList;
};

class StandardWidgetKit
{
public:
    StandardWidgetKit()
    {
    }

    virtual ~StandardWidgetKit()
    {
    }

    virtual AbstractWidget* LoadWidget(XMLElement* node);
};

#endif
