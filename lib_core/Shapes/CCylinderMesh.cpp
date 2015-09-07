#include "Shapes/CCylinderMesh.h"
#include "CMath.h"

//-----------------------------------------------------------------------------------------
CCylinderMesh::CCylinderMesh(real dRadius /*= 0.5*/, real dHeight /*= 1.*/ , int iSegmentCount /*= DefaultCylinderSegmentCount*/)
    : CMesh("Cylinder")
    , m_pCylinderBuffer(0)
    , m_dRadius(dRadius)
    , m_dHeight(dHeight)
    , m_iSegmentCount(iSegmentCount)
{
}

//-----------------------------------------------------------------------------------------
void CCylinderMesh::updateGeometry()
{
    m_pCylinderBuffer = createSubMesh();

    CBuffer<QVector3D>& posBuffer = m_pCylinderBuffer->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = m_pCylinderBuffer->indicesBuffer();

    // Construction du cercle supérieur

    // Centre
    posBuffer << QVector3D(0., 0., m_dHeight / 2);

    for (int i = 0; i < m_iSegmentCount; ++i)
    {
        real theta = (2. * Math::Pi / (real) m_iSegmentCount) * (real) i;

        QVector3D v(m_dRadius * qCos(theta), m_dRadius * qSin(theta), m_dHeight / 2.);

        posBuffer << v;

        if (i < m_iSegmentCount - 1)
        {
            idBuffer << 0 << i + 1 << i + 2;
        }
        else
        {
            idBuffer << 0 << i + 1 << 1;
        }
    }

    // Construction du cercle inférieur

    int iCenterID = posBuffer.size();
    // Centre
    posBuffer << QVector3D(0., 0., - m_dHeight / 2);

    int iStartID = posBuffer.size();

    for (int i = 0; i < m_iSegmentCount; ++i)
    {
        real theta = (2. * Math::Pi / (real) m_iSegmentCount) * (real) i;

        posBuffer << QVector3D(m_dRadius * qCos(theta), m_dRadius  * qSin(theta), - m_dHeight / 2.);

        if (i < m_iSegmentCount - 1)
        {
            idBuffer << i + iStartID << iCenterID << i + iStartID + 1;
        }
        else
        {
            idBuffer << i + iStartID << iCenterID << iStartID;
        }
    }

    // Faces intermédiaires
    for (int i = 1; i < m_iSegmentCount; ++i)
    {
        idBuffer << i + 1 << i << i + 1 + m_iSegmentCount;
        idBuffer << i + 1 + m_iSegmentCount << i + 2 + m_iSegmentCount << i + 1;
    }

    idBuffer << 1 << m_iSegmentCount << m_iSegmentCount + 2;
    idBuffer << m_iSegmentCount + 2 << m_iSegmentCount << m_iSegmentCount * 2 + 1;

    computeNormals();
    computeTangents();
}

//-----------------------------------------------------------------------------------------
CCylinderMesh::~CCylinderMesh()
{
}
