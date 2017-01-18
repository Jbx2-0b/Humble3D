#include "CMeshInstance.h"
#include "CSubMeshInstance.h"
#include "CSceneManager.h"
#include "CMesh.h"

//-----------------------------------------------------------------------------------------
CMeshInstance::CMeshInstance(CSceneManager* pSceneManager, CMesh* pMesh, const QString& name)
    : ASceneNodeItem(pSceneManager, name)
    , m_pMesh(pMesh)
    , m_pLocalRootNode(0)
{
    m_pMesh->lock();
    for (CSubMesh* pSubMesh : m_pMesh->subMeshs())
    {
        createSubMeshInstance(pSubMesh);
    }

    m_pMesh->registerListener(this);
    m_pMesh->unlock();
}

//-----------------------------------------------------------------------------------------
CMeshInstance::~CMeshInstance(void)
{
    m_pMesh->unregisterListener(this);

    for (CSubMeshInstance* pSubMeshInstance : m_SubMeshInstanceMap.values())
    {
        delete pSubMeshInstance;
    }
}

//--------------------------------------------------------------------------
void CMeshInstance::registerListener(IMeshInstanceListener* pListener)
{
    m_MeshInstanceListeners.insert(pListener);
}

//--------------------------------------------------------------------------
void CMeshInstance::unregisterListener(IMeshInstanceListener* pListener)
{
    m_MeshInstanceListeners.remove(pListener);
}

//--------------------------------------------------------------------------
void CMeshInstance::notifyUpdate()
{
    if (m_bNotificationsEnabled)
    {
        for (IMeshInstanceListener* pListener : m_MeshInstanceListeners)
        {
            pListener->onUpdate(this);
        }

        ASceneNodeItem::notifyUpdate();
    }
}

//-----------------------------------------------------------------------------------------
void CMeshInstance::setMaterialName(const QString& materialName)
{
    m_MaterialName = materialName;

    for (CSubMeshInstance* pSubMeshInstance : m_SubMeshInstanceMap)
    {
        pSubMeshInstance->setMaterialName(materialName);
    }
}

//-----------------------------------------------------------------------------------------
void CMeshInstance::setRenderStates(const CRenderStates& renderStates)
{
    for (CSubMeshInstance* pSubMeshInstance : m_SubMeshInstanceMap)
    {
        pSubMeshInstance->setRenderStates(renderStates);
    }
}

//-----------------------------------------------------------------------------------------
// Selection
//-----------------------------------------------------------------------------------------
CSubMeshInstance* CMeshInstance::createSubMeshInstance(CSubMesh* pSubMesh)
{
    CSubMeshInstance* pSubMeshInstance = new CSubMeshInstance(m_pSceneManager, this, pSubMesh);
    m_SubMeshInstanceMap[pSubMesh] = pSubMeshInstance;
    if (!m_MaterialName.isEmpty()) pSubMeshInstance->setMaterialName(m_MaterialName);
    notifyUpdate();
    return pSubMeshInstance;
}

//-----------------------------------------------------------------------------------------
void CMeshInstance::removeSubMeshInstance(CSubMesh* pSubMesh)
{
    delete m_SubMeshInstanceMap[pSubMesh];
    m_SubMeshInstanceMap.remove(pSubMesh);
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CMeshInstance::onUpdate(CMesh* pMesh)
{
    for (CSubMesh* pSubMesh : m_SubMeshInstanceMap.keys())
    {
        if (!pMesh->subMeshs().contains(pSubMesh))
        {
            removeSubMeshInstance(pSubMesh);
        }
    }

    for (CSubMesh* pSubMesh : pMesh->subMeshs())
    {
        if (!m_SubMeshInstanceMap.contains(pSubMesh))
        {
            createSubMeshInstance(pSubMesh);
        }
    }

    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CMeshInstance::onDelete(CMesh*)
{
    // Nothing to do, MeshInstance must be destroy before his Mesh
}
