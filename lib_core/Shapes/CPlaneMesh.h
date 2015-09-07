#ifndef CPLANEMESH_H
#define CPLANEMESH_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMesh.h"

static const real DefaultPlaneMeshSize = 1.;
static const int DefaultPlaneMeshQuadCount = 3;

class LIB_CORE_SHARED_EXPORT CPlaneMesh : public CMesh
{
public:

    enum EnumPlaneAxis
    {
        ePlaneXY,
        ePlaneXZ,
        ePlaneYZ
    };

    //! Constructeur
    CPlaneMesh(real dLength = DefaultPlaneMeshSize,
               real dWidth =  DefaultPlaneMeshSize,
               int iLengthSquareCount = DefaultPlaneMeshQuadCount,
               int iWidthSquareCount = DefaultPlaneMeshQuadCount);

    void init(
            EnumPlaneAxis ePlaneAxis,
            real dLength,
            real dWidth,
            int iLengthSquareCount,
            int iWidthSquareCount,
            real fLengthTextureRepeat = 1.0,
            real fWidthTextureRepeat = 1.0)
    {
        m_ePlaneAxis = ePlaneAxis;
        m_dLength = dLength;
        m_dWidth = dWidth;
        m_iLengthQuadCount = iLengthSquareCount;
        m_iWidthQuadCount = iWidthSquareCount;
        m_fLengthTextureRepeat = fLengthTextureRepeat;
        m_fWidthTextureRepeat = fWidthTextureRepeat;

        update();
    }

    //! Destructeur
    virtual ~CPlaneMesh();

protected:

    virtual void updateGeometry();
    CSubMesh* m_pGridBuffer;
    EnumPlaneAxis m_ePlaneAxis;
    real m_dLength;
    real m_dWidth;
    int m_iLengthQuadCount;
    int m_iWidthQuadCount;
    real m_fLengthTextureRepeat;
    real m_fWidthTextureRepeat;
};

#endif // CPLANEMESH_H
