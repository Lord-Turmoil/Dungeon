/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Widget.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 17, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
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

#include <cstring>

#include "../../inc/ui/Event.h"
#include "../../inc/ui/Transition.h"
#include "../../inc/ui/Widget.h"

#include "../../inc/device/Explorer.h"
#include "../../inc/device/Sound.h"
#include "../../inc/device/Speaker.h"

#include "../../inc/utility/Auxilliary.h"
#include "../../inc/utility/Parser.h"

/******************************************************************************
 * Button::Button -- Constructor of the object.                               *
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
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button::Button()
    : VisualWidget(WidgetType::BUTTON), m_pUpDrawer(nullptr), m_pDownDrawer(nullptr), m_pHoverDrawer(nullptr),
      m_pDisabledDrawer(nullptr), m_pHoverSound(nullptr), m_pClickSound(nullptr), m_pTrigger(nullptr), m_isHover(false),
      m_isPressed(false), m_isDisabled(false)
{
}

/******************************************************************************
 * Button::~Button -- Destructor of the object.                               *
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
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button::~Button()
{
    _DELETE(m_pUpDrawer);
    _DELETE(m_pDownDrawer);
    _DELETE(m_pHoverDrawer);
    _DELETE(m_pDisabledDrawer);

    _DELETE(m_pTrigger);

    _DELETE(m_pClickSound);
}

/******************************************************************************
 * Button::Update -- Update the button.                                       *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt -- The event to handle. nullptr for no event.                *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
void Button::Update(Event* evnt)
{
    if (m_isDisabled || !m_isActivated)
    {
        return;
    }

    // Update drawer and trigger.
    if (m_pUpDrawer)
    {
        m_pUpDrawer->Update();
    }
    if (m_pDownDrawer)
    {
        m_pDownDrawer->Update();
    }
    if (m_pHoverDrawer)
    {
        m_pHoverDrawer->Update();
    }
    if (m_pDisabledDrawer)
    {
        m_pDisabledDrawer->Update();
    }

    if (m_pTrigger)
    {
        m_pTrigger->Update();
    }

    _UpdateTransition();

    if (evnt)
    {
        _ProcessInput(*evnt);
    }
}

/******************************************************************************
 * Button::Draw -- Draw the button.                                           *
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
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
void Button::Draw()
{
    if (!m_isActivated)
    {
        return;
    }

    if (m_isDisabled && m_pDisabledDrawer)
    {
        m_pDisabledDrawer->Draw();
    }
    else if (m_isPressed && m_pDownDrawer)
    {
        m_pDownDrawer->Draw();
    }
    else if (m_isHover && m_pHoverDrawer)
    {
        m_pHoverDrawer->Draw();
    }
    else if (m_pUpDrawer)
    {
        m_pUpDrawer->Draw();
    }
}

/******************************************************************************
 * Button::Load -- Load button.                                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
bool Button::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;
    XMLElement* tag;
    bool value;

    _CHECK_TAG("Button");
    _RETURN_IF_ERROR();

    VisualWidget::Load(node);

    value = false;
    _PARSE("disabled", value, name, "false");
    m_isDisabled = value;

    tag = node->FirstChildElement();
    while (tag)
    {
        attr = tag->Name();
        if (_STR_SAME(attr, "Up"))
        {
            SetUpDrawer(LoadDrawer(tag->FirstChildElement()));
        }
        else if (_STR_SAME(attr, "Down"))
        {
            SetDownDrawer(LoadDrawer(tag->FirstChildElement()));
        }
        else if (_STR_SAME(attr, "Hover"))
        {
            SetHoverDrawer(LoadDrawer(tag->FirstChildElement()));
        }
        else if (_STR_SAME(attr, "Disabled"))
        {
            SetDisabledDrawer(LoadDrawer(tag->FirstChildElement()));
        }
        else if (_STR_SAME(attr, "Trigger"))
        {
            SetTrigger(LoadTrigger(tag->FirstChildElement()));
        }
        else if (_STR_SAME(attr, "ClickSound"))
        {
            SetClickSound(LoadBuzzer(tag->FirstChildElement()));
        }
        else if (_STR_SAME(attr, "HoverSound"))
        {
            SetHoverSound(LoadBuzzer(tag->FirstChildElement()));
        }

        tag = tag->NextSiblingElement();
    }

    SetLayer(m_layer);

    /*
    ** Here... I took a shortcut. :P
    */
    Coordinate offset = m_coord;
    m_coord = COORD_ZERO;
    Translate(offset);

    VisualWidget::LoadAttribute(node);

    _RETURN_STATE();
}

/******************************************************************************
 * Button::SetLayer -- Set the layer of the button.                           *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetLayer(int layer)
{
    if (m_pUpDrawer)
    {
        m_pUpDrawer->SetLayer(layer);
    }
    if (m_pDownDrawer)
    {
        m_pDownDrawer->SetLayer(layer);
    }
    if (m_pHoverDrawer)
    {
        m_pHoverDrawer->SetLayer(layer);
    }
    if (m_pDisabledDrawer)
    {
        m_pDisabledDrawer->SetLayer(layer);
    }

    return this;
}

/******************************************************************************
 * Button::SetCoord -- Set the coordinate of the button.                      *
 *                                                                            *
 *    This is also for the transition to modify the button's coord.           *
 *                                                                            *
 * INPUT:   coord -- Top left coord of the button.                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetCoord(const Coordinate& coord)
{
    if (coord != m_coord)
    {
        Translate(coord - m_coord);
    }

    return this;
}

/******************************************************************************
 * Button::SetAlpha -- Set the alpha of the widget.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   alpha -- The alpha to set.                                        *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetAlpha(int alpha)
{
    if (m_pUpDrawer)
    {
        m_pUpDrawer->SetAlpha(alpha);
    }
    if (m_pDownDrawer)
    {
        m_pDownDrawer->SetAlpha(alpha);
    }
    if (m_pHoverDrawer)
    {
        m_pHoverDrawer->SetAlpha(alpha);
    }
    if (m_pDisabledDrawer)
    {
        m_pDisabledDrawer->SetAlpha(alpha);
    }

    return this;
}

/******************************************************************************
 * Button::SetScale -- Set the scale of button.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   scale                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetScale(double scale)
{
    if (m_pUpDrawer)
    {
        m_pUpDrawer->SetScale(scale);
    }
    if (m_pDownDrawer)
    {
        m_pDownDrawer->SetScale(scale);
    }
    if (m_pHoverDrawer)
    {
        m_pHoverDrawer->SetScale(scale);
    }
    if (m_pDisabledDrawer)
    {
        m_pDisabledDrawer->SetScale(scale);
    }

    return this;
}

/******************************************************************************
 * Button::SetRotationAngle -- Set the rotatoin angle of button.              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   angle                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetRotationAngle(double angle)
{
    if (m_pUpDrawer)
    {
        m_pUpDrawer->SetRotationAngle(angle);
    }
    if (m_pDownDrawer)
    {
        m_pDownDrawer->SetRotationAngle(angle);
    }
    if (m_pHoverDrawer)
    {
        m_pHoverDrawer->SetRotationAngle(angle);
    }
    if (m_pDisabledDrawer)
    {
        m_pDisabledDrawer->SetRotationAngle(angle);
    }

    return this;
}

/******************************************************************************
 * Button::Translate -- Translate the button.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   coord -- The alpha to set.                                        *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button* Button::Translate(const Coordinate& offset)
{
    Drawer* drawer;

    m_coord += offset;

    drawer = m_pUpDrawer;
    while (drawer)
    {
        dynamic_cast<Cell*>(drawer)->Translate(offset);
        drawer = drawer->GetSubDrawer();
    }
    drawer = m_pDownDrawer;
    while (drawer)
    {
        dynamic_cast<Cell*>(drawer)->Translate(offset);
        drawer = drawer->GetSubDrawer();
    }
    drawer = m_pHoverDrawer;
    while (drawer)
    {
        dynamic_cast<Cell*>(drawer)->Translate(offset);
        drawer = drawer->GetSubDrawer();
    }
    drawer = m_pDisabledDrawer;
    while (drawer)
    {
        dynamic_cast<Cell*>(drawer)->Translate(offset);
        drawer = drawer->GetSubDrawer();
    }

    if (m_pTrigger)
    {
        dynamic_cast<Cell*>(m_pTrigger)->Translate(offset);
    }

    return this;
}

/******************************************************************************
 * Button::SetUpDrawer -- Set the drawer when button is up.                   *
 *                                                                            *
 *    Drawers should only be owned to one class. So the previous one must be  *
 *    deleted.                                                                *
 *                                                                            *
 * INPUT:   drawer                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  Remember to delete the previous one.                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetUpDrawer(Drawer* drawer)
{
    _DELETE(m_pUpDrawer);
    m_pUpDrawer = drawer;

    return this;
}

/******************************************************************************
 * Button::SetDownDrawer -- Set the style when button is down.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   drawer                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetDownDrawer(Drawer* drawer)
{
    _DELETE(m_pDownDrawer);
    m_pDownDrawer = drawer;

    return this;
}

/******************************************************************************
 * Button::SetHoverDrawer -- Set the drawer when button is hovered.           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   drawer                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetHoverDrawer(Drawer* drawer)
{
    _DELETE(m_pHoverDrawer);
    m_pHoverDrawer = drawer;

    return this;
}

/******************************************************************************
 * Button::SetDisabledDrawer -- Set the drawer when button is disabled.       *
 *                                                                            *
 *    If this drawer isn't assigned, button will not be displayed when it is  *
 *    disabled.                                                               *
 *                                                                            *
 * INPUT:   drawer                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetDisabledDrawer(Drawer* drawer)
{
    _DELETE(m_pDisabledDrawer);
    m_pDisabledDrawer = drawer;

    return this;
}

/******************************************************************************
 * Button::SetTrigger -- Set the trigger of the button.                       *
 *                                                                            *
 *    If no trigger is assigned, the button will have no action.              *
 *                                                                            *
 * INPUT:   trigger                                                           *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetTrigger(Trigger* trigger)
{
    _DELETE(m_pTrigger);
    m_pTrigger = trigger;

    return this;
}

/******************************************************************************
 * Button::SetHoverSound -- Set the buzzer of the button when hover.          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   buzzer                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetHoverSound(Buzzer* buzzer)
{
    _DELETE(m_pHoverSound);
    m_pHoverSound = buzzer;

    return this;
}

/******************************************************************************
 * Button::SetClickSound -- Set the buzzer of the button.                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   buzzer                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/16 Tony : Created.                                               *
 *============================================================================*/
Button* Button::SetClickSound(Buzzer* buzzer)
{
    _DELETE(m_pClickSound);
    m_pClickSound = buzzer;

    return this;
}

/******************************************************************************
 * Button::_ProcessInput -- Process event.                                    *
 *                                                                            *
 *    This should only be called in Update.                                   *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/21 Tony : Created.                                               *
 *   2022/05/30 Tony : Keydown event doesn't need m_isPressed to be true.     *
 *============================================================================*/
void Button::_ProcessInput(Event& evnt)
{
    if (!m_pTrigger) // nothing will happen... :(
    {
        return;
    }

    switch (m_pTrigger->Detect(evnt))
    {
        // outside
    case TriggerValue::TV_MOVE:
        m_isHover = false;
        break;
    case TriggerValue::TV_HOLD:
        m_isPressed = false;
        break;
    case TriggerValue::TV_RELEASE:
        m_isPressed = false;
        break;
        // inside
    case TriggerValue::TV_HOVER:
        if (!m_isHover)
        {
            if (!m_isPressed && !evnt.Instant(MK_LBUTTON) && m_pHoverSound)
            {
                m_pHoverSound->Play();
            }
            m_isHover = true;
        }
        break;
    case TriggerValue::TV_PRESS:
        m_isPressed = true;
        if (m_pClickSound)
        {
            m_pClickSound->Play();
        }
        break;
    case TriggerValue::TV_KEYDOWN:
        if (m_pOnClick)
        {
            m_pOnClick();
        }
        break;
    case TriggerValue::TV_CLICK:
        if (m_isPressed && m_pOnClick)
        {
            m_pOnClick();
        }
        m_isPressed = false;
        break;
    default:
        break;
    }
}

/******************************************************************************
 * Slider::Slider -- Constructor of the object.                               *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Slider::Slider()
    : VisualWidget(WidgetType::SLIDER), m_pBlock(nullptr), m_pBarDrawer(nullptr), m_pBarTrigger(nullptr), m_barWidth(0),
      m_barHeight(0), m_value(0.0)
{
}

/******************************************************************************
 * Slider::~Slider -- Deconstructor of the object.                            *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Slider::~Slider()
{
    _DELETE(m_pBlock);
    _DELETE(m_pBarDrawer);
    _DELETE(m_pBarTrigger);
}

/******************************************************************************
 * Slider::Update -- Update slider.                                           *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void Slider::Update(Event* evnt)
{
    if (!m_isActivated)
    {
        return;
    }

    if (m_pBlock)
    {
        m_pBlock->Update(evnt);
    }
    if (m_pBarDrawer)
    {
        m_pBarDrawer->Update();
    }
    if (m_pBarTrigger)
    {
        m_pBarTrigger->Update();
    }

    _UpdateTransition();

    if (evnt)
    {
        _ProcessInput(*evnt);

        _AdjustBarCoord();
    }
}

/******************************************************************************
 * Slider::Draw -- Draw the slider.                                           *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void Slider::Draw()
{
    if (!m_isActivated)
    {
        return;
    }

    if (m_pBlock)
    {
        m_pBlock->Draw();
    }
    if (m_pBarDrawer)
    {
        m_pBarDrawer->Draw();
    }
}

/******************************************************************************
 * Slider::Load -- Load the slider.                                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
bool Slider::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;
    XMLElement* tag;

    _CHECK_TAG("Slider");
    _RETURN_IF_ERROR();

    VisualWidget::Load(node);

    _PARSE("value", m_value, name, 0.0);

    tag = node->FirstChildElement();
    while (tag)
    {
        attr = tag->Name();
        if (_STR_SAME(attr, "Block"))
        {
            SetBlock(dynamic_cast<Button*>(StandardWidgetKit().LoadWidget(tag->FirstChildElement())));
        }
        else if (_STR_SAME(attr, "Bar"))
        {
            SetBarDrawer(LoadDrawer(tag->FirstChildElement()));
        }
        else if (_STR_SAME(attr, "BarTrigger"))
        {
            SetBarTrigger(LoadTrigger(tag->FirstChildElement()));
        }

        tag = tag->NextSiblingElement();
    }

    SetLayer(m_layer);

    Coordinate offset = m_coord;
    m_coord = COORD_ZERO;
    Translate(offset);

    VisualWidget::LoadAttribute(node);

    _RETURN_STATE();
}

/******************************************************************************
 * Slider::SetLayer -- Set the layer of the slider.                           *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Slider* Slider::SetLayer(int layer)
{
    if (m_pBlock)
    {
        m_pBlock->SetLayer(layer + 1);
    }
    if (m_pBarDrawer)
    {
        m_pBarDrawer->SetLayer(layer);
    }

    return this;
}

/******************************************************************************
 * Slider::SetCoord -- Set the coordinate of the slider.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   coord                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Slider* Slider::SetCoord(const Coordinate& coord)
{
    if (coord != m_coord)
    {
        Translate(coord - m_coord);
    }

    return this;
}

/******************************************************************************
 * Slider::SetAlpha -- Set the alpha value of the slider.                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   alpha                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Slider* Slider::SetAlpha(int alpha)
{
    if (m_pBlock)
    {
        m_pBlock->SetAlpha(alpha);
    }

    if (m_pBarDrawer)
    {
        m_pBarDrawer->SetAlpha(alpha);
    }

    return this;
}

/******************************************************************************
 * Slider::SetScale -- Set the scale of the slider.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   scale                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
Slider* Slider::SetScale(double scale)
{
    if (m_pBlock)
    {
        m_pBlock->SetScale(scale);
    }

    if (m_pBarDrawer)
    {
        m_pBarDrawer->SetScale(scale);
    }

    return this;
}

/******************************************************************************
 * Slider::SetRotationAngle -- Set the rotatoin angle of slider.              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   angle                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
Slider* Slider::SetRotationAngle(double angle)
{
    if (m_pBlock)
    {
        m_pBlock->SetRotationAngle(angle);
    }

    if (m_pBarDrawer)
    {
        m_pBarDrawer->SetRotationAngle(angle);
    }

    return this;
}

/******************************************************************************
 * Slider::Translate -- Translate the slider.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   offset                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Slider* Slider::Translate(const Coordinate& offset)
{
    Drawer* drawer;

    m_coord += offset;

    if (m_pBlock)
    {
        m_pBlock->Translate(offset);
    }
    drawer = m_pBarDrawer;
    while (drawer)
    {
        dynamic_cast<Cell*>(drawer)->Translate(offset);
        drawer = drawer->GetSubDrawer();
    }
    if (m_pBarTrigger)
    {
        dynamic_cast<Cell*>(m_pBarTrigger)->Translate(offset);
    }

    return this;
}

/******************************************************************************
 * Slider::SetBlock -- Set the block of the slider.                           *
 *                                                                            *
 *    The block of the slider is a button. Hmm... interesting.                *
 *                                                                            *
 * INPUT:   block -- This button must be newed.                               *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Slider* Slider::SetBlock(Button* block)
{
    _DELETE(m_pBlock);
    m_pBlock = block;

    return this;
}

/******************************************************************************
 * Slider::SetBarDrawer -- Set the bar drawer of the slider.                  *
 *                                                                            *
 *    This drawer must derive fron rect or roundrect cell.                    *
 *                                                                            *
 * INPUT:   drwer                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Slider* Slider::SetBarDrawer(Drawer* drawer)
{
    _DELETE(m_pBarDrawer);
    m_pBarDrawer = drawer;

    if (m_pBarDrawer)
    {
        m_barWidth = dynamic_cast<Cell*>(m_pBarDrawer)->GetWidth();
        m_barHeight = dynamic_cast<Cell*>(m_pBarDrawer)->GetHeight();
    }

    return this;
}

/******************************************************************************
 * Slider::SetBarTrigger -- Set the trigger of the bar.                       *
 *                                                                            *
 *    This trigger must derive from rect or roundrect cell.                   *
 *                                                                            *
 * INPUT:   trigger                                                           *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
Slider* Slider::SetBarTrigger(Trigger* trigger)
{
    _DELETE(m_pBarTrigger);
    m_pBarTrigger = trigger;

    return this;
}

/******************************************************************************
 * Slider::SetValue -- Set the value of slider.                               *
 *                                                                            *
 *    This will call the callback at the same time.                           *
 *                                                                            *
 * INPUT:   value                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/30 Tony : Created.                                               *
 *============================================================================*/
void Slider::SetValue(double value)
{
    value = max(value, 0.0);
    value = min(value, 1.0);

    m_value = value;

    if (m_pOnChange)
    {
        m_pOnChange(m_value);
    }

    _AdjustBarCoord();
}

/******************************************************************************
 * Slider::_AdjustBarCoord -- Adjust bar's position.                          *
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
 *   2022/12/13 Tony : Created.                                               *
 *============================================================================*/
void Slider::_AdjustBarCoord()
{
    if (m_pBlock)
    {
        m_pBlock->SetCoord({ m_coord.x + static_cast<int>(m_barWidth * m_value), m_coord.y + (m_barHeight >> 1) });
    }
}

/******************************************************************************
 * Slider::_ProcessInput -- Process input of slider.                          *
 *                                                                            *
 *    If m_pBlock is not assigned, a progress bar can be placed on it for a   *
 *    different style. :)                                                     *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void Slider::_ProcessInput(Event& evnt)
{
    // If both m_pBlock and trigger are missing, nothing will happen.
    if (!m_pBlock && !m_pBarTrigger)
    {
        return;
    }

    Coordinate mouse = evnt.Mouse();

    if (m_pBlock)
    {
        // Here, button already processed input in Update().
        if (m_pBlock->IsPressed())
        {
            SetValue(static_cast<double>(mouse.x - m_coord.x) / static_cast<double>(m_barWidth));
        }
    }

    if (!m_pBarTrigger)
    {
        return;
    }

    // only respond to press
    switch (m_pBarTrigger->Detect(evnt))
    {
    case TriggerValue::TV_PRESS:
        SetValue(static_cast<double>(mouse.x - m_coord.x) / static_cast<double>(m_barWidth));
        break;
    default:
        break;
    }
}

/******************************************************************************
 * ProgressBar::ProgressBar -- Constructor of the object.                     *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
ProgressBar::ProgressBar()
    : VisualWidget(WidgetType::PROGRESSBAR), m_pBarDrawer(nullptr), m_pStuffDrawer(nullptr), m_padding(1)
{
}

/******************************************************************************
 * ProgressBar::~ProgressBar -- Deconstructor of the objecet.                 *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
ProgressBar::~ProgressBar()
{
    _DELETE(m_pBarDrawer);
    _DELETE(m_pStuffDrawer);
}

/******************************************************************************
 * ProgressBar::Update -- Update progress bar.                                *
 *                                                                            *
 *    Progressbar doesn't have interaction with user. :D                      *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void ProgressBar::Update(Event* evnt)
{
    if (!m_isActivated)
    {
        return;
    }

    if (m_pBarDrawer)
    {
        m_pBarDrawer->Update();
    }
    if (m_pStuffDrawer)
    {
        dynamic_cast<Cell*>(m_pStuffDrawer)->SetWidth(static_cast<int>(m_barWidth * m_value));
        m_pStuffDrawer->Update();
    }

    _UpdateTransition();
}

/******************************************************************************
 * ProgressBar::Draw -- Draw progressbar.                                     *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void ProgressBar::Draw()
{
    if (!m_isActivated)
    {
        return;
    }

    if (m_pBarDrawer)
    {
        m_pBarDrawer->Draw();
    }
    if (m_pStuffDrawer)
    {
        m_pStuffDrawer->Draw();
    }
}

/******************************************************************************
 * ProgressBar::Load -- Load progressbar.                                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
bool ProgressBar::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;
    XMLElement* tag;

    _CHECK_TAG("Progress");

    VisualWidget::Load(node);
    _RETURN_IF_ERROR();

    _PARSE("value", m_value, name, 0.0);

    tag = node->FirstChildElement();
    while (tag)
    {
        attr = tag->Name();
        if (_STR_SAME(attr, "Bar"))
        {
            SetBarDrawer(LoadDrawer(tag->FirstChildElement()));
        }
        else if (_STR_SAME(attr, "Stuff"))
        {
            SetStuffDrawer(LoadDrawer(tag->FirstChildElement()));
        }

        tag = tag->NextSiblingElement();
    }

    SetLayer(m_layer);

    Coordinate offset = m_coord;
    m_coord = COORD_ZERO;
    Translate(offset);

    VisualWidget::LoadAttribute(node);

    _RETURN_STATE();
}

/******************************************************************************
 * ProgressBar::SetLayer -- Set the layer of progressbar.                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   layer                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
ProgressBar* ProgressBar::SetLayer(int layer)
{
    if (m_pBarDrawer)
    {
        m_pBarDrawer->SetLayer(layer + 1);
    }
    if (m_pStuffDrawer)
    {
        m_pStuffDrawer->SetLayer(layer);
    }

    return this;
}

/******************************************************************************
 * ProgressBar::SetCoord -- Set the coord of the progressbar.                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   coord                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
ProgressBar* ProgressBar::SetCoord(const Coordinate& coord)
{
    if (coord != m_coord)
    {
        Translate(coord - m_coord);
    }

    return this;
}

/******************************************************************************
 * ProgressBar::SetAlpha -- Set the alpha value of progressbar.               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   alpha                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
ProgressBar* ProgressBar::SetAlpha(int alpha)
{
    if (m_pBarDrawer)
    {
        m_pBarDrawer->SetAlpha(alpha);
    }
    if (m_pStuffDrawer)
    {
        m_pStuffDrawer->SetAlpha(alpha);
    }

    return this;
}

/******************************************************************************
 * ProgressBar::SetScale -- Set the scale of the progressbar.                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   scale                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
ProgressBar* ProgressBar::SetScale(double scale)
{
    if (m_pBarDrawer)
    {
        m_pBarDrawer->SetScale(scale);
    }
    if (m_pStuffDrawer)
    {
        m_pStuffDrawer->SetScale(scale);
    }

    return this;
}

/******************************************************************************
 * ProgressBar::SetRotationAngle -- Set the rotatoin angle of progressbar.    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   angle                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
ProgressBar* ProgressBar::SetRotationAngle(double angle)
{
    if (m_pBarDrawer)
    {
        m_pBarDrawer->SetRotationAngle(angle);
    }
    if (m_pStuffDrawer)
    {
        m_pStuffDrawer->SetRotationAngle(angle);
    }

    return this;
}

/******************************************************************************
 * ProgressBar::Translate -- Translate progressbar.                           *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   offset                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
ProgressBar* ProgressBar::Translate(const Coordinate& offset)
{
    Drawer* drawer;

    m_coord += offset;

    drawer = m_pBarDrawer;
    while (drawer)
    {
        dynamic_cast<Cell*>(drawer)->Translate(offset);
        drawer = drawer->GetSubDrawer();
    }
    drawer = m_pStuffDrawer;
    while (drawer)
    {
        dynamic_cast<Cell*>(drawer)->Translate(offset);
        drawer = drawer->GetSubDrawer();
    }

    return this;
}

/******************************************************************************
 * ProgressBar::SetBarDrawer -- Set bar drawer of progressbar.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   drawer                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
ProgressBar* ProgressBar::SetBarDrawer(Drawer* drawer)
{
    _DELETE(m_pBarDrawer);
    m_pBarDrawer = drawer;
    if (m_pBarDrawer)
    {
        m_barWidth = dynamic_cast<Cell*>(m_pBarDrawer)->GetWidth();
        m_barHeight = dynamic_cast<Cell*>(m_pBarDrawer)->GetHeight();
    }

    return this;
}

/******************************************************************************
 * ProgressBar::SetStuffDrawer -- Set stuff drawer of progressbar.            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   drawer                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
ProgressBar* ProgressBar::SetStuffDrawer(Drawer* drawer)
{
    _DELETE(m_pStuffDrawer);
    m_pStuffDrawer = drawer;

    return this;
}

/******************************************************************************
 * ProgressBar::SetPadding -- Set the padding of ProgressBar.                 *
 *                                                                            *
 *    The padding is actually the border width of the progressbar. :)         *
 *                                                                            *
 * INPUT:   padding                                                           *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
ProgressBar* ProgressBar::SetPadding(int padding)
{
    if (m_pBarDrawer)
    {
        RawDrawer* drawer = dynamic_cast<RawDrawer*>(m_pBarDrawer);
        if (drawer)
        {
            drawer->SetLineThickness(padding);
        }
    }

    return this;
}

/******************************************************************************
 * RadioBox::RadioBox -- Constructor of the object.                           *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioBox::RadioBox()
    : VisualWidget(WidgetType::RADIO), m_pBox(nullptr), m_pMarkDrawer(nullptr), m_parent(nullptr), m_isCancelable(true),
      m_isPressed(false), m_isChecked(false)
{
}

/******************************************************************************
 * RadioBox::~RadioBox -- Deconstructor of the object.                        *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioBox::~RadioBox()
{
    _DELETE(m_pBox);
    _DELETE(m_pMarkDrawer);
}

/******************************************************************************
 * RadioBox::Update -- Update radio box.                                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void RadioBox::Update(Event* evnt)
{
    if (!m_isActivated)
    {
        return;
    }

    if (m_pBox)
    {
        m_pBox->Update(evnt);
    }
    if (m_pMarkDrawer)
    {
        m_pMarkDrawer->Update();
    }

    _UpdateTransition();

    if (evnt)
    {
        _ProcessInput(*evnt);
    }
}

/******************************************************************************
 * RadioBox::Draw -- Draw radio box.                                          *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void RadioBox::Draw()
{
    if (!m_isActivated)
    {
        return;
    }

    if (m_pBox)
    {
        m_pBox->Draw();
    }
    if (m_pMarkDrawer && m_isChecked && !m_isPressed)
    {
        m_pMarkDrawer->Draw();
    }
}

/******************************************************************************
 * RadioBox::Load -- Load radio box.                                          *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
bool RadioBox::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;
    XMLElement* tag;
    bool value = false;

    _CHECK_TAG("Radio");
    _RETURN_IF_ERROR();

    VisualWidget::Load(node);
    _PARSE("cancelable", value, name, false);
    m_isCancelable = value;

    tag = node->FirstChildElement();
    while (tag)
    {
        attr = tag->Name();
        if (_STR_SAME(attr, "Box"))
        {
            SetBox(dynamic_cast<Button*>(StandardWidgetKit().LoadWidget(tag->FirstChildElement())));
        }
        else if (_STR_SAME(attr, "Mark"))
        {
            SetMarkDrawer(LoadDrawer(tag->FirstChildElement()));
        }

        tag = tag->NextSiblingElement();
    }

    SetLayer(m_layer);

    Coordinate offset = m_coord;
    m_coord = COORD_ZERO;
    Translate(offset);

    VisualWidget::LoadAttribute(node);

    _RETURN_STATE();
}

/******************************************************************************
 * RadioBox::SetLayer -- Set the layer of the radio box.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   layer                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioBox::SetLayer(int layer)
{
    if (m_pBox)
    {
        m_pBox->SetLayer(layer);
    }
    if (m_pMarkDrawer)
    {
        m_pMarkDrawer->SetLayer(layer + 1);
    }

    return this;
}

/******************************************************************************
 * RadioBox::SetCoord -- Set the coordinate of the radio box.                 *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioBox::SetCoord(const Coordinate& coord)
{
    if (coord != m_coord)
    {
        Translate(coord - m_coord);
    }

    return this;
}

/******************************************************************************
 * RadioBox::SetAlpha -- Set the alpha value of the radio box.                *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioBox::SetAlpha(int alpha)
{
    if (m_pBox)
    {
        m_pBox->SetAlpha(alpha);
    }
    if (m_pMarkDrawer)
    {
        m_pMarkDrawer->SetAlpha(alpha);
    }

    return this;
}

/******************************************************************************
 * RadioBox::SetScale -- Set the scale of the radio box.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   scale                                                             *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioBox::SetScale(double scale)
{
    if (m_pBox)
    {
        m_pBox->SetScale(scale);
    }
    if (m_pMarkDrawer)
    {
        m_pMarkDrawer->SetScale(scale);
    }

    return this;
}

/******************************************************************************
 * RadioBox::SetRotationAngle -- Set the rotatoin angle of radio box.         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   angle                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioBox::SetRotationAngle(double angle)
{
    if (m_pBox)
    {
        m_pBox->SetRotationAngle(angle);
    }
    if (m_pMarkDrawer)
    {
        m_pMarkDrawer->SetRotationAngle(angle);
    }

    return this;
}

/******************************************************************************
 * RadioBox::Translate -- Translate the radio box.                            *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioBox::Translate(const Coordinate& offset)
{
    Drawer* drawer;

    m_coord += offset;

    if (m_pBox)
    {
        m_pBox->Translate(offset);
    }
    if (m_pMarkDrawer)
    {
        drawer = m_pMarkDrawer;
        while (drawer)
        {
            dynamic_cast<Cell*>(drawer)->Translate(offset);
            drawer = drawer->GetSubDrawer();
        }
    }

    return this;
}

/******************************************************************************
 * RadioBox::SetBox -- Set the box of the radio.                              *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioBox::SetBox(Button* box)
{
    _DELETE(m_pBox);
    m_pBox = box;

    return this;
}

/******************************************************************************
 * RadioBox::SetMarkDrawer -- Set the mark if checked.                        *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioBox::SetMarkDrawer(Drawer* drawer)
{
    _DELETE(m_pMarkDrawer);
    m_pMarkDrawer = drawer;

    return this;
}

/******************************************************************************
 * RadioBox::SetState -- Set the state of the radio box.                      *
 *                                                                            *
 *    This will trigger the onclick event.                                    *
 *                                                                            *
 * INPUT:   isChecked -- Whether is checked or not.                           *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/30 Tony : Created.                                               *
 *============================================================================*/
void RadioBox::SetState(bool isChecked)
{
    m_isChecked = isChecked;
    if (m_pOnCheck)
    {
        m_pOnCheck(m_isChecked);
    }
}

/******************************************************************************
 * RadioBox::_ProcessInput -- Process input of check box.                     *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void RadioBox::_ProcessInput(Event& evnt)
{
    if (!m_pBox)
    {
        return;
    }

    if (m_pBox->IsPressed())
    {
        m_isPressed = true;
    }
    else if (m_isPressed)
    {
        if (m_pBox->IsHover())
        {
            if (m_isChecked && m_isCancelable)
            {
                SetState(false);
            }
            else if (!m_isChecked)
            {
                SetState(true);
                if (m_parent)
                {
                    m_parent->Check(this);
                }
            }
        }
        m_isPressed = false;
    }
}

/******************************************************************************
 * RadioList::RadioList -- Constructor of the object.                         *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioList::RadioList() : VisualWidget(WidgetType::RADIOLIST)
{
}

/******************************************************************************
 * RadioList::~RadioList -- Destructor of the objecet.                        *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioList::~RadioList()
{
    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        delete it->second;
    }
}

/******************************************************************************
 * RadioList::Update -- Update radio list.                                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void RadioList::Update(Event* evnt)
{
    if (!m_isActivated)
    {
        return;
    }

    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        it->second->Update(evnt);
    }
}

/******************************************************************************
 * RadioList::Draw -- Draw radio list.                                        *
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
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void RadioList::Draw()
{
    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        it->second->Draw();
    }
}

/******************************************************************************
 * RadioList::Load -- Load radio list.                                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
bool RadioList::Load(XMLElement* node)
{
    const char* name = node->Name();
    XMLElement* tag;
    RadioBox* radio;

    _CHECK_TAG("RadioList");
    _RETURN_IF_ERROR();

    VisualWidget::Load(node);

    StandardWidgetKit kit;
    tag = node->FirstChildElement();
    while (tag)
    {
        radio = dynamic_cast<RadioBox*>(kit.LoadWidget(tag));
        if (Logger::Error())
        {
            LOG_ERROR(CANNOT_LOAD, "radio");
            return false;
        }
        AddRadio(radio->Name(), radio);

        tag = tag->NextSiblingElement();
    }

    SetLayer(m_layer);

    /*
    ** 2022/06/01 Tony: RadioList... no need for this.
    ** Coordinate offset = m_coord - COORD_ZERO;
    ** m_coord = COORD_ZERO;
    ** Translate(offset);
    */

    _RETURN_STATE();
}

/******************************************************************************
 * RadioList::ResetTransition -- Reset transition.                            *
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
 *   2022/06/19 Tony : Created.                                               *
 *============================================================================*/
RadioList* RadioList::ResetTransition()
{
    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        it->second->ResetTransition();
    }

    return this;
}

/******************************************************************************
 * RadioList::SetLayer -- Set the layer of radio list.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   layer                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioList* RadioList::SetLayer(int layer)
{
    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        it->second->SetLayer(layer);
    }

    return this;
}

/******************************************************************************
 * RadioList::SetCoord -- Set the coordinate of the radio list.               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   coord                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioList* RadioList::SetCoord(const Coordinate& coord)
{
    if (coord != m_coord)
    {
        Translate(coord - m_coord);
    }

    return this;
}

/******************************************************************************
 * RadioList::SetAlpha -- Set the alpha value of radio list.                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   alpha                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioList* RadioList::SetAlpha(int alpha)
{
    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        it->second->SetAlpha(alpha);
    }

    return this;
}

/******************************************************************************
 * RadioList::SetScale -- Set the scale of radio list.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   scale                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
RadioList* RadioList::SetScale(double scale)
{
    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        it->second->SetScale(scale);
    }

    return this;
}

/******************************************************************************
 * RadioList::SetRotationAngle -- Set the rotatoin angle of radio list.      *
 *                                                                            *
 *    Perhaps... It is meaningless...                                         *
 *                                                                            *
 * INPUT:   angle                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
RadioList* RadioList::SetRotationAngle(double angle)
{
    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        it->second->SetRotationAngle(angle);
    }

    return this;
}

/******************************************************************************
 * RadioList::Translate -- Translate radio list.                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   offset                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioList* RadioList::Translate(const Coordinate& offset)
{
    m_coord += offset;

    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        it->second->Translate(offset);
    }

    return this;
}

/******************************************************************************
 * RadioList::AddRadio -- Add radio box to radio list.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name  -- The name of the new radio.                               *
 *          radio -- The new radio.                                           *
 *                                                                            *
 * OUTPUT:  Return the added radio.                                           *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioList::AddRadio(const std::string& name, RadioBox* radio)
{
    if (!radio)
    {
        return nullptr;
    }

    auto it = m_radios.find(name);
    if (it == m_radios.end())
    {
        radio->SetParentList(this);
        m_radios.emplace(name, radio);
    }
    else
    {
        LOG_ERROR(NAME_CONFLICT, "Radio", name.c_str());
        delete it->second;
        it->second = radio;
    }

    return radio;
}

/******************************************************************************
 * RadioList::GetRadio -- Get a radio in the radio list.                      *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   name -- The name of the required radio.                           *
 *                                                                            *
 * OUTPUT:  Return the radio. nullptr if doesn't exist.                       *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/30 Tony : Created.                                               *
 *============================================================================*/
RadioBox* RadioList::GetRadio(const std::string& name)
{
    auto it = m_radios.find(name);

    if (it == m_radios.end())
    {
        return nullptr;
    }

    return it->second;
}

/******************************************************************************
 * RadioList::Check -- Check default radio box.                               *
 *                                                                            *
 *    This will check the first radio box by default.                         *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void RadioList::Check()
{
    if (m_radios.size() == 0)
    {
        return;
    }

    m_radios.begin()->second->SetState(true);
    Check(m_radios.begin()->second);
}

/******************************************************************************
 * RadioList::Check -- Check a radio box in the list.                         *
 *                                                                            *
 *    This is called by radio box when checked. If one radio box checked, it  *
 *    it will un-checked others.                                              *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/22 Tony : Created.                                               *
 *============================================================================*/
void RadioList::Check(RadioBox* radio)
{
    for (auto it = m_radios.begin(); it != m_radios.end(); it++)
    {
        if (it->second != radio)
        {
            it->second->SetState(false);
        }
    }
}

/******************************************************************************
 * StaticWidget::StaticWidget -- Constructor of the object.                   *
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
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
StaticWidget::StaticWidget() : VisualWidget(WidgetType::STATIC), m_pDrawer(nullptr)
{
}

/******************************************************************************
 * StaticWidget::~StaticWidget -- Destructor of the object.                   *
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
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
StaticWidget::~StaticWidget()
{
    _DELETE(m_pDrawer);
}

/******************************************************************************
 * StaticWidget::Update -- Update static widget.                              *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
void StaticWidget::Update(Event* evnt)
{
    if (!m_isActivated)
    {
        return;
    }

    if (m_pDrawer)
    {
        m_pDrawer->Update();
    }

    _UpdateTransition();
}

/******************************************************************************
 * StaticWidget::Draw -- Draw static widget.                                  *
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
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
void StaticWidget::Draw()
{
    if (m_isActivated && m_pDrawer)
    {
        m_pDrawer->Draw();
    }
}

/******************************************************************************
 * StaticWidget::Load -- Load static widget.                                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
bool StaticWidget::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;
    XMLElement* tag;

    _CHECK_TAG("Static");
    _RETURN_IF_ERROR();

    VisualWidget::Load(node);

    tag = node->FirstChildElement();
    while (tag)
    {
        attr = tag->Name();
        if (_STR_SAME(attr, "Drawer"))
        {
            SetDrawer(LoadDrawer(tag->FirstChildElement()));
        }

        tag = tag->NextSiblingElement();
    }

    SetLayer(m_layer);

    Coordinate offset = m_coord;
    m_coord = COORD_ZERO;
    Translate(offset);

    VisualWidget::LoadAttribute(node);

    _RETURN_STATE();
}

/******************************************************************************
 * StaticWidget::SetLayer -- Set the layer of static widget.                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   layer                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
StaticWidget* StaticWidget::SetLayer(int layer)
{
    if (m_pDrawer)
    {
        m_pDrawer->SetLayer(layer);
    }

    return this;
}

/******************************************************************************
 * StaticWidget::SetCoord -- Set the coordinate of static widget.             *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   coord                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
StaticWidget* StaticWidget::SetCoord(const Coordinate& coord)
{
    if (coord != m_coord)
    {
        Translate(coord - m_coord);
    }

    return this;
}

/******************************************************************************
 * StaticWidget::SetAlpha -- Set the alpha value of static widget.            *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   alpha                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
StaticWidget* StaticWidget::SetAlpha(int alpha)
{
    if (m_pDrawer)
    {
        m_pDrawer->SetAlpha(alpha);
    }

    return this;
}

/******************************************************************************
 * StaticWidget::SetScale -- Set the scale of static widget.                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   scale                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/06 Tony : Created.                                               *
 *============================================================================*/
StaticWidget* StaticWidget::SetScale(double scale)
{
    if (m_pDrawer)
    {
        m_pDrawer->SetScale(scale);
    }

    return this;
}

/******************************************************************************
 * StaticWidget::SetRotationAngle -- Set the rotatoin angle of static widget. *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   angle                                                             *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/11/25 Tony : Created.                                               *
 *============================================================================*/
StaticWidget* StaticWidget::SetRotationAngle(double angle)
{
    if (m_pDrawer)
    {
        m_pDrawer->SetRotationAngle(angle);
    }

    return this;
}

/******************************************************************************
 * StaticWidget::Translate -- Translate static widget.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   offset                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
StaticWidget* StaticWidget::Translate(const Coordinate& offset)
{
    Drawer* drawer;

    m_coord += offset;

    drawer = m_pDrawer;
    while (drawer)
    {
        dynamic_cast<Cell*>(drawer)->Translate(offset);
        drawer = drawer->GetSubDrawer();
    }

    return this;
}

/******************************************************************************
 * StaticWidget::SetDrawer -- Set the drawer of static widget.                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   drawer                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
StaticWidget* StaticWidget::SetDrawer(Drawer* drawer)
{
    _DELETE(m_pDrawer);
    m_pDrawer = drawer;

    return this;
}

/******************************************************************************
 * KeyboardDetector::KeyboardDetector -- Constructor of the object.           *
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
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
KeyboardDetector::KeyboardDetector() : AbstractWidget(WidgetType::DETECTOR), m_pTrigger(nullptr), m_isTriggered(false)
{
}

/******************************************************************************
 * KeyboardDetector::~KeyboardDetector -- Destructor of the object.           *
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
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
KeyboardDetector::~KeyboardDetector()
{
    _DELETE(m_pTrigger);
}

/******************************************************************************
 * KeyboardDetector::Update --- Update keyboard detector.                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
void KeyboardDetector::Update(Event* evnt)
{
    if (!m_isActivated)
    {
        return;
    }

    if (m_pTrigger)
    {
        m_pTrigger->Update();
    }

    if (evnt)
    {
        _ProcessInput(*evnt);
    }
}

/******************************************************************************
 * KeyboardDetector::Load -- Load keyboard detector.                          *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
bool KeyboardDetector::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;
    XMLElement* tag;

    _CHECK_TAG("Detector");
    _RETURN_IF_ERROR();

    AbstractWidget::Load(node);

    tag = node->FirstChildElement();
    while (tag)
    {
        attr = tag->Name();
        if (_STR_SAME(attr, "Trigger"))
        {
            SetTrigger(dynamic_cast<KeyboardTrigger*>(LoadTrigger(tag)));
        }
        else
        {
            LOG_WARNING(UNKNOWN_TAG, attr);
        }

        tag = tag->NextSiblingElement();
    }

    _RETURN_STATE();
}

/******************************************************************************
 * KeyboardDetector::SetTrigger -- Set trigger of keyboard detector.          *
 *                                                                            *
 *    This must be keyboard trigger.                                          *
 *                                                                            *
 * INPUT:   trigger                                                           *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
KeyboardDetector* KeyboardDetector::SetTrigger(KeyboardTrigger* trigger)
{
    _DELETE(m_pTrigger);
    m_pTrigger = trigger;

    return this;
}

/******************************************************************************
 * KeyboardDetector::_ProcessInput -- Handle keyboard event.                  *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/24 Tony : Created.                                               *
 *============================================================================*/
void KeyboardDetector::_ProcessInput(Event& evnt)
{
    if (!m_pTrigger) // nothing will happen... :(
    {
        return;
    }

    if (m_pTrigger->Detect(evnt) == TriggerValue::TV_KEYDOWN)
    {
        if (!m_isTriggered)
        {
            if (m_pOnTriggered)
            {
                m_pOnTriggered();
            }
            m_isTriggered = true;
        }
    }
    else
    {
        m_isTriggered = false;
    }
}

/******************************************************************************
 * AudioPlayer::AudioPlayer -- Constructor of the object.                     *
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
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
AudioPlayer::AudioPlayer() : AbstractWidget(WidgetType::AUDIO), m_playList(nullptr)
{
}

/******************************************************************************
 * AudioPlayer::~AudioPlayer -- Destructor of the object.                     *
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
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
AudioPlayer::~AudioPlayer()
{
}

/******************************************************************************
 * AudioPlayer::Update -- Update play list.                                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   evnt                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
void AudioPlayer::Update(Event* evnt)
{
}

/******************************************************************************
 * AudioPlayer::Load -- Load resource.                                        *
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
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
bool AudioPlayer::Load(XMLElement* node)
{
    /*
    **	<Audio src=""></Audio>
    */
    const char* name = node->Name();
    const char* src;

    _CHECK_TAG("Audio");
    _RETURN_IF_ERROR();

    AbstractWidget::Load(node);

    src = node->Attribute("src");
    if (!src)
    {
        LOG_ERROR(MISSING_ATTRIBUTE, "src");
        return false;
    }

    AudioResource* res = LoadResource<AudioResource>(src);
    if (!res)
    {
        return false;
    }
    m_playList = dynamic_cast<PlayList*>(res->GetResource());
    if (!m_playList)
    {
        LOG_ERROR(RESOURCE_MISMATCH, src);
        return false;
    }

    return true;
}

/******************************************************************************
 * AudioPlayer::Play -- Start to play.                                        *
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
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
void AudioPlayer::Play()
{
    if (m_playList)
    {
        Speaker::GetInstance()->AddPlayList(m_playList);
    }
}

/******************************************************************************
 * AudioPlayer::Pause -- Pause or un-pause.                                   *
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
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
void AudioPlayer::Pause(bool isPaused)
{
    if (m_playList)
    {
        Speaker::GetInstance()->Pause(isPaused);
    }
}

/******************************************************************************
 * AudioPlayer::Stop -- Stop playing.                                         *
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
 *   2022/07/17 Tony : Created.                                               *
 *   2022/12/17 Tony : Extra check for consistence.                           *
 *============================================================================*/
void AudioPlayer::Stop()
{
    Speaker* speaker = Speaker::GetInstance();
    if (m_playList && (speaker->GetCurrentPlayList() == m_playList))
    {
        speaker->RemovePlayList();
    }
}

/******************************************************************************
 * StandardWidgetKit::LoadWidget -- Load standard widgets.                    *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   node                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/17 Tony : Created.                                               *
 *============================================================================*/
AbstractWidget* StandardWidgetKit::LoadWidget(XMLElement* node)
{
    if (!node)
    {
        return nullptr;
    }

    const char* name = node->Name();
    AbstractWidget* rv = nullptr;

    if (_STR_SAME(name, "Button"))
    {
        rv = new Button();
    }
    else if (_STR_SAME(name, "Slider"))
    {
        rv = new Slider();
    }
    else if (_STR_SAME(name, "Progress"))
    {
        rv = new ProgressBar();
    }
    else if (_STR_SAME(name, "Radio"))
    {
        rv = new RadioBox();
    }
    else if (_STR_SAME(name, "RadioList"))
    {
        rv = new RadioList();
    }
    else if (_STR_SAME(name, "Static"))
    {
        rv = new StaticWidget();
    }
    else if (_STR_SAME(name, "Detector"))
    {
        rv = new KeyboardDetector();
    }
    else if (_STR_SAME(name, "Audio"))
    {
        rv = new AudioPlayer();
    }

    if (rv)
    {
        rv->Load(node);
    }
    else
    {
        LOG_ERROR(UNKNOWN_TAG, name);
    }

    return rv;
}
