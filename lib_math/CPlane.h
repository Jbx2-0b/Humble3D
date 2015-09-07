#ifndef CPLANE_H
#define CPLANE_H

// Foundations
#include "CRay.h"
#include "CMath.h"
#include "CBox3D.h"

// Qt
#include <QtCore>
#include <QVector3D>

// STL
#include <limits>

class CPlane
{

public:

    enum EnumSide
    {
        eNoSide,
        eNegativeSide,
        ePositiveSide,
        eBothSide
    };

    //! Constructeur
    CPlane() : m_fD(0.) {}

    //! Constructeur
    CPlane (const QVector3D& normal, real constant)
    {
        m_vNormal = normal;
        m_fD = constant;
    }

    //! Constructeur
    CPlane (real a, real b, real c, real d)
        : m_vNormal(a, b, c), m_fD(d) {}

    //! Constructeur
    CPlane (const QVector3D& normal, const QVector3D& point)
    {
        redefine(normal, point);
    }

    //! Constructeur
    CPlane(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3)
    {
        redefine(v1, v2, v3);
    }

    QVector3D getNormal() const
    {
        return m_vNormal;
    }

    real getConstant() const
    {
        return m_fD;
    }

    void setConstant(real fConstant)
    {
        m_fD = fConstant;
    }

    EnumSide getSide(const QVector3D& point) const
    {
        real fDistance = getDistance(point);

        if (fDistance < 0.0)
            return eNegativeSide;

        if (fDistance > 0.0)
            return ePositiveSide;

        return eNoSide;
    }

    EnumSide getSide(const CBox3D& box) const
    {
        if (box.isNull())
            return eNoSide;

        return getSide(box.getCenter(), box.getHalfSize());
    }

    EnumSide getSide(const QVector3D& centre, const QVector3D& halfSize) const
    {
        // Calculate the distance between box centre and the plane
        real dist = getDistance(centre);

        // Calculate the maximise allows absolute distance for
        // the distance between box centre and plane
        real maxAbsDist = Math::absDotProduct(m_vNormal, halfSize);

        if (dist < -maxAbsDist)
            return eNegativeSide;

        if (dist > +maxAbsDist)
            return ePositiveSide;

        return eBothSide;
    }

    real getDistance(const QVector3D& point) const
    {
        return QVector3D::dotProduct(m_vNormal, point) + m_fD;
    }

    void redefine(const QVector3D& point0, const QVector3D& point1, const QVector3D& point2)
    {
        QVector3D edge1 = point1 - point0;
        QVector3D edge2 = point2 - point0;
        m_vNormal = QVector3D::crossProduct(edge1, edge2);
        m_vNormal.normalize();
        m_fD = -QVector3D::dotProduct(m_vNormal, point0);
    }

    void redefine(const QVector3D& normal, const QVector3D& point)
    {
        m_vNormal = normal;
        m_fD = -QVector3D::dotProduct(normal, point);
    }

    QVector3D projectVector(const QVector3D& v) const
    {
        // We know plane normal is unit length, so use simple method
        QMatrix4x4 xform;
        xform(0, 0) = 1.0f - m_vNormal.x() * m_vNormal.x();
        xform(0, 1) = -m_vNormal.x() * m_vNormal.y();
        xform(0, 2) = -m_vNormal.x() * m_vNormal.z();
        xform(1, 0) = -m_vNormal.y() * m_vNormal.x();
        xform(1, 1) = 1.0f - m_vNormal.y() * m_vNormal.y();
        xform(1, 2) = -m_vNormal.y() * m_vNormal.z();
        xform(2, 0) = -m_vNormal.z() * m_vNormal.x();
        xform(2, 1) = -m_vNormal.z() * m_vNormal.y();
        xform(2, 2) = 1.0f - m_vNormal.z() * m_vNormal.z();
        return xform * v;
    }

    real normalize(void)
    {
        real fLength = m_vNormal.length();
        m_vNormal /= fLength;
        m_fD  /= fLength;
        return fLength;
    }

    std::pair<bool, real> intersects(const CRay& ray)
    {

        real denom = QVector3D::dotProduct(m_vNormal, ray.getDirection());
        if (qAbs(denom) < std::numeric_limits<real>::epsilon())
        {
            // Parallel
            return std::pair<bool, real>(false, 0);
        }
        else
        {
            real nom = QVector3D::dotProduct(m_vNormal, ray.getOrigin()) + m_fD;
            real t = -(nom/denom);
            return std::pair<bool, real>(t >= 0, t);
        }
    }

    bool operator==(const CPlane& rhs) const
    {
          return (rhs.m_fD == m_fD && rhs.m_vNormal == m_vNormal);
    }

    bool operator!=(const CPlane& rhs) const
    {
        return (rhs.m_fD != m_fD || rhs.m_vNormal != m_vNormal);
    }


private:

    QVector3D m_vNormal;
    real m_fD;
};

#endif // CPLANE_H
