#include "CBox3D.h"
#include "CMath.h"

const CBox3D CBox3D::NullBox(QVector3D(0., 0., 0.), QVector3D(0., 0., 0.));
const CBox3D CBox3D::NDCBox(QVector3D(-1., -1., -1.), QVector3D(1., 1., 1.));

//-----------------------------------------------------------------------------------------
void CBox3D::setExtents(const QVector3D& corner1, const QVector3D& corner2)
{
    m_vMinimumCorner = QVector3D(
                qMin(corner1.x(), corner2.x()),
                qMin(corner1.y(), corner2.y()),
                qMin(corner1.z(), corner2.z()));
    m_vMaximumCorner = QVector3D(
                qMax(corner1.x(), corner2.x()),
                qMax(corner1.y(), corner2.y()),
                qMax(corner1.z(), corner2.z()));

    m_bNeedUpdateCorners = true;
}

//-----------------------------------------------------------------------------------------
const QList<QVector3D>& CBox3D::getCorners() const
{
    if (m_bNeedUpdateCorners)
    {
        m_Corners.clear();
        m_Corners << m_vMinimumCorner;
        m_Corners << m_vMaximumCorner;
        m_Corners << QVector3D(m_vMinimumCorner.x(), m_vMinimumCorner.y(), m_vMaximumCorner.z());
        m_Corners << QVector3D(m_vMinimumCorner.x(), m_vMaximumCorner.y(), m_vMaximumCorner.z());
        m_Corners << QVector3D(m_vMinimumCorner.x(), m_vMaximumCorner.y(), m_vMinimumCorner.z());
        m_Corners << QVector3D(m_vMaximumCorner.x(), m_vMinimumCorner.y(), m_vMinimumCorner.z());
        m_Corners << QVector3D(m_vMaximumCorner.x(), m_vMinimumCorner.y(), m_vMaximumCorner.z());
        m_Corners << QVector3D(m_vMaximumCorner.x(), m_vMaximumCorner.y(), m_vMinimumCorner.z());

        m_bNeedUpdateCorners = false;
    }

    return m_Corners;
}

//-----------------------------------------------------------------------------------------
QVector3D CBox3D::getCenter() const
{
    if (m_bNeedUpdateCenter)
    {
        m_Center = QVector3D((m_vMaximumCorner.x() - m_vMinimumCorner.x()) / 2. + m_vMinimumCorner.x(),
                             (m_vMaximumCorner.y() - m_vMinimumCorner.y()) / 2. + m_vMinimumCorner.y(),
                             (m_vMaximumCorner.z() - m_vMinimumCorner.z()) / 2. + m_vMinimumCorner.z());

        m_bNeedUpdateCenter = false;
    }

    return m_Center;
}

//-----------------------------------------------------------------------------------------
QVector3D CBox3D::getSize() const
{
    return QVector3D(qAbs(m_vMaximumCorner.x() - m_vMinimumCorner.x()),
                     qAbs(m_vMaximumCorner.y() - m_vMinimumCorner.y()),
                     qAbs(m_vMaximumCorner.z() - m_vMinimumCorner.z()));
}

//-----------------------------------------------------------------------------------------
QVector3D CBox3D::getHalfSize() const
{
    return QVector3D(qAbs(m_vMaximumCorner.x() - m_vMinimumCorner.x()) / 2.,
                     qAbs(m_vMaximumCorner.y() - m_vMinimumCorner.y()) / 2.,
                     qAbs(m_vMaximumCorner.z() - m_vMinimumCorner.z()) / 2.);
}

//-----------------------------------------------------------------------------------------
bool CBox3D::intersects(const CBox3D& bbox) const
{
    if (m_vMaximumCorner.x() < bbox.m_vMinimumCorner.x())
        return false;
    if (m_vMinimumCorner.x() > bbox.m_vMaximumCorner.x())
        return false;
    if (m_vMaximumCorner.y() < bbox.m_vMinimumCorner.y())
        return false;
    if (m_vMinimumCorner.y() > bbox.m_vMaximumCorner.y())
        return false;
    if (m_vMaximumCorner.z() < bbox.m_vMinimumCorner.z())
        return false;
    if (m_vMinimumCorner.z() > bbox.m_vMaximumCorner.z())
        return false;
    return true;
}

//-----------------------------------------------------------------------------------------
bool CBox3D::contains(const QVector3D& vector) const
{
    if (vector.x() >= m_vMinimumCorner.x() && vector.x() <= m_vMaximumCorner.x() &&
            vector.y() >= m_vMinimumCorner.y() && vector.y() <= m_vMaximumCorner.y() &&
            vector.z() >= m_vMinimumCorner.z() && vector.z() <= m_vMaximumCorner.z())
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------
bool CBox3D::contains(const CBox3D& bbox) const
{
    foreach (const QVector3D& corner, bbox.getCorners())
    {
        if (!contains(corner))
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------
void CBox3D::unite(const QVector3D& point)
{
    if (point.x() < m_vMinimumCorner.x())
        m_vMinimumCorner.setX(point.x());
    else if (point.x() > m_vMaximumCorner.x())
        m_vMaximumCorner.setX(point.x());

    if (point.y() < m_vMinimumCorner.y())
        m_vMinimumCorner.setY(point.y());
    else if (point.y() > m_vMaximumCorner.y())
        m_vMaximumCorner.setY(point.y());

    if (point.z() < m_vMinimumCorner.z())
        m_vMinimumCorner.setZ(point.z());
    else if (point.z() > m_vMaximumCorner.z())
        m_vMaximumCorner.setZ(point.z());

    m_bNeedUpdateCorners = true;
    m_bNeedUpdateCenter = true;
}

//-----------------------------------------------------------------------------------------
CBox3D CBox3D::united(const CBox3D& bbox) const
{
    if (isNull()) return bbox;
    if (bbox.isNull()) return *this;

    CBox3D unitedBbox;

    unitedBbox.getMinimum().setX(qMin(getMinimum().x(), bbox.getMinimum().x()));
    unitedBbox.getMinimum().setY(qMin(getMinimum().y(), bbox.getMinimum().y()));
    unitedBbox.getMinimum().setZ(qMin(getMinimum().z(), bbox.getMinimum().z()));

    unitedBbox.getMaximum().setX(qMax(this->getMaximum().x(), bbox.getMaximum().x()));
    unitedBbox.getMaximum().setY(qMax(this->getMaximum().y(), bbox.getMaximum().y()));
    unitedBbox.getMaximum().setZ(qMax(this->getMaximum().z(), bbox.getMaximum().z()));

    return unitedBbox;
}

//-----------------------------------------------------------------------------------------
void CBox3D::setNull()
{
    m_vMinimumCorner = m_vMaximumCorner = QVector3D(0., 0., 0.);
}

//-----------------------------------------------------------------------------------------
void CBox3D::transform(const QMatrix4x4& transformation)
{
    *this = transformed(transformation);

    m_bNeedUpdateCorners = true;
    m_bNeedUpdateCenter = true;
}

//-----------------------------------------------------------------------------------------
CBox3D CBox3D::transformed(const QMatrix4x4& transformation) const
{
    QList<QVector3D> corners = getCorners();

    for (int i = 0; i < corners.size(); ++i)
    {
        corners[i] = transformation * corners[i];
    }

    qreal minX, minY, minZ, maxX, maxY, maxZ;
    minX = maxX = corners[0].x();
    minY = maxY = corners[0].y();
    minZ = maxZ = corners[0].z();

    for (int i = 1; i < corners.size(); ++i)
    {
        minX = qMin(minX, (qreal)corners[i].x());
        minY = qMin(minY, (qreal)corners[i].y());
        minZ = qMin(minZ, (qreal)corners[i].z());
        maxX = qMax(maxX, (qreal)corners[i].x());
        maxY = qMax(maxY, (qreal)corners[i].y());
        maxZ = qMax(maxZ, (qreal)corners[i].z());
    }

    return CBox3D(QVector3D(minX, minY, minZ), QVector3D(maxX, maxY, maxZ));
}

//-----------------------------------------------------------------------------------------
bool CBox3D::isNull() const
{
    if (m_vMinimumCorner == m_vMaximumCorner)
        return true;

    return false;
}

//-----------------------------------------------------------------------------------------
bool CBox3D::sortByLessZValue(const CBox3D& bbox1, const CBox3D& bbox2)
{
    return bbox1.getCenter().z() > bbox2.getCenter().z();
}

//-----------------------------------------------------------------------------------------
bool CBox3D::intersects(const CRay& ray) const
{
    real dDistance;
    return intersection(ray, &dDistance);
}


//-----------------------------------------------------------------------------------------
bool CBox3D::intersection(const CRay& ray, real* dDistance) const
{
    *dDistance = 0.;
    if (isNull()) return false;

    real lowt = 0.0f;
    real t;
    bool hit = false;
    QVector3D hitpoint;
    const QVector3D& min = getMinimum();
    const QVector3D& max = getMaximum();
    const QVector3D& rayorig = ray.getOrigin();
    const QVector3D& raydir = ray.getDirection();

    // Check origin inside first
    if (rayorig.x() > min.x() &&
            rayorig.y() > min.y() &&
            rayorig.z() > min.z() &&
            rayorig.x() < max.x() &&
            rayorig.y() < max.y() &&
            rayorig.z() < max.z())
    {
        return true;
    }

    // Check each face in turn, only check closest 3
    // Min x()
    if (rayorig.x() <= min.x() && raydir.x() > 0)
    {
        t = (min.x() - rayorig.x()) / raydir.x();
        if (t >= 0)
        {
            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if (hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
                    hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
                    (!hit || t < lowt))
            {
                hit = true;
                lowt = t;
            }
        }
    }
    // Max x
    if (rayorig.x() >= max.x() && raydir.x() < 0)
    {
        t = (max.x() - rayorig.x()) / raydir.x();
        if (t >= 0)
        {
            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if (hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
                    hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
                    (!hit || t < lowt))
            {
                hit = true;
                lowt = t;
            }
        }
    }
    // Min y()
    if (rayorig.y() <= min.y() && raydir.y() > 0)
    {
        t = (min.y() - rayorig.y()) / raydir.y();
        if (t >= 0)
        {
            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
                    hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
                    (!hit || t < lowt))
            {
                hit = true;
                lowt = t;
            }
        }
    }
    // Max y()
    if (rayorig.y() >= max.y() && raydir.y() < 0)
    {
        t = (max.y() - rayorig.y()) / raydir.y();
        if (t >= 0)
        {
            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
                    hitpoint.z() >= min.z() && hitpoint.z() <= max.z() &&
                    (!hit || t < lowt))
            {
                hit = true;
                lowt = t;
            }
        }
    }
    // Min z()
    if (rayorig.z() <= min.z() && raydir.z() > 0)
    {
        t = (min.z() - rayorig.z()) / raydir.z();
        if (t >= 0)
        {
            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
                    hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
                    (!hit || t < lowt))
            {
                hit = true;
                lowt = t;
            }
        }
    }
    // Max z()
    if (rayorig.z() >= max.z() && raydir.z() < 0)
    {
        t = (max.z() - rayorig.z()) / raydir.z();
        if (t >= 0)
        {
            // Substitute t back into ray and check bounds and dist
            hitpoint = rayorig + raydir * t;
            if (hitpoint.x() >= min.x() && hitpoint.x() <= max.x() &&
                    hitpoint.y() >= min.y() && hitpoint.y() <= max.y() &&
                    (!hit || t < lowt))
            {
                hit = true;
                lowt = t;
            }
        }
    }

    *dDistance = lowt;
    return hit;
}

//-----------------------------------------------------------------------------------------
real CBox3D::distance(const QVector3D& v) const
{
    if (contains(v))
        return 0;
    else
    {
        QVector3D maxDist;

        if (v.x() < m_vMinimumCorner.x())
            maxDist.setX(m_vMinimumCorner.x() - v.x());
        else if (v.x() > m_vMaximumCorner.x())
            maxDist.setX(v.x() - m_vMaximumCorner.x());

        if (v.y() < m_vMinimumCorner.y())
            maxDist.setY(m_vMinimumCorner.y() - v.y());
        else if (v.y() > m_vMaximumCorner.y())
            maxDist.setY(v.y() - m_vMaximumCorner.y());

        if (v.z() < m_vMinimumCorner.z())
            maxDist.setZ(m_vMinimumCorner.z() - v.z());
        else if (v.z() > m_vMaximumCorner.z())
            maxDist.setZ(v.z() - m_vMaximumCorner.z());

        return maxDist.length();
    }
}
