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
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   All concrete widgets.                                                    *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
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
	virtual ~Button();

	virtual void Update(Event* evnt = nullptr);
	virtual void Draw();
	virtual bool Load(XMLElement* node);

	virtual Button* SetLayer(int layer);
	virtual Button* SetCoord(const Coordinate& coord);
	virtual Button* Translate(const Coordinate& offset);
	virtual Button* SetAlpha(int alpha);
	virtual Button* SetScale(double scale);

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

	Drawer* GetUpDrawer() { return m_pUpDrawer; }
	Drawer* GetDownDrawer() { return m_pDownDrawer; }
	Drawer* GetHoverDrawer() { return m_pHoverDrawer; }
	Drawer* GetDisabledDrawer() { return m_pDisabledDrawer; }

	/*
	** The four states share a common trigger.
	*/
	Button* SetTrigger(Trigger* trigger);
	Trigger* GetTrigger() { return m_pTrigger; }

	Button* SetHoverSound(Buzzer* buzzer);
	Buzzer* GetHoverSound() { return m_pHoverSound; }
	Button* SetClickSound(Buzzer* buzzer);
	Buzzer* GetClickSound() { return m_pClickSound; }

	Button* Disable(bool isDisabled)
	{
		m_isDisabled = isDisabled;
		return this;
	}
	bool IsDisabled() const { return m_isDisabled; }

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

	bool IsPressed() const { return m_isPressed; }
	bool IsHover() const { return m_isHover; }

private:
	virtual void _ProcessInput(Event& evnt);

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
	virtual ~Slider();

	virtual void Update(Event* evnt = nullptr);
	virtual void Draw();
	virtual bool Load(XMLElement* node);

	virtual Slider* SetLayer(int layer);
	virtual Slider* SetCoord(const Coordinate& coord);
	virtual Slider* Translate(const Coordinate& offset);
	virtual Slider* SetAlpha(int alpha);
	virtual Slider* SetScale(double scale);

	/*
	** Drag bar consists of a block and a bar. Be aware that the
	** block is a button whose position must center its coordinate.
	** So the block should be set individually outside the dragbar.
	** And the bar should better not be a circle. :)
	*/
	Slider* SetBlock(Button* block);
	Slider* SetBarDrawer(Drawer* drawer);

	Button* GetBlock() { return m_pBlock; }
	Drawer* GetBarDrawer() { return m_pBarDrawer; }

	Slider* SetBarTrigger(Trigger* trigger);
	Trigger* GetBarTrigger() { return m_pBarTrigger; }

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
	double GetValue() const { return m_value; }

private:
	virtual void _ProcessInput(Event& evnt);

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
	virtual ~ProgressBar();

	virtual void Update(Event* evnt = nullptr);
	virtual void Draw();
	virtual bool Load(XMLElement* node);

	virtual ProgressBar* SetLayer(int layer);
	virtual ProgressBar* SetCoord(const Coordinate& coord);
	virtual ProgressBar* Translate(const Coordinate& offset);
	virtual ProgressBar* SetAlpha(int alpha);
	virtual ProgressBar* SetScale(double scale);

	/*
	** A progress bar is composed of a bar and a stuff...:?
	*/
	ProgressBar* SetBarDrawer(Drawer* drawer);
	ProgressBar* SetStuffDrawer(Drawer* drawer);

	Drawer* GetBarDrawer() { return m_pBarDrawer; }
	Drawer* GetStuffDrawer() { return m_pStuffDrawer; }

	ProgressBar* SetPadding(int padding);
	int GetPadding() const { return m_padding; }

	void SetValue(double value)
	{
		value = max(value, 0.0);
		value = min(value, 1.0);
		m_value = value;
	}
	double GetValue(double value) const { return m_value; }

private:
	virtual void _ProcessInput(Event& evnt) {}	// No interaction.

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
	virtual ~RadioBox();

	virtual void Update(Event* evnt = nullptr);
	virtual void Draw();
	virtual bool Load(XMLElement* node);

	virtual RadioBox* SetLayer(int layer);
	virtual RadioBox* SetCoord(const Coordinate& coord);
	virtual RadioBox* Translate(const Coordinate& offset);
	virtual RadioBox* SetAlpha(int alpha);
	virtual RadioBox* SetScale(double scale);

	RadioBox* SetParentList(RadioList* parent)
	{
		m_parent = parent;
		return this;
	}
	RadioList* GetParentList() { return m_parent; }

	/*
	** Radio box is composed of two parts, the box and the
	** stuff. The box has two state, normal and hover. And
	** the stuff will only display when the radio is checked.
	*/
	RadioBox* SetBox(Button* box);
	RadioBox* SetMarkDrawer(Drawer* drawer);

	Button* GetBox() { return m_pBox; }
	Drawer* GetMarkDrawer() { return m_pMarkDrawer; }

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
	bool GetState() const { return m_isChecked; };

private:
	virtual void _ProcessInput(Event& evnt);

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
	virtual ~RadioList();

	virtual void Update(Event* evnt = nullptr);
	virtual void Draw();
	virtual bool Load(XMLElement* node);
	virtual RadioList* ResetTransition();

	virtual RadioList* SetLayer(int layer);
	virtual RadioList* SetCoord(const Coordinate& coord);
	virtual RadioList* Translate(const Coordinate& offset);
	virtual RadioList* SetAlpha(int alpha);
	virtual RadioList* SetScale(double scale);

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
	virtual void _ProcessInput(Event& evnt) {}

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
	virtual ~StaticWidget();

	virtual void Update(Event* evnt = nullptr);
	virtual void Draw();
	virtual bool Load(XMLElement* node);

	virtual StaticWidget* SetLayer(int layer);
	virtual StaticWidget* SetCoord(const Coordinate& coord);
	virtual StaticWidget* Translate(const Coordinate& offset);
	virtual StaticWidget* SetAlpha(int alpha);
	virtual StaticWidget* SetScale(double scale);

	StaticWidget* SetDrawer(Drawer* drawer);
	Drawer* GetDrawer() const { return m_pDrawer; }

private:
	virtual void _ProcessInput(Event& evnt) {}

	Drawer* m_pDrawer;
};


/********************************************************************
** Detector only detects keyboard event.
*/
class KeyboardDetector : public AbstractWidget
{
public:
	KeyboardDetector();
	virtual ~KeyboardDetector();

	virtual void Update(Event* evnt);
	virtual bool Load(XMLElement* node);

	KeyboardDetector* SetTrigger(KeyboardTrigger* trigger);
	KeyboardTrigger* GetTrigger() { return m_pTrigger; }

	KeyboardDetector* OnTriggered(std::function<void(void)> callback)
	{
		m_pOnTriggered = callback;
		return this;
	}

private:
	virtual void _ProcessInput(Event& evnt);

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
	virtual ~AudioPlayer();

	virtual void Update(Event* evnt);
	virtual bool Load(XMLElement* node);

	void Play();
	void Pause(bool isPaused);
	void Stop();

protected:
	PlayList* m_playList;
};


class StandardWidgetKit
{
public:
	StandardWidgetKit() {}
	virtual ~StandardWidgetKit() {}

	AbstractWidget* LoadWidget(XMLElement* node);
};


#endif