#include "CSubMesh.h"
#include "CMesh.h"
#include "CMaterialManager.h"
#include "ARenderer.h"

//-----------------------------------------------------------------------------------------
CSubMesh::CSubMesh(CMesh* pParentMesh)
: m_pParentMesh(pParentMesh)
{
}

//-----------------------------------------------------------------------------------------
CSubMesh::~CSubMesh()
{
}

//-----------------------------------------------------------------------------------------
void CSubMesh::render(ARenderer* pRenderer)
{
    pRenderer->renderMeshBuffer(this);
}

//-----------------------------------------------------------------------------------------
void CSubMesh::setMaterialName(const QString& materialName)
{
    m_MaterialName = materialName;
    notifyMaterialChanged();
}

//-----------------------------------------------------------------------------------------
bool CSubMesh::hasMaterial() const
{
    return CMaterialManager::getInstance()->isMaterialExist(m_MaterialName);
}

