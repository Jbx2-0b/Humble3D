#include "CSceneManager.h"
#include "CLogManager.h"
#include "CRay.h"
#include "CTriangle.h"
#include "CMaterialManager.h"
#include "CShaderManager.h"
#include "CMeshManager.h"
#include "COctree.h"


//-----------------------------------------------------------------------------------------
CSceneManager::CSceneManager()
    : AEntity("SceneManager")
    , m_pOctree(0)
    , m_AmbientColor(DefaultSceneAmbientColor)
    , m_RootNode(this, "Root")
{
    CMeshManager::getInstance().registerListener(this);
}

//-----------------------------------------------------------------------------------------
CSceneManager::~CSceneManager()
{
    delete m_pOctree;
    clearAll();
    CMeshManager::getInstance().unregisterListener(this);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::buildOctree(const CBox3D& worldBoundingBox, unsigned int uiPolygonCountThreshold, unsigned int iMaxDepth)
{
    if (m_pOctree)
    {
        LogManager.addMessage(eINFO, "Deleting previous octree...");
        delete m_pOctree;
    }

    LogManager.addMessage(eINFO, QString("Creating octree... World bounding box: %1 Max Depth %2").arg(worldBoundingBox.toString()).arg(iMaxDepth));
    m_pOctree = new COctree(this, worldBoundingBox, uiPolygonCountThreshold, iMaxDepth);
    LogManager.addMessage(eINFO, "Creating octree... Done.");
}

//-----------------------------------------------------------------------------------------
// Listeners
//-----------------------------------------------------------------------------------------
void CSceneManager::registerAnimationListener(IAnimationListener* pListener)
{
    m_AnimationListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::unregisterAnimationListener(IAnimationListener* pListener)
{
    m_AnimationListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::registerListener(ISceneManagerListener* pListener)
{
    m_SceneManagerListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::unregisterListener(ISceneManagerListener* pListener)
{
    m_SceneManagerListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
// Suppressions
//-----------------------------------------------------------------------------------------
void CSceneManager::clearAll()
{
    clearAnimations();
    clearMeshInstances();
    clearLights();
    clearCameras();
    clearBillboards();
    clearNodeItems();
    clearNodes();
}

//-----------------------------------------------------------------------------------------
void CSceneManager::clearMeshInstances()
{
    for (CMeshInstance* pMeshInstance : m_MeshInstances)
    {
        m_NodeItemByID.remove(pMeshInstance->getID());

        pMeshInstance->unregisterListener(this);

        for (CSubMeshInstance* pSubMeshInstance : pMeshInstance->subMeshInstances())
        {
            removeRenderableItem(pSubMeshInstance);
        }
        delete pMeshInstance;
    }

    m_MeshInstances.clear();
}

//-----------------------------------------------------------------------------------------
void CSceneManager::clearLights()
{
    for (CLight* pLight : m_Lights)
    {
        m_NodeItemByID.remove(pLight->getID());
        delete pLight;
    }
    m_Lights.clear();
}

//-----------------------------------------------------------------------------------------
void CSceneManager::clearCameras()
{
    for (CCamera* pCamera : m_Cameras)
    {
        m_NodeItemByID.remove(pCamera->getID());
        pCamera->unregisterListener(this);
        delete pCamera;
    }
    m_Cameras.clear();
}

//-----------------------------------------------------------------------------------------
void CSceneManager::clearAnimations()
{
    for (CAnimation* pAnimation : m_AnimationByID)
    {
        pAnimation->unregisterListener(this);
        delete pAnimation;
    }
    m_AnimationByID.clear();
}

//-----------------------------------------------------------------------------------------
void CSceneManager::clearBillboards()
{
    for (CBillboard* pBillboard : m_Billboards)
    {

        m_NodeItemByID.remove(pBillboard->getID());
        removeRenderableItem(pBillboard);
        delete pBillboard;
    }
    m_Billboards.clear();
}

//-----------------------------------------------------------------------------------------
void CSceneManager::clearNodeItems()
{
    qDeleteAll(m_NodeItemByID);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::clearNodes()
{
    QList<CSceneNode*> nodeList = m_RootNode.getChildNodes();

    m_RootNode.clearChildNodes();

    for (CSceneNode* pNode : nodeList)
    {
        delete pNode;
    }

    m_RootNode.resetTransformation();
}

//-----------------------------------------------------------------------------------------
void CSceneManager::clearEmptyNodes()
{
    recursiveClearEmptyNodes(&m_RootNode);
}

//-----------------------------------------------------------------------------------------
bool CSceneManager::recursiveClearEmptyNodes(CSceneNode* pNode)
{
    for (CSceneNode* pChildNode : pNode->getChildNodes())
    {
        if (recursiveClearEmptyNodes(pChildNode))
        {
            pNode->removeChild(pChildNode);
            delete pChildNode;
        }
    }

    return pNode->getItems().isEmpty() && pNode->getChildNodes().isEmpty();
}

//-----------------------------------------------------------------------------------------
void CSceneManager::addSceneNode(CSceneNode* pSceneNode)
{
    m_SceneNodes.insert(pSceneNode);

    for (ISceneManagerListener* pListener : m_SceneManagerListeners)
    {
        pListener->onCreateSceneNode(pSceneNode);
    }
    pSceneNode->registerListener(this);
}

//-----------------------------------------------------------------------------------------
int CSceneManager::getPolygonCount() const
{
    int iCount = 0;

    for (CMeshInstance* pMeshInstance : m_MeshInstances)
    {
        iCount += pMeshInstance->getMesh()->getPolygonCount();
    }

    return iCount;
}

//-----------------------------------------------------------------------------------------
QList<QString> CSceneManager::getItemNames() const
{
    QList<QString> names;
    for (ASceneNodeItem* pItem : m_NodeItemByID.values())
    {
        names.append(pItem->getName());
    }

    return names;
}

//-----------------------------------------------------------------------------------------
CAnimation* CSceneManager::getAnimationByName(const QString& name) const
{
    for (CAnimation* pAnimation : m_AnimationByID.values())
    {
        if (pAnimation->getName() == name)
            return pAnimation;
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
// Creations d'items
//-----------------------------------------------------------------------------------------
CMeshInstance* CSceneManager::createMeshInstance(const QString& meshName, const QString& name /*= "MeshInstance"*/)
{
    return createMeshInstance(CMeshManager::getInstance().getMeshByName(meshName), name);
}

//-----------------------------------------------------------------------------------------
CMeshInstance* CSceneManager::createMeshInstance(CMesh* pMesh, const QString& name /*= "MeshInstance"*/)
{
    if (pMesh)
    {
        CMeshInstance* pMeshInstance = new CMeshInstance(this, pMesh, name);
        appendMeshInstance(pMeshInstance, name);
        return pMeshInstance;
    }

    LogManager.addMessage(eERROR, "CSceneManager::createMeshInstance() : CMesh is null ptr !");
    return 0;
}

//-----------------------------------------------------------------------------------------
CLight* CSceneManager::createLight(const QString& name)
{
    CLight* pLight = new CLight(this, name);

    m_Lights.append(pLight);
    m_NodeItemByID[pLight->getID()] = pLight;

    return pLight;
}

//-----------------------------------------------------------------------------------------
CCamera* CSceneManager::createCamera(const QString& name)
{
    CCamera* pCamera = new CCamera(this, name);

    m_Cameras.append(pCamera);
    m_NodeItemByID[pCamera->getID()] = pCamera;

    pCamera->registerListener(this);

    return pCamera;
}

//-----------------------------------------------------------------------------------------
CAnimation* CSceneManager::createAnimation(const QString& name)
{
    CAnimation* pAnimation = new CAnimation(name);

    m_AnimationByID[pAnimation->getID()] = pAnimation;

    pAnimation->registerListener(this);
    return pAnimation;
}

//-----------------------------------------------------------------------------------------
CBillboard* CSceneManager::createBillboard(const QString& name)
{
    CBillboard* pBillboard = new CBillboard(this, name);

    m_Billboards.append(pBillboard);
    m_NodeItemByID[pBillboard->getID()] = pBillboard;
    appendRenderableItem(pBillboard);

    return pBillboard;
}

//-----------------------------------------------------------------------------------------
void CSceneManager::appendMeshInstance(CMeshInstance* pMeshInstance, const QString& itemName /*= ""*/)
{
    if (!itemName.isEmpty())
    {
        pMeshInstance->setName(itemName);
    }

    m_MeshInstances.append(pMeshInstance);
    m_NodeItemByID[pMeshInstance->getID()] = pMeshInstance;

    for (CSubMeshInstance* pSubMeshInstance : pMeshInstance->subMeshInstances())
    {
        appendRenderableItem(pSubMeshInstance);
    }

    pMeshInstance->registerListener(this);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::appendRenderableItem(ARenderableItem* pRenderableItem)
{
    if (!m_RenderableItems.contains(pRenderableItem))
    {
        m_RenderableItems.insert(pRenderableItem);
        //pRenderableItem->setSceneManager(this);
        addToRenderQueue(pRenderableItem);
        pRenderableItem->registerListener(this);
    }
}

//-----------------------------------------------------------------------------------------
void CSceneManager::deleteMeshInstance(CMeshInstance* pMeshInstance)
{
    m_MeshInstances.removeOne(pMeshInstance);
    m_NodeItemByID.remove(pMeshInstance->getID());

    pMeshInstance->unregisterListener(this);

    for (CSubMeshInstance* pSubMeshInstance : pMeshInstance->subMeshInstances())
    {
        removeRenderableItem(pSubMeshInstance);
    }

    delete pMeshInstance;
}

//-----------------------------------------------------------------------------------------
void CSceneManager::removeRenderableItem(ARenderableItem* pRenderableItem)
{
    m_RenderableItems.remove(pRenderableItem);
    removeFromRenderQueue(pRenderableItem);
    pRenderableItem->unregisterListener(this);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::addToRenderQueue(ARenderableItem* pRenderableItem)
{
    if (CMaterial* pMaterial = CMaterialManager::getInstance().getMaterialByName(pRenderableItem->getMaterialName()))
    {
        if (pMaterial->getRenderPass(0)->renderStates().blending().isEnabled())
        {
            m_TransparentRenderQueue.insert(pMaterial->getID(), pRenderableItem);
        }
        else
        {
            m_RenderQueue.insert(pMaterial->getID(), pRenderableItem);
        }
    }
}

//-----------------------------------------------------------------------------------------
void CSceneManager::removeFromRenderQueue(ARenderableItem* pRenderableItem)
{
    m_TransparentRenderQueue.remove(pRenderableItem);
    m_RenderQueue.remove(pRenderableItem);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::deleteLight(CLight* pLight)
{
    m_Lights.removeOne(pLight);
    m_NodeItemByID.remove(pLight->getID());
    delete pLight;
}

//-----------------------------------------------------------------------------------------
void CSceneManager::deleteCamera(CCamera* pCamera)
{
    m_Cameras.removeOne(pCamera);
    m_NodeItemByID.remove(pCamera->getID());
    pCamera->unregisterListener(this);
    delete pCamera;
}

//-----------------------------------------------------------------------------------------
void CSceneManager::deleteAnimation(CAnimation* pAnimation)
{
    m_AnimationByID.remove(pAnimation->getID());
    pAnimation->unregisterListener(this);
    delete pAnimation;
}

//-----------------------------------------------------------------------------------------
void CSceneManager::deleteBillboards(CBillboard* pBillboard)
{
    m_Billboards.removeOne(pBillboard);
    m_NodeItemByID.remove(pBillboard->getID());
    removeRenderableItem(pBillboard);
    delete pBillboard;
}


//-----------------------------------------------------------------------------------------
CBox3D CSceneManager::getWorldAxisAlignedBoundingBox() const
{
    return m_RootNode.getGlobalAxisAlignedBoundingBox();
}

//-----------------------------------------------------------------------------------------
void CSceneManager::setAmbientColor(const QVector4D& ambientColor)
{
    m_AmbientColor = ambientColor;
}

//-----------------------------------------------------------------------------------------
QVector4D CSceneManager::getAmbientColor() const
{
    return m_AmbientColor;
}

//-----------------------------------------------------------------------------------------
void CSceneManager::onMaterialChanged(ARenderableItem* pRenderableItem)
{
    removeFromRenderQueue(pRenderableItem);
    addToRenderQueue(pRenderableItem);
}

//-----------------------------------------------------------------------------------------
ASceneNodeItem* CSceneManager::pick(CCamera* pCamera, const QPoint& position, const QSize& viewSize)
{
    QVector3D vPick = pCamera->mapPointToWorldCoordinates(position, viewSize);
    QVector3D eyePosition = pCamera->getEyePosition();
    CRay eyeLine(eyePosition, (vPick - eyePosition).normalized());

    real dDistance = std::numeric_limits<real>::infinity();

    ASceneNodeItem* pSelectedItem = recursiveFindIntersection(eyeLine, &m_RootNode, dDistance);

    if (pSelectedItem)
    {
        pSelectedItem->setSelected(true);
    }

    for (ASceneNodeItem* pOtherItem : m_NodeItemByID.values())
    {
        if (pOtherItem != pSelectedItem)
        {
            pOtherItem->setSelected(false);
        }
    }

    return pSelectedItem;
}

//-----------------------------------------------------------------------------------------
ASceneNodeItem* CSceneManager::findItemByName(const QString& name)
{
    for (ASceneNodeItem* pItem : m_NodeItemByID.values())
    {
        if (pItem->getName() == name)
            return pItem;
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CSceneNode* CSceneManager::findNode(const QString& name)
{
    return m_RootNode.findNode(name);
}

//-----------------------------------------------------------------------------------------
CSceneNode* CSceneManager::findNode(int iNodeID)
{
    return m_RootNode.findNode(iNodeID);
}

//-----------------------------------------------------------------------------------------
// CAnimation::IAnimationListener
//-----------------------------------------------------------------------------------------
void CSceneManager::onStart(CAnimation* pAnimation)
{
    notifyStart(pAnimation);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::onPause(CAnimation* pAnimation)
{
    notifyPause(pAnimation);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::onStop(CAnimation* pAnimation)
{
    notifyStop(pAnimation);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::notifyStart(CAnimation* pAnimation)
{
    for (IAnimationListener* pListener : m_AnimationListeners)
    {
        pListener->onStartAnimation(pAnimation);
    }

    for (ISceneManagerListener* pListener : m_SceneManagerListeners)
    {
        pListener->onUpdateAnimation(pAnimation);
    }
}

//-----------------------------------------------------------------------------------------
void CSceneManager::notifyPause(CAnimation* pAnimation)
{
    for (IAnimationListener* pListener : m_AnimationListeners)
    {
        pListener->onPauseAnimation(pAnimation);
    }

    for (ISceneManagerListener* pListener : m_SceneManagerListeners)
    {
        pListener->onUpdateAnimation(pAnimation);
    }
}

//-----------------------------------------------------------------------------------------
void CSceneManager::notifyStop(CAnimation* pAnimation)
{
    for (IAnimationListener* pListener : m_AnimationListeners)
    {
        pListener->onStopAnimation(pAnimation);
    }

    for (ISceneManagerListener* pListener : m_SceneManagerListeners)
    {
        pListener->onUpdateAnimation(pAnimation);
    }
}


//-----------------------------------------------------------------------------------------
// CMesh::IMeshListener
//-----------------------------------------------------------------------------------------
void CSceneManager::onUpdate(CMesh*)
{
}

//-----------------------------------------------------------------------------------------
void CSceneManager::onDelete(CMesh* pMesh)
{
    for (CMeshInstance* pMeshInstance : m_MeshInstances)
    {
        if (pMeshInstance->getMesh() == pMesh)
        {
            deleteMeshInstance(pMeshInstance);
        }
    }
}

//-----------------------------------------------------------------------------------------
void CSceneManager::onUpdate(CSceneNode* pSceneNode)
{
    for (ISceneManagerListener* pListener : m_SceneManagerListeners)
    {
        pListener->onUpdateSceneNode(pSceneNode);
    }
}

//-----------------------------------------------------------------------------------------
void CSceneManager::onDelete(CSceneNode* pSceneNode)
{
    pSceneNode->unregisterListener(this);

    for (ISceneManagerListener* pListener : m_SceneManagerListeners)
    {
        pListener->onDeleteSceneNode(pSceneNode);
    }

    m_SceneNodes.remove(pSceneNode);
}

//-----------------------------------------------------------------------------------------
const QSet<CMaterial*> CSceneManager::getUsedMaterials() const
{
    QSet<CMaterial*> materials;
    for (CMeshInstance* pMeshInstance : m_MeshInstances)
    {
        if (CMaterial* pMaterial = CMaterialManager::getInstance().getMaterialByName(pMeshInstance->getMaterialName()))
        {
            materials.insert(pMaterial);
        }

        for (CSubMeshInstance* pSubMeshInstance : pMeshInstance->subMeshInstances())
        {
            if (CMaterial* pMaterial = CMaterialManager::getInstance().getMaterialByName(pSubMeshInstance->getMaterialName()))
            {
                materials.insert(pMaterial);
            }
        }
    }
    return materials;
}

//-----------------------------------------------------------------------------------------
const QSet<CShader*> CSceneManager::getUsedShaders() const
{
    QSet<CShader*> shaders;
    for (CMaterial* pMaterial : getUsedMaterials())
    {
        for (CRenderPass* pRenderPass : pMaterial->getRenderPasses())
        {
            if (CShader* pShader = CShaderManager::getInstance().getShaderByName(pRenderPass->getShaderName()))
            {
                shaders.insert(pShader);
            }
        }
    }
    return shaders;
}

//-----------------------------------------------------------------------------------------
const QSet<ATexture*> CSceneManager::getUsedTextures() const
{
    QSet<ATexture*> textures;
    for (CMaterial* pMaterial : getUsedMaterials())
    {
        for (const CTextureParam& textureParam : pMaterial->getTextureParams())
        {
            if (ATexture* pTexture = CTextureManager::getInstance().getTextureByName(textureParam.getTextureName()))
            {
                textures.insert(pTexture);
            }
        }
    }
    return textures;
}

//-----------------------------------------------------------------------------------------
QSet<CSceneNode*> CSceneManager::getVisibleSceneNodes(const CFrustum& frustum)
{
    QSet<CSceneNode*> visibleItems;

    if (m_pOctree)
    {
        visibleItems = m_pOctree->getVisibleSceneNodes(frustum);
    }
    else
    {
        for (CSceneNode* pSceneNode : m_SceneNodes)
        {
            const CBox3D& sceneNodeBBox = pSceneNode->getLocalAxisAlignedBoundingBox();
            if (!sceneNodeBBox.isNull())
            {
                if (frustum.isSphereInFrustum(CSphere(sceneNodeBBox)) != eOutside)
                {
                    if (frustum.isBoxInFrustum(sceneNodeBBox) != eOutside)
                    {
                        visibleItems.insert(pSceneNode);
                    }
                }
            }
        }
    }

    return visibleItems;
}

//-----------------------------------------------------------------------------------------
// CMeshInstance::IMeshInstanceListener
//-----------------------------------------------------------------------------------------
void CSceneManager::onUpdate(CMeshInstance* pMeshInstance)
{
    for (CSubMeshInstance* pSubMeshInstance : pMeshInstance->subMeshInstances())
    {
        appendRenderableItem(pSubMeshInstance);
    }
}

//-----------------------------------------------------------------------------------------
// CCamera::ICameraListener
//-----------------------------------------------------------------------------------------
void CSceneManager::onUpdate(CCamera* pCamera)
{
    notifyUpdate(pCamera);
}

//-----------------------------------------------------------------------------------------
void CSceneManager::notifyUpdate(CCamera* pCamera)
{
    for (ISceneManagerListener* pListener : m_SceneManagerListeners)
    {
        pListener->onUpdateCamera(pCamera);
    }
}

//-----------------------------------------------------------------------------------------
// Picking
//-----------------------------------------------------------------------------------------
ASceneNodeItem* CSceneManager::recursiveFindIntersection(const CRay& ray, CSceneNode* pNode, real& dDistance)
{
    ASceneNodeItem* pSelectedItem = 0;

    // Pour chaque NodeItem attaché au node
    for (ASceneNodeItem* pItem : pNode->getItems())
    {
        if (pItem->isSelectable())
        {
            CBox3D bbox = pItem->getBoundingBox();

            CBox3D transfBBox = bbox.transformed(pNode->getGlobalTransformation());

            if (transfBBox.intersects(ray))
            {
                real dD = std::numeric_limits<real>::infinity();

                if (pItem->intersection(ray, &dD, pNode->getGlobalTransformation()))
                {
                    if (dD < dDistance)
                    {
                        dDistance = dD;
                        pSelectedItem = pItem;
                    }
                }
            }
        }
    }
    // On effectue la recherche de maniére récursive pour les enfants
    for (CSceneNode* pChildNode : pNode->getChildNodes())
    {
        if (ASceneNodeItem* pItem = recursiveFindIntersection(ray, pChildNode, dDistance))
        {
            pSelectedItem = pItem;
        }
    }

    return pSelectedItem;
}

