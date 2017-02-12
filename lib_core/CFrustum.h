#ifndef CFRUSTUM_H
#define CFRUSTUM_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CBox3D.h"
#include "CSphere.h"
#include "CPlane.h"
#include "CGeometryGlobal.h"

// Qt
#include <QMatrix4x4>

class LIB_CORE_SHARED_EXPORT CFrustum
{
public:

    enum { eNear, eFar, eLeft, eRight, eTop, eBottom };

    //! Constructeur
    CFrustum() {}

    //! Constructeur
    CFrustum(const QMatrix4x4& modelViewProjectionMatrix);

    //! Met à jour le frustum
    void update(const QMatrix4x4& mvp);

    //! Teste l'intersetion avec un point
    EnumIntersectionType intersect(const QVector3D& point) const;

    //! Teste l'intersetion avec une sphere
    EnumIntersectionType intersect(const CSphere& sphere) const;

    //! Teste l'intersetion avec une boite
    EnumIntersectionType intersect(const CBox3D& box) const;

    bool isVisible(const CBox3D& box) const;

private:

    CPlane m_Planes[6];
};


#endif // CFRUSTUM_H
