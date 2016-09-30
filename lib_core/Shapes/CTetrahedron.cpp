#include "Shapes/CTetrahedron.h"
#include "CSubMesh.h"

//-----------------------------------------------------------------------------------------
CTetrahedron::CTetrahedron()
    : CMesh("SolidBox")
    , m_pBuffer(0)
{
    setMultithreadingEnabled(true);
}

//-----------------------------------------------------------------------------------------
CTetrahedron::~CTetrahedron()
{
}


//-----------------------------------------------------------------------------------------
void CTetrahedron::updateGeometry()
{
    clearSubMeshs();
    CSubMesh* pSubMesh = createSubMesh();

    CBuffer<QVector3D>& posBuffer = pSubMesh->positionsBuffer();

    QVector3D vA(-0.5, 0.0,  0.0);
    QVector3D vB(+0.5, 0.0,  0.0);
    QVector3D vC(0.0, 0.0,  sqrt(0.75));
    QVector3D vD(0.0, sqrt(1.0 - pow(2.0/3.0 * sqrt(3.0/4.0), 2)),  sqrt(0.75) / 3.);

    posBuffer
            << vA << vC << vD
            << vC << vB << vD
            << vB << vA << vD
            << vC << vA << vB;


    CBuffer<IndiceType>& idBuffer = pSubMesh->indicesBuffer();
    idBuffer.resize(posBuffer.size());
    std::iota(idBuffer.begin(), idBuffer.end(), 0);

    computeNormals();
    computeTangents();

    qDebug() << getBoundingBox().getSize();
}

