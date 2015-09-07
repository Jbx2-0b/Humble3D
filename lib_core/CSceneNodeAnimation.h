#ifndef CSCENENODEANIMATION_H
#define CSCENENODEANIMATION_H

// Lib
#include "lib_core_global.h"

// Qt
#include <QString>
#include <QVector>

// Foundations
#include "CGeometryGlobal.h"
#include "AEntity.h"
#include "CVectorKey.h"
#include "CQuaternionKey.h"

class LIB_CORE_SHARED_EXPORT CSceneNodeAnimation : public AEntity
{
public:

    //! Constructeur
    CSceneNodeAnimation(const QString& nodeName, const QString& name = "NodeAnimation")
        : AEntity(name)
        , m_NodeName(nodeName)
        , m_eBehavior(eAnimationBehaviourDefault) {}

    //! Destructeur
    virtual ~CSceneNodeAnimation() {}

    //! Retourne le nom du node ou bone concerné par l'animation
    const QString& getNodeName() const { return m_NodeName; }

    //! Retourne le temps total de l'animation
    real getAnimationTime() const;

    //! Accesseur sur les clefs d'animation de position
    QList<CVectorKey>& positionKeys() { return m_PositionKeys; }

    //! Accesseur sur les clefs d'animation de rotation
    QList<CQuaternionKey>& rotationKeys() { return m_RotationKeys; }

    //! Accesseur sur les clefs d'animation de mise à l'échelle
    QList<CVectorKey>& scalingKeys() { return m_ScalingKeys; }

    //! Retourne les deux clefs entourant un temps t
    QPair<CVectorKey, CVectorKey> getPositionKeys(real dT) const;

    //! Retourne les deux clefs entourant un temps t
    QPair<CQuaternionKey, CQuaternionKey> getRotationKeys(real dT) const;

    //! Retourne les deux clefs entourant un temps t
    QPair<CVectorKey, CVectorKey> getScalingKeys(real dT) const;

    //! Définit la transition entre deux clefs d'animations
    void setAnimationBehavior(EnumAnimationBehaviour eBehavior) { m_eBehavior = eBehavior; }

    //! Retourne la transition entre deux clefs d'animations
    EnumAnimationBehaviour getAnimationBehavior() const { return m_eBehavior; }

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "SceneNodeAnimation"; }

private:

    //! Nom du noeud ou bone auquel s'applique l'animation
    QString m_NodeName;

    //! Clefs d'animation de position
    QList<CVectorKey> m_PositionKeys;

    //! Clefs d'animation de rotation
    QList<CQuaternionKey> m_RotationKeys;

    //! Clefs d'animation de mise à l'échelle
    QList<CVectorKey> m_ScalingKeys;

    //! Transition entre deux clefs
    EnumAnimationBehaviour m_eBehavior;
};


#endif // CSCENENODEANIMATION_H
