#ifndef CRAY_H
#define CRAY_H

// Foundations
#include "CMath.h"

// Qt
#include <QVector3D>


class CRay
{
public:

    CRay(const QVector3D& vOrigin, const QVector3D& vDirection)
    {
        m_vOrigin = vOrigin;
        m_vDirection = vDirection;
    }

    inline const QVector3D& getOrigin() const			{ return m_vOrigin; }
    inline const QVector3D& getDirection() const		{ return m_vDirection; }
    inline QVector3D point(real t) const                { return m_vOrigin + t * m_vDirection; }


protected:

    QVector3D m_vOrigin;
    QVector3D m_vDirection;
};

#endif // CRAY_H
