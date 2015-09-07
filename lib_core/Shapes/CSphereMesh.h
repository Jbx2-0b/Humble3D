#ifndef CSPHEREMESH_H
#define CSPHEREMESH_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"
#include "CMath.h"


class LIB_CORE_SHARED_EXPORT CSphereMesh : public CMesh
{
public:

    CSphereMesh(real dRadius = 1., int iRings = 32, int iSectors = 24);

    void setRadius(real dRadius);

    void setRingCount(int iRings);

    void setSectorCount(int iSectors);

    real getRadius() const { return m_dRadius; }

    int getRingCount() const { return m_iRings; }

    int getSectorCount() const { return m_iSectors; }

protected:

    real m_dRadius;
    int m_iRings;
    int m_iSectors;
    CSubMesh* m_pSphereSubMesh;

    virtual void updateGeometry();
};

#endif // CSPHEREMESH_H
