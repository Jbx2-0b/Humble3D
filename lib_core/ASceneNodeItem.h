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

    //! Libére un listener
    void unregisterListener(ISceneNodeItemListener* pListener);

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Définit si l'item est visible
    virtual void setVisible(bool bVisible);

    //! Définit si l'item est selectionnable
    virtual void setSelectable(bool bSelectable);

    //! Définit si l'item est selectionné
    virtual void setSelected(bool bSelected);

    //! Définit le type d'objet pour la simulation physique
    void setPhysicShape(EnumPhysicShape ePhysicShape);

    //! Définit la masse de l'objet
    void setMass(real dMass);

    //! Définit le coefficient de restitution (= elasticité). Valeurs dans l'intervalle [0; 1]
    void setRestitution(real dRestitution);

    //! Définit le coefficient de friction
    void setFriction(real dFriction);

    //! Définit le coefficient de friction pour les rotations
    void setRollingFriction(real dRollingFriction);

    //! Définit la vitesse linéaire
    void setLinearVelocity(const QVector3D& linearVelocity);

    //! Définit la vitesse angulaire
    void setAngularVelocity(const QVector3D& angularVelocity);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne un pointeur sur le gestionnaire de scénes
    CSceneManager* getSceneManager() const;

    //! Retourne true si l'item est visible
    virtual bool isVisible() const;

    //! Retourne true si l'item est selectionnable
    virtual bool isSelectable() const;

    //! Retourne true si l'item est selectionné
    virtual bool isSelected() const;

    //! Retourne la liste de noeuds auquel l'item est associé
    virtual const QSet<CSceneNode*>& getNodes() const;

    //! Retourne le nombre de noeuds auquel l'item est associé
    unsigned int getNodeCount() const;

    //! Retourne le premier noeud de la liste, null si non définit
    CSceneNode* getNode() const;

    //! Retourne la forme de l'objet pour la simulation physique
    EnumPhysicShape getPhysicShape() const;

    //! Retourne la masse de l'objet
    real getMass() const;

    //! Retourne le coefficient de restitution (= elasticité). Valeurs dans l'intervalle [0; 1]
    real getRestitution() const;

    //! Retourne le coefficient de friction
    real getFriction() const;

    //! Retourne le coefficient de friction pour les rotations
    real getRollingFriction() const;

    //! Retourne la vitesse linéaire
    QVector3D getLinearVelocity() const;

    //! Retourne la vitesse angulaire
    QVector3D getAngularVelocity() const;

    //! Retourne la boite englobante
    virtual const CBox3D& getBoundingBox() const = 0;

    //! Retourne le nombre de polygones
    virtual unsigned int getPolygonCount() const = 0;

    //! Retourne la distance à un point
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

    //! Crée une string contenant les propriétés de l'objet
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

    //! Transmet le message de mise à jour
    virtual void notifyUpdate();

    //! Transmet le message de suppression
    virtual void notifyDelete();

    //! Pointeur sur le scenemanager conternant l'objet, null sinon
    CSceneManager* m_pSceneManager;

    //! True si est visible
    bool m_bVisible;

    //! True si est selectionnable
    bool m_bSelectable;

    //! True si l'objet est selectionné
    bool m_bSelected;

    //! Type d'objet pour la simulation physique
    EnumPhysicShape m_ePhysicShape;

    //! Masse de l'objet pour une simulation physique
    real m_dMass;

    //! Coefficient de restitution (= elasticité). Valeurs dans l'intervalle [0; 1]
    real m_dRestitution;

    //! Coefficient de friction
    real m_dFriction;

    //! Coefficient de friction pour les rotations
    real m_dRollingFriction;

    //! Vitesse linéaire
    QVector3D m_LinearVelocity;

    //! Vitesse angulaire
    QVector3D m_AngularVelocity;

    //! Ajoute un noeud
    void addNode(CSceneNode* pNode);

    //! Supprime un noeud
    void removeNode(CSceneNode* pNode);

    //! Liste des noeuds auquel l'item est associé
    QSet<CSceneNode*> m_Nodes;
};

#endif // ASCENENODEITEM_H
