#include "CCylinder.h"

CCylinder::CCylinder()
    : m_dRadius(1.)
    , m_dHeight(1.)
{}

CCylinder::CCylinder(real dRadius, real dHeight)
    : m_dRadius(dRadius)
    , m_dHeight(dHeight)
{}

real CCylinder::getRadius() const
{
    return m_dRadius;
}

real CCylinder::getHeight() const
{
    return m_dHeight;
}
