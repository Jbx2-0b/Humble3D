#ifndef CTRIANGLE_H
#define CTRIANGLE_H

// Foundations
#include "CPlane.h"

// Qt
#include <QVector3D>

class CTriangle
{
public:

    CTriangle() {}

    CTriangle(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3)
        : m_v1(v1)
        , m_v2(v2)
        , m_v3(v3)
    {}

    void transform(const QMatrix4x4& transformation)
    {
        m_v1 = transformation * m_v1;
        m_v2 = transformation * m_v2;
        m_v3 = transformation * m_v3;
    }

    CTriangle transformed(const QMatrix4x4& transformation)
    {
        return CTriangle(transformation * m_v1, transformation * m_v2, transformation * m_v3);
    }

    inline CPlane plane() const
    {
        return CPlane(m_v1, m_v2, m_v3);
    }

    bool intersection(const CRay& ray, real* dDistance) const
    {
        *dDistance = std::numeric_limits<real>::infinity();

        std::pair<bool, real> i = plane().intersects(ray);

        if (i.first)
        {
            QVector3D pt = ray.point(i.second);
            if (contains(pt))
            {
                *dDistance = i.second;
                return true;
            }
        }

        return false;
    }

    bool contains(const QVector3D& point) const
    {
        bool b1, b2, b3;

        b1 = sign(point, m_v1, m_v2) < 0.0f;
        b2 = sign(point, m_v2, m_v3) < 0.0f;
        b3 = sign(point, m_v3, m_v1) < 0.0f;

        return ((b1 == b2) && (b2 == b3));
    }

    bool isInside(QVector3D aPt)
    {
        bool b1, b2, b3;

        b1 = sign(aPt, m_v1, m_v2) < 0.0f;
        b2 = sign(aPt, m_v2, m_v3) < 0.0f;
        b3 = sign(aPt, m_v3, m_v1) < 0.0f;

        return ((b1 == b2) && (b2 == b3));
    }

private:

    static real sign(const QVector3D& p1, const QVector3D& p2, const QVector3D& p3)
    {
        return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
    }

    QVector3D m_v1;
    QVector3D m_v2;
    QVector3D m_v3;

};

#endif // CTRIANGLE_H
