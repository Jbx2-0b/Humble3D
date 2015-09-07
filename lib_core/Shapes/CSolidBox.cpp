#include "Shapes/CSolidBox.h"
#include "CSubMesh.h"

//-----------------------------------------------------------------------------------------
CSolidBox::CSolidBox(const QVector3D& size /*= QVector3D(1.0, 1.0, 1.0)*/)
    : CMesh("SolidBox")
    , m_pCubeBuffer(0)
    , m_vMin(-size / 2.)
    , m_vMax(size / 2.)
{
    setMultithreadingEnabled(true);
}

//-----------------------------------------------------------------------------------------
CSolidBox::~CSolidBox()
{
}


//-----------------------------------------------------------------------------------------
void CSolidBox::setSize(const QVector3D& size)
{
    m_vMin = -size / 2.;
    m_vMax = size / 2;
    update();
}

//-----------------------------------------------------------------------------------------
void CSolidBox::updateGeometry()
{
    if (!m_pCubeBuffer)
    {
        m_pCubeBuffer = createSubMesh();
    }

    m_pCubeBuffer->clear();

    CBuffer<QVector3D>& posBuffer = m_pCubeBuffer->positionsBuffer();

    posBuffer << QVector3D(m_vMin.x(), m_vMin.y(),  m_vMax.z())
              << QVector3D(m_vMax.x(), m_vMin.y(),  m_vMax.z())
              << QVector3D(m_vMin.x(), m_vMax.y(),  m_vMax.z())
              << QVector3D(m_vMax.x(), m_vMax.y(),  m_vMax.z())
              << QVector3D(m_vMin.x(), m_vMin.y(),  m_vMin.z())
              << QVector3D(m_vMax.x(), m_vMin.y(),  m_vMin.z())
              << QVector3D(m_vMin.x(), m_vMax.y(),  m_vMin.z())
              << QVector3D(m_vMax.x(), m_vMax.y(),  m_vMin.z());


    CBuffer<IndiceType>& idBuffer = m_pCubeBuffer->indicesBuffer();

    idBuffer <<	7 << 3 << 5
             << 5 << 3 << 1
             << 2 << 6 << 4
             << 4 << 0 << 2
             << 0 << 5 << 1
             << 5 << 0 << 4
             << 2 << 3 << 7
             << 7 << 6 << 2
             << 4 << 7 << 5
             << 7 << 4 << 6
             << 0 << 3 << 2
             << 0 << 1 << 3;

    computeNormals();
    computeTangents();
}

