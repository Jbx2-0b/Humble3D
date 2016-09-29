#ifndef CTETRAHEDRON_H
#define CTETRAHEDRON_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"


class LIB_CORE_SHARED_EXPORT CTetrahedron : public CMesh
{
public:

    CTetrahedron(const QVector3D& size = QVector3D(1.0, 1.0, 1.0));

    void setSize(const QVector3D& size);

    QVector3D getSize() const { return m_Size; }

    virtual ~CTetrahedron();

protected:

    virtual void updateGeometry();

    CSubMesh* m_pBuffer;

    QVector3D m_Size;
};


#endif // CTETRAHEDRON_H
