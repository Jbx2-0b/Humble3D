#include "CSkeletonSubMesh.h"
#include "CMaterialManager.h"
#include "CMesh.h"
#include "CSceneManager.h"


//-----------------------------------------------------------------------------------------
CSkeletonSubMesh::CSkeletonSubMesh(CMesh* pParentMesh)
: CSubMesh(pParentMesh)
{
    //setMaterial(CMaterialManager::getInstance()->getMaterial(eGreen)->getName());
    //setInheritVisibility(false);
    //setVisible(false);
	
    //m_BoneNodes = m_pParentMesh->getBoneNodes();

	QList<CSceneNode*> nodeList = m_BoneNodes.keys();

	foreach (CSceneNode* pNode1, nodeList)
	{	
		bool bLinked = false;

		foreach (CSceneNode* pNode2, nodeList)
		{	
			if (pNode1->isLinked(pNode2))
			{
				m_Arms << TArm(pNode1, pNode2);
				bLinked = true;
			}
		}
    // TODO
//		if (bLinked) pNode1->registerListener(this);
	}

	updateGeometry();
}

//-----------------------------------------------------------------------------------------
CSkeletonSubMesh::~CSkeletonSubMesh()
{
	foreach (const TArm& arm, m_Arms)
	{
        // TODO
        //arm.first->unregisterListener(this);
        //arm.second->unregisterListener(this);
	}
}

//-----------------------------------------------------------------------------------------
void CSkeletonSubMesh::updateGeometry()
{
	clear();

    /*
	if (CSceneNode* pMeshNode = m_pParentMesh->getNode())
	{
		QMatrix4x4 globalInverseMeshTransform = pMeshNode->getGlobalInverseTransformation();

		CBuffer<QVector3D>& posBuffer = positionsBuffer();
		CBuffer<IndiceType>& idBuffer = indicesBuffer();

		int iCurrentIndice = 0;
		foreach (const TArm& arm, m_Arms)
		{
            posBuffer << globalInverseMeshTransform * arm.first->getGlobalTransformation()  * QVector3D();
            posBuffer << globalInverseMeshTransform * arm.second->getGlobalTransformation()  * QVector3D();
			idBuffer << iCurrentIndice << iCurrentIndice + 1;
			iCurrentIndice += 2;
		}
    }*/
}

//-----------------------------------------------------------------------------------------
void CSkeletonSubMesh::onUpdate(CSceneNode*)
{
	updateGeometry();
}

//-----------------------------------------------------------------------------------------
void CSkeletonSubMesh::onDelete(CSceneNode*)
{

}

