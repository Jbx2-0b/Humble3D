#ifndef COCTREENODE_H
#define COCTREENODE_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CBox3D.h"
#include "CNode.h"
#include "CSceneManager.h"

// Qt
#include <QMutex>

class LIB_CORE_SHARED_EXPORT COctreeNode : public CNode<COctreeNode>
{
    friend class CNode<COctreeNode>;

public:

	//! Constructeur
    COctreeNode(const CBox3D& bbox, COctreeNode* pParent = 0);

	//! Destructeur
    virtual ~COctreeNode();

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "OctreeNode"; }

    //! Retourne la bounding box associée
	const CBox3D& getBoundingBox() const;

    //! Crée 8 enfants
    const QList<COctreeNode*>& createChilds();

	//! Retourne un pointeur sur le mutex
	QMutex* getMutex() { return &m_Mutex; }

    //! Retourne le nombre de polygons associes a ce noeud
    unsigned int getPolygonCount() const;

    //! Ajoute un noeud
    void addSceneNode(CSceneNode* pSceneNode);

    //! Supprime un noeud
    void removeSceneNode(CSceneNode* pSceneNode);

    //! Retourne la liste des noeuds associés
    const QSet<CSceneNode*>& getSceneNodes() const;

    //! Retourne la liste des noeuds associés
    QSet<CSceneNode*>& getSceneNodes();

    //! Vide la liste des noeuds
    void clearSceneNodes();

	//! Affiche l'arbre des noeuds sur la console
	void dumpNodeTree() const;

	//! Affiche le noeud et ses enfants sur la sortie standard
	virtual void recursiveDump(int iLevel) const;

protected:

    //! Crée un noeud enfant
    COctreeNode* createChild(const CBox3D& bbox);

    QSet<CSceneNode*> m_SceneNodes;

	CBox3D m_BoundingBox;

	QMutex m_Mutex;
};

#endif // COCTREENODE_H
