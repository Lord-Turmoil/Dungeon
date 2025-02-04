/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : AbstractWidget.h                          *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 17, 2022                             *
 *                                                                            *
 *                    Last Update : December 22, 2022                         *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   A refactoring of previous widget module.                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _ABSTRACT_WIDGET_H_
#define _ABSTRACT_WIDGET_H_

#include <string>
#include <vector>

#include "../common/Common.h"
#include "../utility/tinyxml.h"

class Event;
class WidgetManager;

class AbstractWidget
{
public:
    AbstractWidget(WidgetType type) : m_type(type), m_manager(nullptr), m_isActivated(true)
    {
    }

    virtual ~AbstractWidget()
    {
    }

    WidgetType Type() const
    {
        return m_type;
    }

    void SetName(const std::string& name)
    {
        m_name = name;
    }

    const std::string Name() const
    {
        return m_name;
    }

    virtual bool IsVisual() const
    {
        return false;
    }

    AbstractWidget* SetManager(WidgetManager* manager)
    {
        m_manager = manager;
        return this;
    }

    WidgetManager* GetManager() const
    {
        return m_manager;
    }

    AbstractWidget* Activate(bool isActivated)
    {
        m_isActivated = isActivated;
        return this;
    }

    bool IsActivated() const
    {
        return m_isActivated;
    }

public:
    virtual void Update(Event* evnt = nullptr)
    {
    }

    virtual void Draw() {};
    virtual bool Load(XMLElement* node);

protected:
    virtual void _ProcessInput(Event& evnt)
    {
    }

protected:
    WidgetType m_type;
    std::string m_name;
    WidgetManager* m_manager;

    /*
    ** Whether a widget is activated for use or not.
    ** if is not activated, then user simply can't
    ** see it, let alone have interaction with it.
    */
    bool m_isActivated;
};

/********************************************************************
** VisualWidget are widgets that can be seen and have interactions.
** 2022/11/25 TS:
** Added support for rotation transition.
*/
class Transition;

class VisualWidget : public AbstractWidget
{
public:
    VisualWidget(WidgetType type) : AbstractWidget(type), m_layer(LAYER_UI)
    {
    }

    ~VisualWidget() override;

    bool IsVisual() const override
    {
        return true;
    }

    /*
    ** This will automatically set draw cells' and triggers'
    ** position. This is also used by transition to move
    ** the widget.
    */
    virtual VisualWidget* SetCoord(const Coordinate& coord) = 0;

    Coordinate GetCoord() const
    {
        return m_coord;
    }

    virtual VisualWidget* Translate(const Coordinate& offset) = 0;
    virtual VisualWidget* SetAlpha(int alpha) = 0;
    virtual VisualWidget* SetScale(double scale) = 0;
    virtual VisualWidget* SetRotationAngle(double angle) = 0;

    virtual VisualWidget* SetLayer(int layer) = 0;

    int GetLayer() const
    {
        return m_layer;
    }

    /*
    ** These are for transition component. If a widget has
    ** transition component, it may change through time.
    ** However, in this project, there's only basic translate
    ** transition and alpha transition, so it won't be too
    ** complicated, I guess? ;)
    */
    Transition* AddTransition(Transition* transition);

    /*
    ** This will reset all transitions, to apply them
    ** all over again.
    */
    virtual VisualWidget* ResetTransition();
    virtual VisualWidget* TerminateTransition();
    virtual VisualWidget* StopTransition();

    /*
     ** Here, evnt can be nullptr, which means no event to handle.
     */
    void Update(Event* evnt = nullptr) override
    {
    }

    void Draw() override
    {
    }

    bool Load(XMLElement* node) override;
    virtual bool LoadAttribute(XMLElement* node);

protected:
    void _ProcessInput(Event& evnt) override
    {
    }

    void _AdjustProperty(XMLElement* node);

    /*
    ** This will update all transitions, and if a transition
    ** is over, then it will still keep it in case of Reset.
    */
    void _UpdateTransition();

    /*
    ** The coordinate of the widget. However, the widget itself
    ** doesn't have a actual form, so this coordinate act as the
    ** reference point for all it's components. (top-left)
    */
    Coordinate m_coord;

    /*
    ** Widget's layer, to display in multiple layer and may
    ** shield each other.
    */
    int m_layer;

    /*
    ** A widget may have many transitions. However, in this
    ** project, it won't show its full potential.
    */
    std::vector<Transition*> m_transitionList;
};

#endif
