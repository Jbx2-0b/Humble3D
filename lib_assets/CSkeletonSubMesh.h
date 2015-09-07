#ifndef CSKELETONSUBMESH_H
#define CSKELETONSUBMESH_H

#include "CSubMesh.h"
#include "CSceneNode.h"
#include <QMultiHash>
#include <QPair>


class CSkeletonSubMesh : public CSubMesh //, public CSceneNode::IListener
{
public:

	//! Constructeur
    CSkeletonSubMesh(CMesh* pParentMesh);

	//! Destructeur
	virtual ~CSkeletonSubMesh();

protected:

	//! Impl�mentation de CSceneNode::IListener
	virtual void onUpdate(CSceneNode* pNode);

	//! Impl�mentation de CSceneNode::IListener
	virtual void onDelete(CSceneNode* pNode);

private:
	
	//! Met � jour la g�om�trie
	void updateGeometry();

	typedef QPair<CSceneNode*, CSceneNode*> TArm;
	QList<TArm> m_Arms;
	QMultiHash<CSceneNode*, CBone> m_BoneNodes;
};

#endif // CSKELETONSUBMESH_H
