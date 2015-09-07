#include "Shapes/CPlaneMesh.h"

//-----------------------------------------------------------------------------------------
CPlaneMesh::CPlaneMesh(real dLength,
                       real dWidth,
                       int iLengthSquareCount,
                       int iWidthQuadCount)
    : CMesh("PlaneMesh")
    , m_ePlaneAxis(ePlaneXZ)
    , m_dLength(dLength)
    , m_dWidth(dWidth)
    , m_iLengthQuadCount(iLengthSquareCount)
    , m_iWidthQuadCount(iWidthQuadCount)
    , m_fLengthTextureRepeat(1.0)
    , m_fWidthTextureRepeat(1.0)
{
    m_pGridBuffer = createSubMesh();
}

//-----------------------------------------------------------------------------------------
void CPlaneMesh::updateGeometry()
{
    m_pGridBuffer->clear();

    real dStepL = m_dLength / (real) m_iLengthQuadCount;
    real dStepW = m_dWidth / (real) m_iWidthQuadCount;

    real dCenterL = m_dLength / 2.;
    real dCenterW = m_dWidth / 2.;

    CBuffer<QVector3D>& posBuffer = m_pGridBuffer->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = m_pGridBuffer->indicesBuffer();
    CBuffer<QVector3D>& texCoordBuffer = m_pGridBuffer->texCoordBuffer(0);

    for (int l = 0; l < m_iLengthQuadCount + 1; ++l)
    {
        for (int w = 0; w < m_iWidthQuadCount + 1; ++w)
        {
            switch (m_ePlaneAxis)
            {
            case ePlaneXY:
                posBuffer << QVector3D((real) l * dStepL - dCenterL, (real) w * dStepW - dCenterW, 0.);
                break;
            case ePlaneXZ:
                posBuffer << QVector3D((real) l * dStepL - dCenterL, 0, (real) w * dStepW - dCenterW);
                break;
            case ePlaneYZ:
                posBuffer << QVector3D(0, (real) l * dStepL - dCenterL, (real) w * dStepW - dCenterW);
                break;
            }

            texCoordBuffer << QVector3D(l / ((real)m_iLengthQuadCount / m_fLengthTextureRepeat), w / ((real)m_iWidthQuadCount / m_fWidthTextureRepeat), 0.);

            int i = w + (m_iWidthQuadCount + 1) * l;

            if (l < m_iLengthQuadCount && w < m_iWidthQuadCount)
            {
                idBuffer << i		<< i + 1						<< i + m_iWidthQuadCount + 1;
                idBuffer << i + 1	<< i + m_iWidthQuadCount + 2	<< i + m_iWidthQuadCount + 1;
            }
        }
    }

    computeNormals();
    computeTangents();

    m_pGridBuffer->setDirty();
}

//-----------------------------------------------------------------------------------------
CPlaneMesh::~CPlaneMesh()
{
    removeSubMesh(m_pGridBuffer);
}
