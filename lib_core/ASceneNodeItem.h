#ifndef ASCENENODEITEM_H
#define ASCENENODEITEM_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "AEntity.h"
#include "CGeometryGlobal.h"

// Qt
#include <QString>

class ASceneNodeItem;
class CSceneManager;
class CSceneNode;

class LIB_CORE_SHARED_EXPORT ASceneNodeItem : public AEntity
{
    friend class CSceneNode;

public:

	//! Constructeur
    ASceneNodeItem(CSceneManager* pSceneManager, const QString& name);

	//! Destructeur
    virtual ~ASceneNodeItem();

	//-------------------------------------------------------------------------------------------------
	// Listeners
	//-------------------------------------------------------------------------------------------------

    class ISceneNodeItemListener
	{
        friend class ASceneNodeItem;

	protected:
        virtual void onUpdate(ASceneNodeItem* pNodeItem) = 0;
        virtual void onDelete(ASceneNodeItem* pNodeItem) = 0;
	};

	//! Enregistre un listener
    void registerListener(ISceneNodeItemListener* pListener);

	//! Lib�re un listener
    void unregisterListener(ISceneNodeItemListener* pListener);

	//-------------------------------------------------------------------------------------------------
	// Setters
	//-------------------------------------------------------------------------------------------------

	//! D�finit si l'item est visible
	virtual void setVisible(bool bVisible);

	//! D�finit si l'item est selectionnable
	virtual void setSelectable(bool bSelectable);

	//! D�finit si l'item est selectionn�
	virtual void setSelected(bool bSelected);

    //! D�finit le type d'objet pour la simulation physique
    void setPhysicShape(EnumPhysicShape ePhysicShape);

    //! D�finit la masse de l'objet
    void setMass(real dMass);

    //! D�finit le coefficient de restitution (= elasticit�). Valeurs dans l'intervalle [0; 1]
    void setRestitution(real dRestitution);

    //! D�finit le coefficient de friction
    void setFriction(real dFriction);

    //! D�finit le coefficient de friction pour les rotations
    void setRollingFriction(real dRollingFriction);

    //! D�finit la vitesse lin�aire
    void setLinearVelocity(const QVector3D& linearVelocity);

    //! D�finit la vitesse angulaire
    void setAngularVelocity(const QVector3D& angularVelocity);

	//-------------------------------------------------------------------------------------------------
	// Getters
	//-------------------------------------------------------------------------------------------------

	//! Retourne un pointeur sur le gestionnaire de sc�nes
    CSceneManager* getSceneManager() const;

	//! Retourne true si l'item est visible
	virtual bool isVisible() const;

	//! Retourne true si l'item est selectionnable
	virtual bool isSelectable() const;

	//! Retourne true si l'item est selectionn�
	virtual bool isSelected() const;

	//! Retourne la liste de noeuds auquel l'item est associ�
    virtual const QSet<CSceneNode*>& getNodes() const;

    //! Retourne le nombre de noeuds auquel l'item est associ�
    unsigned int getNodeCount() const;

	//! Retourne le premier noeud de la liste, null si non d�finit
    CSceneNode* getNode() const;

    //! Retourne la forme de l'objet pour la simulation physique
    EnumPhysicShape getPhysicShape() const;

    //! Retourne la masse de l'objet
    real getMass() const;

    //! Retourne le coefficient de restitution (= elasticit�). Valeurs dans l'intervalle [0; 1]
    real getRestitution() const;

    //! Retourne le coefficient de friction
    real getFriction() const;

    //! Retourne le coefficient de friction pour les rotations
    real getRollingFriction() const;

    //! Retourne la vitesse lin�aire
    QVector3D getLinearVelocity() const;

    //! Retourne la vitesse angulaire
    QVector3D getAngularVelocity() const;

    //! Retourne la boite englobante
    virtual const CBox3D& getBoundingBox() const = 0;

    //! Retourne le nombre de polygones
    virtual unsigned int getPolygonCount() const = 0;

    //! Retourne la distance � un point
    real getDistance(const QVector3D& pt)
    {
        real dDistance = 0.;
        CRay ray(pt, getBoundingBox().getCenter() - pt);
        if (getBoundingBox().intersection(ray, &dDistance))
        {
            return dDistance;
        }
        return 0.;
    }
	
    //! Calcule l'intersection avec un rayon
    virtual bool intersection(const CRay& ray, real* dDistance, const QMatrix4x4& transformation = QMatrix4x4()) const
    {
        return getBoundingBox().transformed(transformation).intersection(ray, dDistance);
    }

	//! Cr�e une string contenant les propri�t�s de l'objet
    virtual QString toString() const
	{
		QString result;
		result += "[SceneNodeItem \n";
		result += AEntity::toString();
		result += QString("Visible: %1\n").arg(m_bVisible);
		result += QString("Selectable: %1\n").arg(m_bSelectable);
		result += QString("Selected: %1\n").arg(m_bSelected);
		result += QString("Physic Shape: %1\n").arg(CGeometryGlobal::stringFromPhysicShape(m_ePhysicShape));
		result += QString("Mass: %1\n").arg(m_dMass);
		result += "]\n";
		return result;
	}

protected:

    //! Liste des listeners
    QSet<ISceneNodeItemListener*> m_SceneNodeListeners;

    //! Transmet le message de mise � jour
    virtual void notifyUpdate();

    //! Transmet le message de suppression
    virtual void notifyDelete();

    //! Pointeur sur le scenemanager conternant l'objet, null sinon
    CSceneManager* m_pSceneManager;

	//! True si est visible
    bool m_bVisible;

	//! True si est selectionnable
	bool m_bSelectable;

	//! True si l'objet est selectionn�
	bool m_bSelected;

    //! Type d'objet pour la simulation physique
    EnumPhysicShape m_ePhysicShape;

    //! Masse de l'objet pour une simulation physique
    real m_dMass;

    //! Coefficient de restitution (= elasticit�). Valeurs dans l'intervalle [0; 1]
    real m_dRestitution;

    //! Coefficient de friction
    real m_dFriction;

    //! Coefficient de friction pour les rotations
    real m_dRollingFriction;

    //! Vitesse lin�aire
    QVector3D m_LinearVelocity;

    //! Vitesse angulaire
    QVector3D m_AngularVelocity;

	//! Ajoute un noeud
	void addNode(CSceneNode* pNode);

	//! Supprime un noeud
	void removeNode(CSceneNode* pNode);

	//! Liste des noeuds auquel l'item est associ�
    QSet<CSceneNode*> m_Nodes;
};

#endif // ASCENENODEITEM_H
