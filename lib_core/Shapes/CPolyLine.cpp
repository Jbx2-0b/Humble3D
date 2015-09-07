#include "Shapes/CPolyLine.h"
#include "CSceneManager.h"

//-----------------------------------------------------------------------
CPolyLine::CPolyLine()
    : CMesh("PolyLine")
    , m_pBuffer(0)
    , m_bClosed(true)
{
    setMultithreadingEnabled(true);
}

//-----------------------------------------------------------------------
void CPolyLine::updateGeometry()
{
    clearSubMeshs();

    m_pBuffer = createSubMesh();
    m_pBuffer->setPrimitiveType(ePrimitiveLine);

    CBuffer<QVector3D>& posBuffer = m_pBuffer->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = m_pBuffer->indicesBuffer();

    if (m_Points.size() < 2)
        return;

    posBuffer << m_Points[0];

    for (int i = 1; i < m_Points.count(); ++i)
    {
        posBuffer << m_Points[i];
        idBuffer << i - 1;
        idBuffer << i;
    }

    m_pBuffer->setDirty();
}
