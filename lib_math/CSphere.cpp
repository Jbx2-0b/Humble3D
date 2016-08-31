#include "CSphere.h"
#include "CBox3D.h"

//-----------------------------------------------------------------------------------------
CSphere::CSphere()
    : m_dRadius(0.0f)
{

}

//-----------------------------------------------------------------------------------------
CSphere::CSphere(const QVector3D& center, real dRadius)
    : m_Center(center)
    , m_dRadius(dRadius)
{
}

//-----------------------------------------------------------------------------------------
CSphere::CSphere(const CBox3D& box, EnumBoundingMode eBoundingMode /*= eOuterSphere*/)
{
    m_Center = box.getCenter();

    if (eBoundingMode == eInnerSphere)
    {
        QVector3D halfSize = box.getHalfSize();
        m_dRadius = qMax(halfSize.x(), qMax(halfSize.y(), halfSize.z()));
    }
    else // eOuterSphere
    {
        m_dRadius = m_Center.distanceToPoint(box.getMaximum());
    }
}

//-----------------------------------------------------------------------------------------
bool CSphere::intersects(const CBox3D& box) const
{
    float dist_squared = m_dRadius * m_dRadius;
    QVector3D min = box.getMinimum();
    QVector3D max = box.getMaximum();

    if (m_Center.x() < min.x()) dist_squared -= Math::squared(m_Center.x() - min.x());
    else if (m_Center.x() > max.x()) dist_squared -= Math::squared(m_Center.x() - max.x());
    if (m_Center.y() < min.y()) dist_squared -= Math::squared(m_Center.y() - min.y());
    else if (m_Center.y() > max.y()) dist_squared -= Math::squared(m_Center.y() - max.y());
    if (m_Center.z() < min.z()) dist_squared -= Math::squared(m_Center.z() - min.z());
    else if (m_Center.z() > max.z()) dist_squared -= Math::squared(m_Center.z() - max.z());
    return dist_squared > 0;
}
