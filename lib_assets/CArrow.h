#ifndef CARROW_H
#define CARROW_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CMesh.h"
#include "CGeometryGlobal.h"

class LIB_ASSETS_SHARED_EXPORT CArrow : public CMesh
{
public:

    //! Constructeur
    CArrow();

    void setDirection(const QVector3D& direction);

    //! Destructeur
    virtual ~CArrow();

protected:

    virtual void updateGeometry();

    QVector3D m_Origin;
    QVector3D m_Direction;
};


#endif // CARROW_H
