#include "CSubMeshInstance.h"
#include "CMeshInstance.h"
#include "CSceneManager.h"
#include "CSubMesh.h"

//-----------------------------------------------------------------------------------------
CSubMeshInstance::CSubMeshInstance(CSceneManager* pSceneManager, CMeshInstance* pParent, CSubMesh* pSubMesh)
    : ARenderableItem(pSceneManager, pParent->getName() + "_Sub")
    , m_pParentInstance(pParent)
    , m_pSubMesh(pSubMesh)
    , m_bInheritVisibility(true)
{
    setMaterialName(pSubMesh->getMaterialName());
    setSelectable(false);
    m_pSubMesh->registerListener(this);
}

//-----------------------------------------------------------------------------------------
CSubMeshInstance::~CSubMeshInstance()
{
    m_pSubMesh->unregisterListener(this);
}

//-----------------------------------------------------------------------------------------
const QSet<CSceneNode*>& CSubMeshInstance::getNodes() const
{
    return m_pParentInstance->getNodes();
}

//-----------------------------------------------------------------------------------------
bool CSubMeshInstance::isVisible() const
{
    return m_bVisible && (!m_bInheritVisibility || (m_bInheritVisibility && m_pParentInstance->isVisible()));
}

//-----------------------------------------------------------------------------------------
void CSubMeshInstance::onMaterialChanged(CSubMesh* pSubMesh)
{
    setMaterialName(pSubMesh->getMaterialName());
    m_pParentInstance->notifyUpdate();
}

//-----------------------------------------------------------------------------------------
QVector<QMatrix4x4> CSubMeshInstance::getBoneTransformations() const
{
    QVector<QMatrix4x4> transforms;
    transforms.resize(MaxBoneTransformations);

    if (CSceneNode* pLocalRootNode = m_pParentInstance->getLocalRootNode())
    {
        int iCurrentID = 0;
        // TODO : Pour des raisons de performance, enregistrer un tableau associatif CSceneNode[bone.getNodeName()]
        // Après chargement du mesh
        foreach (const CBone& bone, m_pSubMesh->bonesBuffer())
        {
            if (CSceneNode* pNode = m_pSceneManager->findNode(bone.getNodeName()))
            {
                transforms[iCurrentID++] = pLocalRootNode->getGlobalInverseTransformation() * pNode->getGlobalTransformation() * bone.getOffsetMatrix();
            }
        }
    }

    return transforms;
}
