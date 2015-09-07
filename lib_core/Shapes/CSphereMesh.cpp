#include "Shapes/CSphereMesh.h"

//-----------------------------------------------------------------------------------------
CSphereMesh::CSphereMesh(real dRadius, int iRings, int iSectors)
    : CMesh("Sphere")
    , m_dRadius(dRadius)
    , m_iRings(iRings)
    , m_iSectors(iSectors)
{
    setMultithreadingEnabled(true);
    m_pSphereSubMesh = createSubMesh();
}

//-----------------------------------------------------------------------------------------
void CSphereMesh::setRadius(real dRadius)
{
    m_dRadius = dRadius;
    update();
}

//-----------------------------------------------------------------------------------------
void CSphereMesh::setRingCount(int iRings)
{
    m_iRings = iRings;
    update();
}

//-----------------------------------------------------------------------------------------
void CSphereMesh::setSectorCount(int iSectors)
{
    m_iSectors = iSectors;
    update();
}

//-----------------------------------------------------------------------------------------
void CSphereMesh::updateGeometry()
{
    m_pSphereSubMesh->clear();

    QVector<QVector3D>& posBuffer = m_pSphereSubMesh->positionsBuffer();
    QVector<QVector3D>& nBuffer = m_pSphereSubMesh->normalsBuffer();
    QVector<QVector3D>& texcoordBuffer = m_pSphereSubMesh->texCoordBuffer(0);
    CBuffer<IndiceType>& idBuffer = m_pSphereSubMesh->indicesBuffer();

    const real R = 1./(real)(m_iRings - 1);
    const real S = 1./(real)(m_iSectors - 1);
    int r, s;


    for (r = 0; r < m_iRings; r++)
    {
        for(s = 0; s < m_iSectors; s++)
        {
            real y = sin(- Math::HalfPi + Math::Pi * r * R);
            real x = cos(2 * Math::Pi * s * S) * sin(Math::Pi * r * R);
            real z = sin(2 * Math::Pi * s * S) * sin(Math::Pi * r * R);

            posBuffer << QVector3D(x * m_dRadius, y * m_dRadius, z * m_dRadius);
            texcoordBuffer << QVector3D(s * S, r * R, 0.);
            nBuffer << QVector3D(x, y, z);
        }
    }

    for(r = 0; r < m_iRings - 1; r++)
    {
        for(s = 0; s < m_iSectors - 1; s++)
        {
            idBuffer << r * m_iSectors + (s+1) << r * m_iSectors + s << (r+1) * m_iSectors + (s+1);
            idBuffer << (r+1) * m_iSectors + s << (r+1) * m_iSectors + (s+1) << r * m_iSectors + s;
        }
    }
}
