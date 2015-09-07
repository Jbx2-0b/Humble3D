#ifndef CQUADTREENODE_H
#define CQUADTREENODE_H

// Foundations
#include "CBox3D.h"
#include "AQuadtreeItem.h"

// Qt
#include <QMutex>

class CQuadtreeNode
{
public:

	//! Constructeur
	CQuadtreeNode(const CBox3D& bbox);

	//! Constructeur
	CQuadtreeNode(CQuadtreeNode* pParent, const CBox3D& bbox);

	//! Destructeur
	virtual ~CQuadtreeNode();

	// D�finit le noeud parent
	void setParent(CQuadtreeNode* pParent);

	//! Retourne le neoud parent
	CQuadtreeNode* getParent() const;

	//! Retourne la liste des enfants
	inline const QList<CQuadtreeNode*>& getChildren() const;

	//! Retourne la liste des enfants
	QList<CQuadtreeNode*>& getChildren();

	//! D�finit la bounding box associ�es
	void setBoudingBox(const CBox3D& bbox);

	//! Retourne la bounding box associ�es
	const CBox3D& getBoundingBox() const;

	//! Retourne vrai si les deux noeuds sont li�s
	bool isLinked(CQuadtreeNode* pNode);

	//! Retourne la profondeur du noeud
	unsigned int getDepth() const;

	//! Retourne un pointeur sur le mutex
	QMutex* getMutex() { return &m_Mutex; }

	//! Cr�e un noeud enfant
	CQuadtreeNode* createChild(const CBox3D& bbox);

	//! D�tache un noeud enfant. Ne d�truit pas le noeud
	void removeChild(CQuadtreeNode* pNode);

	//! Ajoute une carte
	void addItem(AQuadtreeItem* pQuadtreeItem);

	//! Retourne la liste des items associ�s
	inline const QList<AQuadtreeItem*>& getItems() const { return m_Items; }

	//! Retourne la liste des items associ�s
	inline QList<AQuadtreeItem*>& getItems() { return m_Items; }

	//! Vide la liste des items associ�es
	void clearItems();

	//! Affiche l'arbre des noeuds sur la console
	void dumpNodeTree() const;

	//! Affiche le noeud et ses enfants sur la sortie standard
	virtual void recursiveDump(int iLevel) const;

protected:

	CQuadtreeNode* m_pParent;

	QList<CQuadtreeNode*> m_Children;

	QList<AQuadtreeItem*> m_Items;

	CBox3D m_Bbox;

	QMutex m_Mutex;
};

#endif // CQUADTREENODE_H
