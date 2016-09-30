#include "Shapes/CPyramid.h"
#include "CSubMesh.h"

//-----------------------------------------------------------------------------------------
CPyramid::CPyramid(const QVector3D& size /*= QVector3D(1.0, 1.0, 1.0)*/)
    : CMesh("SolidBox")
    , m_pBuffer(0)
    , m_Size(size)
{
    setMultithreadingEnabled(true);
}

//-----------------------------------------------------------------------------------------
CPyramid::~CPyramid()
{
}


//-----------------------------------------------------------------------------------------
void CPyramid::setSize(const QVector3D& size)
{
    m_Size = size;
    update();
}

//-----------------------------------------------------------------------------------------
void CPyramid::updateGeometry()
{
    clearSubMeshs();
    CSubMesh* pSubMesh = createSubMesh();

    CBuffer<QVector3D>& posBuffer = pSubMesh->positionsBuffer();

    real halfWidth = m_Size.x() / 2.;
    real halfHeight = m_Size.z() / 2.;

    QVector3D vA(-halfWidth, 0.0,  +halfHeight);
    QVector3D vB(-halfWidth, 0.0,  -halfHeight);
    QVector3D vC(+halfWidth, 0.0,  -halfHeight);
    QVector3D vD(+halfWidth, 0.0,  +halfHeight);
    QVector3D vE(0.0, m_Size.y(),  0.0);

    posBuffer
            << vA << vE << vB
            << vB << vE << vC
            << vC << vE << vD
            << vD << vE << vA
            << vA << vB << vD
            << vB << vC << vD;


    CBuffer<IndiceType>& idBuffer = pSubMesh->indicesBuffer();
    idBuffer.resize(18);
    std::iota(idBuffer.begin(), idBuffer.end(), 0);

    computeNormals();
    computeTangents();
}

