#include "CSierpinskiTetrahedronFactory.h"
#include "CMeshManager.h"
#include "CSceneManager.h"

CSierpinskiTetrahedronFactory::CSierpinskiTetrahedronFactory(CSceneManager* pSceneManager)
    : m_pSceneManager(pSceneManager)
    , m_Rank(0)
    , m_Count(0)
{
    m_pTetrahedronMesh = CMeshManager::getInstance().createCustomMesh<CTetrahedron>("TetrahedronMesh", "TetrahedronMesh");
    m_pTetrahedronMaterial = CMaterialManager::getInstance().createMaterial("Mat0");
    m_pTetrahedronMaterial->getRenderPass(0)->setShaderName("fresnel");
    m_pTetrahedronMaterial->setAmbientColor(0.8, 0.3, 0.2);

    QStringList fileNames;
    fileNames << ":/resources/xpos.png" << ":/resources/xneg.png" << ":/resources/ypos.png"
              << ":/resources/yneg.png" << ":/resources/zpos.png" << ":/resources/zneg.png";

    ATexture* pTexture = CTextureManager::getInstance().createTextureCube("TexCube", fileNames);
    m_pTetrahedronMaterial->addTexture(pTexture, eDiffuse);
}

void CSierpinskiTetrahedronFactory::compute(real size, int rank)
{
    m_Rank = rank;
    m_Count = 0;
    recursiveComputation(QVector3D(), size, rank);
    qDebug() << "Tetrahedron count: " << m_Count;
}

void CSierpinskiTetrahedronFactory::recursiveComputation(const QVector3D& center, real size, int currentIteration)
{
    if (currentIteration > 1)
    {
        real iterationSize = size / 2.0;
        QVector3D newSize = iterationSize * m_pTetrahedronMesh->getSize();

        recursiveComputation(center + QVector3D(+0.0 * newSize.x(), 0.0 * newSize.y(), +0.5 * newSize.z()), iterationSize, currentIteration - 1);
        recursiveComputation(center + QVector3D(-0.5 * newSize.x(), 0.0 * newSize.y(), -0.5 * newSize.z()), iterationSize, currentIteration - 1);
        recursiveComputation(center + QVector3D(+0.5 * newSize.x(), 0.0 * newSize.y(), -0.5 * newSize.z()), iterationSize, currentIteration - 1);
        recursiveComputation(center + QVector3D(0.0, 1.0 * newSize.y(), sqrt(1.0 - pow((2.0/3.0) * sqrt(3.0/4.0), 2))),
                             iterationSize, currentIteration - 1);

        recursiveComputation(center + QVector3D(0.0, 1.0 * newSize.y(), 0), iterationSize, currentIteration - 1);
    }
    else
    {
        CMeshInstance* pTetrahedron = m_pSceneManager->createMeshInstance(m_pTetrahedronMesh);
        CSceneNode* pRootNode = m_pSceneManager->getRootNode();
        CSceneNode* pTetrahedronNode = pRootNode->createChild(QString("Tetrahedron%1").arg(currentIteration), center);
        pTetrahedronNode->scale(size);
        pTetrahedronNode->addItem(pTetrahedron);
        pTetrahedron->setMaterialName(m_pTetrahedronMaterial->getName());
        m_Count++;
    }
}
