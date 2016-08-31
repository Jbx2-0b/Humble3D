#include "CBillboard.h"
#include "CShaderManager.h"
#include "CMeshManager.h"
#include "ARenderer.h"
#include "CGeometryGlobal.h"

// Qt
#include <QTimer>

//--------------------------------------------------------------------------
CBillboard::CBillboard(CSceneManager* pSceneManager, const QString& name)
    : ARenderableItem(pSceneManager, name)
    , m_fSize(1.0f)
{
    m_MeshBuffer.setPrimitiveType(ePrimitivePoint);
    m_MeshBuffer.setUsagePattern(eDynamicDraw);
}

//--------------------------------------------------------------------------
CBillboard::~CBillboard()
{
}

//--------------------------------------------------------------------------
void CBillboard::addPosition(const QVector3D& position)
{
    addPositions(QVector<QVector3D>() << position);
}

//--------------------------------------------------------------------------
void CBillboard::addPositions(const QVector<QVector3D>& positions)
{
    CBuffer<QVector3D>& posBuffer = m_MeshBuffer.positionsBuffer();
    CBuffer<IndiceType>& idBuffer = m_MeshBuffer.indicesBuffer();

    foreach (const QVector3D& position, positions)
    {
        posBuffer << position;
        idBuffer << idBuffer.size();
    }

    posBuffer.setDirty();
    idBuffer.setDirty();
    m_MeshBuffer.setDirty();
}

//--------------------------------------------------------------------------
void CBillboard::clearPositions()
{
    m_MeshBuffer.clear();
    m_MeshBuffer.setDirty();
}

//--------------------------------------------------------------------------
void CBillboard::render(ARenderer* pRenderer)
{
    if (CSceneNode* pNode = getNode())
    {
        if (CCamera* pCamera = pRenderer->getCurrentCamera())
        {
            QMatrix4x4 currentMVMatrix = pCamera->getViewMatrix() * pNode->getGlobalTransformation();

            if (currentMVMatrix != m_CurrentModelViewMatrix)
            {
                m_CurrentModelViewMatrix = currentMVMatrix;

                CBuffer<QVector3D>& posBuffer = m_MeshBuffer.positionsBuffer();

                std::sort(posBuffer.begin(), posBuffer.end(), CSortByZValue(currentMVMatrix));

                posBuffer.setDirty();
                m_MeshBuffer.setDirty();
            }

            pRenderer->renderMeshBuffer(&m_MeshBuffer);
        }
    }
}


