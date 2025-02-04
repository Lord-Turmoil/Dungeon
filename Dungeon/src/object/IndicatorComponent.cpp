/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : IndicatorComponent.cpp                    *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : August 16, 2022                           *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   To indicate target direction.                                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Math.h"

#include "../../inc/object/Hero.h"
#include "../../inc/object/IndicatorComponent.h"

/******************************************************************************
 * IndicatorComponent::~IndicatorComponent -- Destructor of the object.       *
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
 *   2022/08/16 Tony : Created.                                               *
 *============================================================================*/
IndicatorComponent::~IndicatorComponent()
{
    _DELETE(m_images[0]);
    _DELETE(m_images[1]);
}

/******************************************************************************
 * IndicatorComponent::Clone -- Clone indicator component.                    *
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
 *   2022/08/16 Tony : Created.                                               *
 *============================================================================*/
IndicatorComponent* IndicatorComponent::Clone() const
{
    auto clone = new IndicatorComponent(m_updateOrder);
    clone->_MakePrototype(false);

    AbstractComponent::Clone(clone);

    clone->m_offset = m_offset;
    clone->m_images[0] = m_images[0]->Clone();
    clone->m_images[1] = m_images[1]->Clone();

    return clone;
}

/******************************************************************************
 * IndicatorComponent::Load -- Load indicator component.                      *
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
 *   2022/08/16 Tony : Created.                                               *
 *============================================================================*/
bool IndicatorComponent::Load(XMLElement* node)
{
    const char* name = node->Name();
    const char* attr;

    _CHECK_TAG(StaticName());
    _RETURN_IF_ERROR();

    AbstractComponent::Load(node);

    _PARSE_PRIVATE("offset", m_offset, name, ParseCoord);

    XMLElement* li = node->FirstChildElement();
    if (!li)
    {
        LOG_ERROR(MISSING_CHILD_ELEMENT, name);
        return false;
    }
    m_images[0] = new ImageObject();
    m_images[0]->Load(li);

    li = li->NextSiblingElement();
    if (!li)
    {
        LOG_ERROR(MISSING_CHILD_ELEMENT, name);
        return false;
    }
    m_images[1] = new ImageObject();
    m_images[1]->Load(li);

    _RETURN_STATE();
}

/******************************************************************************
 * IndicatorComponent::Update -- Update indicator component.                  *
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
 *   2022/08/16 Tony : Created.                                               *
 *============================================================================*/
void IndicatorComponent::Update(Event* evnt)
{
    Symbol* pSymbol = m_pGameObject->GetSymbol();
    m_coord = m_pGameObject->GetCoord();

    if (m_type == IND_NONE)
    {
        pSymbol->SetSupSymbol(nullptr);
    }
    else
    {
        m_symbol.SetCoord(m_coord + m_offset);
        m_symbol.SetImage(m_images[m_type]->GetImage());
        m_symbol.GetAttribute()->rotationAngle = GetRotationRadian(m_target - m_coord);
        pSymbol->SetSupSymbol(&m_symbol);
    }
}

/******************************************************************************
 * IndicatorComponent::SetTarget -- Set target of indicator.                  *
 *                                                                            *
 *    Indicator will target at enemy first.                                   *
 *                                                                            *
 * INPUT:   target                                                            *
 *          type   -- Can only be IND_CRATE or IND_ENEMY.                     *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/08/16 Tony : Created.                                               *
 *============================================================================*/
void IndicatorComponent::SetTarget(const Coordinate& target, IndicatorType type)
{
    if (m_type == IND_NONE)
    {
        m_type = type;
        m_target = target;
        m_dist = Distance(m_coord, m_target);
    }
    else if (m_type == IND_CRATE)
    {
        if (type == IND_ENEMY)
        {
            m_type = type;
            m_target = target;
            m_dist = Distance(m_coord, m_target);
        }
        else // type == IND_CRATE
        {
            double dist = Distance(m_coord, target);
            if (dist < m_dist)
            {
                m_dist = dist;
                m_target = target;
            }
        }
    }
    else // m_type == IND_ENEMY
    {
        if (type == IND_ENEMY)
        {
            double dist = Distance(m_coord, target);
            if (dist < m_dist)
            {
                m_dist = dist;
                m_target = target;
            }
        }
    }
}
