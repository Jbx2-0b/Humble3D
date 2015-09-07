#include "CDebugManager.h"
#include "Shapes/CLineBox.h"
#include "Shapes/CSphereMesh.h"
#include "Shapes/CCylinderMesh.h"
#include <QDebug>


CDebugManager* CDebugManager::s_pInstance = 0;
QMutex CDebugManager::s_Mutex(QMutex::Recursive);

//-----------------------------------------------------------------------------------------
CDebugManager::CDebugManager(CSceneManager* pSceneManager)
: m_pSceneManager(pSceneManager)
{}

//-----------------------------------------------------------------------------------------
CDebugManager::~CDebugManager()
{
    foreach (CMeshInstance* pMeshInstance, m_MeshInstances)
    {
        m_pSceneManager->deleteMeshInstance(pMeshInstance);
    }

    foreach (CMesh* pMesh, m_Meshs)
    {
        CMeshManager::getInstance()->removeMesh(pMesh);
    }
}

//-----------------------------------------------------------------------------------------
void CDebugManager::setVisible(bool bVisible)
{
    foreach (CMeshInstance* pMeshInstance, m_MeshInstances)
    {
        pMeshInstance->setVisible(bVisible);
    }
}

//-----------------------------------------------------------------------------------------
CMeshInstance* CDebugManager::createBox(const CBox3D& box, const Color& color, real scaleFactor /*= 1.0*/)
{
    CLineBox* pBox = CMeshManager::getInstance()->createCustomMesh<CLineBox>("CLineBox");
    m_Meshs << pBox;
    QMatrix4x4 scaleMatrix;
    scaleMatrix.scale(scaleFactor);
    pBox->setBoundingBox(box.transformed(scaleMatrix));
    CMeshInstance* pBoxInstance = m_pSceneManager->createMeshInstance(pBox, "DebugMeshInstance");
    pBoxInstance->setMaterialName(CMaterialManager::getInstance()->getMaterialNameByColor(color));
    m_MeshInstances << pBoxInstance;
    pBoxInstance->setSelectable(false);
    return pBoxInstance;
}

//-----------------------------------------------------------------------------------------
CMeshInstance* CDebugManager::createSphere(const CSphere& sphere, const Color& color, real scaleFactor /*= 1.0*/)
{
    // TODO
    return 0;
}

//-----------------------------------------------------------------------------------------
CMeshInstance* CDebugManager::createCylinder(const CCylinder& Cylinder, const Color& color, real scaleFactor /*= 1.0*/)
{
    // TODO
    return 0;
}

//-----------------------------------------------------------------------------------------
CMeshInstance* CDebugManager::createNormals(CMeshInstance* pMeshInstance, const Color& color, real fSize /*= 1.0*/)
{
    CMesh* pMesh = CMeshManager::getInstance()->createMesh(pMeshInstance->getName() + "NormalMesh");
    m_Meshs << pMesh;

    CSubMesh* pNormalSubMesh = pMesh->createSubMesh();

    pNormalSubMesh->setPrimitiveType(ePrimitiveLine);

    CBuffer<QVector3D>& posBuffer = pNormalSubMesh->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = pNormalSubMesh->indicesBuffer();

    foreach (CSubMesh* pSubMesh, pMeshInstance->getMesh()->subMeshs())
    {
        if (pSubMesh->normalsBuffer().count() == pSubMesh->positionsBuffer().count())
        {
            for (int i = 0; i < pSubMesh->positionsBuffer().count(); ++i)
            {
                posBuffer << pSubMesh->positionsBuffer()[i];
                posBuffer <<  pSubMesh->positionsBuffer()[i] + fSize * pSubMesh->normalsBuffer()[i];

                idBuffer << i * 2 << i * 2 + 1;
            }
        }
    }

    CMeshInstance* pNormalMeshInstance = m_pSceneManager->createMeshInstance(pMesh, pMeshInstance->getName() + "NormalMeshInstance");
    pNormalMeshInstance->setMaterialName(CMaterialManager::getInstance()->getMaterialNameByColor(color));

    foreach (CSceneNode* pSceneNode, pMeshInstance->getNodes())
    {
        pSceneNode->addItem(pNormalMeshInstance);
    }

    m_MeshInstances << pNormalMeshInstance;
    pNormalMeshInstance->setSelectable(false);

    return pMeshInstance;
}

//-----------------------------------------------------------------------------------------
CMeshInstance* CDebugManager::createFrustum(CCamera* pCamera, const Color& color)
{
    CMesh* pMesh = CMeshManager::getInstance()->createMesh(pCamera->getName() + "FrustumMesh");
    m_Meshs << pMesh;

    CSubMesh* pSubMesh = pMesh->createSubMesh();

    pSubMesh->setPrimitiveType(ePrimitiveLine);

    CBuffer<QVector3D>& posBuffer = pSubMesh->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = pSubMesh->indicesBuffer();

    real halfHeight = qTan(pCamera->getVerticalFOV() / 2.); //half the height of the frustum at z=1;
    real halfWidth = halfHeight * pCamera->getAspectRatio(); //same for width

    QVector3D nearRight = halfHeight * pCamera->getNear() * pCamera->getRight().normalized();
    QVector3D nearTop = halfWidth * pCamera->getNear() * pCamera->getUp().normalized();
    QVector3D nearCenter = pCamera->getEyePosition() + pCamera->getNear() * pCamera->getForward().normalized();

    QVector3D farRight = halfHeight * pCamera->getFar() * pCamera->getRight().normalized();
    QVector3D farTop = halfWidth * pCamera->getFar() * pCamera->getUp().normalized();
    QVector3D farCenter = pCamera->getEyePosition() + pCamera->getFar() * pCamera->getForward().normalized();

    QVector3D ntl = nearCenter - nearRight + nearTop; //near top left
    QVector3D ntr = nearCenter + nearRight + nearTop; //near top right
    QVector3D nbr = nearCenter + nearRight - nearTop; //near bottom right
    QVector3D nbl = nearCenter - nearRight - nearTop; //near bottom right

    QVector3D ftl = farCenter - farRight + farTop; //far top left
    QVector3D ftr = farCenter + farRight + farTop; //far top right
    QVector3D fbr = farCenter + farRight - farTop; //far bottom right
    QVector3D fbl = farCenter - farRight - farTop; //far bottom right

    posBuffer << ntl;
    posBuffer << ntr;
    posBuffer << nbr;
    posBuffer << nbl;
    posBuffer << ftl;
    posBuffer << ftr;
    posBuffer << fbr;
    posBuffer << fbl;
    posBuffer << pCamera->getEyePosition();
    posBuffer << pCamera->getForward() * pCamera->getFar();


    idBuffer << 8 << 4
             << 8 << 5
             << 8 << 6
             << 8 << 7
             << 4 << 5
             << 5 << 6
             << 6 << 7
             << 7 << 4;

    CMeshInstance* pMeshInstance = m_pSceneManager->createMeshInstance(pMesh, pCamera->getName() + "FrustumMeshInstance");
    pMeshInstance->setMaterialName(CMaterialManager::getInstance()->getMaterialNameByColor(color));
    pMeshInstance->setSelectable(false);

    m_MeshInstances << pMeshInstance;

    return pMeshInstance;
}

//-----------------------------------------------------------------------------------------
CMeshInstance* CDebugManager::createLight(CLight* pLight, const Color& color)
{
    // TODO
    return 0;
}

//-----------------------------------------------------------------------------------------
void CDebugManager::remove(CMeshInstance* pMeshInstance)
{
    m_MeshInstances.removeOne(pMeshInstance);
    m_pSceneManager->deleteMeshInstance(pMeshInstance);
}

/*
//-----------------------------------------------------------------------------------------
void CMesh::createTangentsSubMesh()
{
    if (m_bNeedTangentsSubMeshUpdate)
    {
        if (!m_pTangentsSubMesh)
        {
            m_pTangentsSubMesh = createSubMesh();
        }
        m_pTangentsSubMesh->clear();

        CBuffer<QVector3D>& posBuffer = m_pTangentsSubMesh->positionsBuffer();
        CBuffer<IndiceType>& idBuffer = m_pTangentsSubMesh->indicesBuffer();

        foreach (CSubMesh* pSubMesh, m_SubMeshs)
        {
            for (int i = 0; i < pSubMesh->positionsBuffer().count(); ++i)
            {
                posBuffer << pSubMesh->positionsBuffer()[i];
                posBuffer << pSubMesh->positionsBuffer()[i] + DefaultNormalsAndTangeantesSize * pSubMesh->tangentsBuffer()[i];
                idBuffer << i * 2 << i * 2 + 1;
            }
        }

        m_pTangentsSubMesh->setMaterial(eRed);

        m_bNeedTangentsSubMeshUpdate = false;
    }
}

//-----------------------------------------------------------------------------------------
void CMesh::createBitangentsSubMesh()
{
    if (m_bNeedBitangentsSubMeshUpdate)
    {
        if (!m_pBitangentsSubMesh)
        {
            m_pBitangentsSubMesh = createSubMesh();
        }
        m_pBitangentsSubMesh->clear();

        CBuffer<QVector3D>& posBuffer = m_pBitangentsSubMesh->positionsBuffer();
        CBuffer<IndiceType>& idBuffer = m_pBitangentsSubMesh->indicesBuffer();

        foreach (CSubMesh* pSubMesh, m_SubMeshs)
        {
            for (int i = 0; i < pSubMesh->positionsBuffer().count(); ++i)
            {
                posBuffer << pSubMesh->positionsBuffer()[i];
                posBuffer << pSubMesh->positionsBuffer()[i] + DefaultNormalsAndTangeantesSize * pSubMesh->bitangentsBuffer()[i];
                idBuffer << i * 2 << i * 2 + 1;
            }
        }

        m_pBitangentsSubMesh->setMaterial(eBlue);

        m_bNeedBitangentsSubMeshUpdate = false;
    }
}

//-----------------------------------------------------------------------------------------
void CMesh::createSkeletonSubMesh()
{
    if (m_bNeedSkeletonSubMeshUpdate)
    {
        m_pSkeletonSubMesh = new CSkeletonSubMesh(this);
        m_SubMeshs.append(m_pSkeletonSubMesh);
        notifyUpdate();

        m_bNeedSkeletonSubMeshUpdate = false;
    }
}
*/

//-----------------------------------------------------------------------------------------
/* TODO : move to Debugger class
 * void CSceneManager::updateSceneGraphMesh()
{
    if (m_bNeedUpdateSceneGraph && m_bSceneGraphVisible)
    {
        if (m_pSceneGraphMesh->meshBuffers().isEmpty())
        {
            m_pSceneGraphMesh->createSubMesh()->setPrimitiveType(ePrimitiveLine);
        }

        CMeshBuffer* pBuffer = m_pSceneGraphMesh->meshBuffers().first();
        pBuffer->clear();

        typedef QPair<QVector3D, QVector3D> TLine;

        struct SNodeTreeTreatement : public CTreeExplorer<CSceneNode>::IExploreWidthListener
        {
            QVector<TLine> lines;

            virtual void onExplore(CSceneNode* pNode)
            {
                if (pNode->getParent())
                {
                    lines << TLine(pNode->getGlobalPosition(), pNode->getParent()->getGlobalPosition());
                }
            }
        };

        SNodeTreeTreatement t;
        CTreeExplorer<CSceneNode> treeExplorer;
        treeExplorer.exploreWidth(&m_RootNode, &t);

        CBuffer<QVector3D>& positionsBuffer = pBuffer->positionsBuffer();
        CBuffer<IndiceType>& indicesBuffer = pBuffer->indicesBuffer();

        int iCurrentIndice = 0;
        foreach (const TLine& line, t.lines)
        {
            positionsBuffer << line.first << line.second;
            indicesBuffer << iCurrentIndice << iCurrentIndice + 1;
            iCurrentIndice+=2;
        }
    }
}
*/
