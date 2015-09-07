#ifndef CSOLIDBOX_H
#define CSOLIDBOX_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"


class LIB_CORE_SHARED_EXPORT CSolidBox : public CMesh
{
public:

    CSolidBox(const QVector3D& size = QVector3D(1.0, 1.0, 1.0));

    void setSize(const QVector3D& size);

    QVector3D getSize() const { return m_vMax - m_vMin; }

    virtual ~CSolidBox();

protected:

    virtual void updateGeometry();

    CSubMesh* m_pCubeBuffer;

    QVector3D m_vMin, m_vMax;
};


#endif // CSOLIDBOX_H
