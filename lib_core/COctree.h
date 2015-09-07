#ifndef COCTREE_H
#define COCTREE_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CBox3D.h"
#include "CSceneNode.h"
#include "COctreeNode.h"
#include "CFrustum.h"
#include "CSceneManager.h"


class LIB_CORE_SHARED_EXPORT COctree : public CSceneManager::ISceneManagerListener
{

public:

    //! Constructeur - Construit un octree
    COctree(CSceneManager* pSceneManager, const CBox3D& worldBbox, unsigned int uiPolygonCountThreshold, unsigned int iMaxDepth);

	//! Destructeur
    virtual ~COctree();

    //! Retourne le noeud racine
    COctreeNode* getRootNode() { return &m_RootNode;}

    //! Ajoute un noeud de scene
    void addSceneNode(CSceneNode* pSceneNode);

    //! Met � jour un noeud de scene
    void updateSceneNode(CSceneNode* pSceneNode);

    //! Supprime un noeud de scene
    void removeSceneNode(CSceneNode* pSceneNode);

    //! Retourne la liste des noeuds visible pour un frustum donn�
    QSet<CSceneNode*> getVisibleSceneNodes(const CFrustum& frustum) const;

protected:

    //! Pr�vient de la mise � jour d'une cam�ra
    virtual void onUpdateCamera(CCamera*) {}

    //! Pr�vient de la mise � jour d'une animation
    virtual void onUpdateAnimation(CAnimation*) {}

    //! Pr�vient de la cr�ation d'un noeud
    virtual void onCreateSceneNode(CSceneNode* pSceneNode);

    //! Pr�vient de la mise � jour d'un noeud
    virtual void onUpdateSceneNode(CSceneNode* pSceneNode);

    //! Pr�vient de la mise � jour d'un noeud
    virtual void onDeleteSceneNode(CSceneNode* pSceneNode);

private:

    void recursiveAddSceneNode(COctreeNode* pOctreeNode, CSceneNode* pSceneNode, unsigned int uiDepth);

    void recursiveFindVisibleSceneNodes(const CFrustum& frustum, const COctreeNode* pNode, QSet<CSceneNode*>& visibleSceneNodes) const;

    //! SceneManager associ�
    CSceneManager* m_pSceneManager;

    //! Noeud racine
    COctreeNode m_RootNode;

    //! Seuil de polygones pour la subdivision
    unsigned int m_uiPolygonCountThreshold;

    //! Seuil de recursivit�
    unsigned int m_uiMaxDepth;

    QMultiHash<CSceneNode*, COctreeNode*> m_SceneNodes;

    mutable int call;
};


#endif // COCTREE_H
