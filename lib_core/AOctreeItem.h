#ifndef AOCTREEITEM_H
#define AOCTREEITEM_H

#include "CBox3D.h"
#include "CRay.h"
#include "COctreeNode.h"
#include "ASceneNodeItem.h"

// Qt
#include <QMutex>

//! Classe abstraite des items pouvant être associé à un noeud du octree
class AOctreeItem
{
    friend class COctreeNode;

public:

	//! Constructeur
    AOctreeItem()
		: m_iLOD(0)
        , m_Mutex(QMutex::Recursive)
        , m_pOctreeNode(0)
    {}

    //! Constructeur
    AOctreeItem(ASceneNodeItem* pSceneNodeItem)
        : m_pSceneNodeItem(pSceneNodeItem)
        , m_iLOD(0)
        , m_Mutex(QMutex::Recursive)
        , m_pOctreeNode(0)
    {}

	//! Retourne un pointeur sur le mutex
	QMutex* getMutex() { return &m_Mutex; }

    //! Retourne l'item associé
    ASceneNodeItem* getItem() { return m_pSceneNodeItem; }

    //! Définit le Level of Detail
	void setLOD(int iLOD) { m_iLOD = iLOD; }

    //! Retourne le Level of Detail
	int getLOD() const { return m_iLOD; }

    //! Retourne le node dans l'octree associé
    COctreeNode* getOctreeNode() const { return m_pOctreeNode; }

private:

    void setOctreeNode(COctreeNode* pOctreeNode) { m_pOctreeNode = pOctreeNode; }

    ASceneNodeItem* m_pSceneNodeItem;
    int m_iLOD;
	QMutex m_Mutex;
    COctreeNode* m_pOctreeNode;
};

#endif //AOCTREEITEM_H
