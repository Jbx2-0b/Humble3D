#ifndef CTETRAHEDRON_H
#define CTETRAHEDRON_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"


class LIB_CORE_SHARED_EXPORT CTetrahedron : public CMesh
{
public:

    CTetrahedron();

    QVector3D getSize() const { return getBoundingBox().getSize(); }

    virtual ~CTetrahedron();

protected:

    virtual void updateGeometry();

    CSubMesh* m_pBuffer;
};


#endif // CTETRAHEDRON_H
