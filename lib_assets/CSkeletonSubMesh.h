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

	//! Implémentation de CSceneNode::IListener
	virtual void onUpdate(CSceneNode* pNode);

	//! Implémentation de CSceneNode::IListener
	virtual void onDelete(CSceneNode* pNode);

private:
	
	//! Met à jour la géométrie
	void updateGeometry();

	typedef QPair<CSceneNode*, CSceneNode*> TArm;
	QList<TArm> m_Arms;
	QMultiHash<CSceneNode*, CBone> m_BoneNodes;
};

#endif // CSKELETONSUBMESH_H
