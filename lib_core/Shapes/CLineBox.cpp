#include "Shapes/CLineBox.h"
#include "CSubMesh.h"

//-----------------------------------------------------------------------------------------
CLineBox::CLineBox(const QVector3D& size /*= QVector3D(1.0, 1.0, 1.0)*/)
    : CMesh("LineBox")
    , m_pCubeBuffer(0)
    , m_vMin(-size / 2.)
    , m_vMax(size / 2.)
{
    setMultithreadingEnabled(true);
}

//-----------------------------------------------------------------------------------------
CLineBox::~CLineBox()
{
}

//-----------------------------------------------------------------------------------------
void CLineBox::setSize(const QVector3D& size)
{
    m_vMin = -size / 2.;
    m_vMax = size / 2;
    update();
}

//-----------------------------------------------------------------------------------------
void CLineBox::setBoundingBox(const CBox3D& bbox)
{
    m_vMin = bbox.getMinimum();
    m_vMax = bbox.getMaximum();
    update();
}

//-----------------------------------------------------------------------------------------
void CLineBox::updateGeometry()
{
    if (!m_pCubeBuffer)
    {
        m_pCubeBuffer = createSubMesh();
        m_pCubeBuffer->setPrimitiveType(ePrimitiveLine);
    }

    m_pCubeBuffer->clear();

    CBuffer<QVector3D>& posBuffer = m_pCubeBuffer->positionsBuffer();

    posBuffer
            << QVector3D(m_vMin.x(), m_vMin.y(),  m_vMax.z())
            << QVector3D(m_vMax.x(), m_vMin.y(),  m_vMax.z())
            << QVector3D(m_vMin.x(), m_vMax.y(),  m_vMax.z())
            << QVector3D(m_vMax.x(), m_vMax.y(),  m_vMax.z())
            << QVector3D(m_vMin.x(), m_vMin.y(),  m_vMin.z())
            << QVector3D(m_vMax.x(), m_vMin.y(),  m_vMin.z())
            << QVector3D(m_vMin.x(), m_vMax.y(),  m_vMin.z())
            << QVector3D(m_vMax.x(), m_vMax.y(),  m_vMin.z());

    CBuffer<IndiceType>& idBuffer = m_pCubeBuffer->indicesBuffer();

    idBuffer
            << 0 << 1
            << 1 << 3
            << 2 << 3
            << 3 << 7
            << 7 << 5
            << 1 << 5
            << 5 << 4
            << 4 << 6
            << 7 << 6
            << 6 << 2
            << 2 << 0
            << 4 << 0;
}

