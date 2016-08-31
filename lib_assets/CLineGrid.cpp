#include "CLineGrid.h"
#include "CSceneManager.h"

//-----------------------------------------------------------------------------------------
CLineGrid::CLineGrid(real dLength,
                     real dWidth,
                     int iLengthSquareCount,
                     int iWidthSquareCount)
    : CMesh("LineGrid")
    , m_dLength(dLength)
    , m_dWidth(dWidth)
    , m_iLengthSquareCount(iLengthSquareCount)
    , m_iWidthSquareCount(iWidthSquareCount)
{
    m_pGridBuffer = createSubMesh();
}

//-----------------------------------------------------------------------------------------
void CLineGrid::updateGeometry()
{
    real dStepL = m_dLength / (real) m_iLengthSquareCount;
    real dStepW = m_dWidth / (real) m_iWidthSquareCount;

    real dCenterL = m_dLength / 2.;
    real dCenterW = m_dWidth / 2.;

    m_pGridBuffer->clear();
    m_pGridBuffer->setPrimitiveType(ePrimitiveLine);

    CBuffer<QVector3D>& posBuffer = m_pGridBuffer->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = m_pGridBuffer->indicesBuffer();

    for (int l = 0; l < m_iLengthSquareCount + 1; ++l)
    {
        for (int w = 0; w < m_iWidthSquareCount + 1; ++w)
        {
            posBuffer << QVector3D((real) l * dStepL - dCenterL, 0, (real) w * dStepW - dCenterW);

            int i = w + (m_iWidthSquareCount + 1) * l;

            if (l < m_iLengthSquareCount && w < m_iWidthSquareCount)
            {
                idBuffer << i << i + 1;
                idBuffer << i << i + m_iWidthSquareCount + 1;
            }
            else if (l == m_iLengthSquareCount && w != m_iWidthSquareCount)
            {
                idBuffer << i << i + 1;
            }
            else if (w == m_iWidthSquareCount && l != m_iLengthSquareCount)
            {
                idBuffer << i << i + m_iWidthSquareCount + 1;
            }
        }
    }

    setMaterial(eWhite);
}

//-----------------------------------------------------------------------------------------
CLineGrid::~CLineGrid()
{
}
