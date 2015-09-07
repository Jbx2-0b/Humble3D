#ifndef CSCENENODE_H
#define CSCENENODE_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMath.h"
#include "CNode.h"
#include "CBox3D.h"
#include "CStringHelper.h"
#include "ASceneNodeItem.h"

// Qt
#include <QList>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>


class CSceneNode;
class CSceneManager;

class LIB_CORE_SHARED_EXPORT CSceneNode : public CNode<CSceneNode>, public ASceneNodeItem::ISceneNodeItemListener
{
    friend class CNode<CSceneNode>;

public:

	//! Ctor
    CSceneNode(CSceneManager* pSceneManager, CSceneNode* pParent = 0);

	//! Ctor
    CSceneNode(CSceneManager* pSceneManager, const QString& name, CSceneNode* pParent = 0);

	//! Dtor
	virtual ~CSceneNode();

	//! Retourne un clone du noeud
	CSceneNode* clone() const;


	//-------------------------------------------------------------------------------------------------
	// Setters
	//-------------------------------------------------------------------------------------------------

	//! Définit la matric de transformation appliquée au noeud
	void setLocalTransformation(const QMatrix4x4& mTransformation);

	//-------------------------------------------------------------------------------------------------
	// Getters
	//-------------------------------------------------------------------------------------------------

	//! Retourne le nom du type
	virtual QString getTypeName() const	{ return "SceneNode"; }

	//! Retourne les items associés au noeud
    QList<ASceneNodeItem*>& getItems() { return m_NodeItems; }

	//! Retourne les items associés au noeud
    const QList<ASceneNodeItem*>& getItems() const { return m_NodeItems; }

    //! Retourne vrai si a des items attachés
    bool hasItems() const { return !m_NodeItems.isEmpty(); }

    //! Retourne les items associés au noeud et à l'ensemble de ses enfants
    QList<ASceneNodeItem*> getAllChildItems() const;

	//! Retourne la matrice de transformation locale
	const QMatrix4x4& getLocalTransformation() const;

	//! Retourne la matrice de transformation inverse locale
	const QMatrix4x4& getLocalInverseTransformation() const;

	//! Retourne la matrice de transformation globale
	const QMatrix4x4& getGlobalTransformation() const;

	//! Retourne la matrice de transformation inverse globale
	const QMatrix4x4& getGlobalInverseTransformation() const;

    //! Retourne la taille locale
    QVector3D getLocalScaling() const;

	//! Retourne l'emplacement du noeud
	QVector3D getGlobalPosition() const;

    //! Retourne la boîte englobante des items associés à ce noeud
    const CBox3D& getLocalAxisAlignedBoundingBox(bool bTransformed = true) const;

	//! Retourne la boite englobante des items associés et ceux des noeuds enfants (union des AABB)
    const CBox3D& getGlobalAxisAlignedBoundingBox() const;

    //! Retourne la masse locale des items du noeud
    real getLocalMass() const;

    //! Retourne la masse globale des items du noeud
    real getGlobalMass() const;

    //! Retourne le nombre de polygones attachés au noeud
    unsigned int getLocalPolygonCount() const;

    //! Retourne le nombre de polygones attachés au noeud et à ses enfants
    unsigned int getGlobalPolygonCount() const;

	//-------------------------------------------------------------------------------------------------
	// Members functions
	//-------------------------------------------------------------------------------------------------

	//! Ajoute un item associé au noeud
    void addItem(ASceneNodeItem* pNodeItem);

	//! Ajoute plusieurs items associés au noeud
    void addItems(const QList<ASceneNodeItem*>& nodeItems);

	//! Ajoute un item associé au noeud
    void removeItem(ASceneNodeItem* pNodeItem);

	//! Crée un noeud enfant et retourne son pointeur
	virtual CSceneNode* createChild(const QVector3D& vTranslation = QVector3D(), const QQuaternion& qRotation = QQuaternion());

	//! Crée un noeud enfant et retourne son pointeur
	virtual CSceneNode* createChild(const QMatrix4x4& transformation);

	//! Crée un noeud enfant et retourne son pointeur
	virtual CSceneNode* createChild(const QString& nodeName, const QVector3D& vTranslation = QVector3D(), const QQuaternion& qRotation = QQuaternion());

	//! Crée un noeud enfant et retourne son pointeur
	virtual CSceneNode* createChild(const QString& nodeName, const QMatrix4x4& transformation);

	//! Réinitialise la matrice de transformation
	void resetTransformation();

	//! Définit la translation
	void translate(const QVector3D &vTranslate);

	//! Définit la translation
	void translate(real dX, real dY, real dZ);

	//! Définit l'échelle
	void scale(const QVector3D &vScale);

	//! Définit l'échelle
	void scale(real dX, real dY, real dZ);

	//! Définit l'échelle
	void scale(real dS);

	//! Fait tourner l'objet autour d'un axe particulier
	virtual void rotate(const QVector3D &vAxis, real dAngle);

	//! Fait tourner l'objet
	virtual void rotate(const QQuaternion &qRotation);

	//! Fait tourner l'objet autour de l'axe Z
	virtual void roll(real dAngle);

	//! Fait tourner l'objet autour de l'axe Y 
	virtual void yaw(real dAngle);

	//! Fait tourner l'objet autour de l'axe X
	virtual void pitch(real dAngle);

	//! Affiche l'arbre des noeuds sur la console
	void dumpNodeTree() const;

	//! Crée une string contenant les propriétés de l'objet
    virtual QString toString() const
	{
		QString result;
		result += "[SceneNode \n";
		result += AEntity::toString();
        if (m_pParent) result += QString("Parent Name: %1\n").arg(m_pParent->getName());
        result += QString("Childs Count: %1\n").arg(m_ChildNodes.size());
        result += QString("Items Count: %1\n").arg(m_NodeItems.size());
        result += QString("Local Mass: %1\n").arg(getLocalMass());
//        result += QString("Local Transformation: %1\n").arg(StringHelper::toQString(getLocalTransformation()));
//        result += QString("Local Inverse Transformation: %1\n").arg(StringHelper::toQString(getLocalInverseTransformation()));
//        result += QString("Global Transformation: %1\n").arg(StringHelper::toQString(getGlobalTransformation()));
//        result += QString("Global Inverse Transformation: %1\n").arg(StringHelper::toQString(getGlobalInverseTransformation()));
        result += QString("Local Axis Aligned Bounding Box: %1\n").arg(getLocalAxisAlignedBoundingBox().toString());
        result += QString("Global Axis Aligned Bounding Box: %1\n").arg(getGlobalAxisAlignedBoundingBox().toString());
		result += "]\n";
		return result;
	}

protected:

    void needUpdateTransformationMatrix()
	{
		m_bNeedUpdateGlobalTransformation = true;
        m_bNeedUpdateGlobalBoundingBox = true;
        if (m_pParent) m_pParent->needUpdateBoundingBox();

        foreach (CSceneNode* pChild, m_ChildNodes)
		{
            pChild->needUpdateTransformationMatrix();
		}
        needUpdateBoundingBox();
	}

    void needUpdateBoundingBox()
    {
        m_bNeedUpdateLocalBoundingBox = true;
        m_bNeedUpdateGlobalBoundingBox = true;
        if (m_pParent) m_pParent->needUpdateBoundingBox();
        notifyUpdate();
    }

    virtual void onUpdate(ASceneNodeItem* pNodeItem);
    virtual void onDelete(ASceneNodeItem* pNodeItem);

    //! Gestionnaire de scene
    CSceneManager* m_pSceneManager;

	//! La matrice de transformation associée au noeud
	QMatrix4x4 m_LocalTransformation;

	//! La matrice de transformation inverse associée au noeud
	QMatrix4x4 m_LocalInverseTransformation;

    //! Taille locale
    QVector3D m_LocalScaling;

	//! La matrice de transformation associée au noeud
	mutable QMatrix4x4 m_GlobalTransformation;

	//! La matrice de transformation inverse associée au noeud
	mutable QMatrix4x4 m_GlobalInverseTransformation;

	//! Vrai si les matrices de transformation nécessitent une mise à jour
	mutable bool m_bNeedUpdateGlobalTransformation;

    //! La matrice de transformation associée au noeud
    mutable CBox3D m_LocalBoundingBox;

    //! La matrice de transformation associée au noeud
    mutable CBox3D m_LocalTransformedBoundingBox;

    //! La matrice de transformation associée au noeud
    mutable CBox3D m_GlobalBoundingBox;

    //! Vrai si la boite englobante locale nécessite une mise à jour
    mutable bool m_bNeedUpdateLocalBoundingBox;

    //! Vrai si la boite englobante globale nécessite une mise à jour
    mutable bool m_bNeedUpdateGlobalBoundingBox;

	//! L'item associé à ce noeud
    QList<ASceneNodeItem*> m_NodeItems;

	//! Parcourt et affiche l'arbre des noeuds sur la console
    virtual void recursiveDump(int iLevel) const;
};

#endif // CSCENENODE_H
