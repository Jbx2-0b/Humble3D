#ifndef CMESHANIMATION_H
#define CMESHANIMATION_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "AEntity.h"
#include "CMeshKey.h"

class LIB_CORE_SHARED_EXPORT CMeshAnimation : public AEntity
{

public:

    //! Constructeur
    CMeshAnimation(const QString& meshInstanceName, const QString& name = "MeshAnimation")
        : AEntity(name)
        , m_MeshInstanceName(meshInstanceName) {}

    //! Destructeur
    virtual ~CMeshAnimation() {}

    //! Retourne le nom du mesh associé
    const QString& getMeshInstanceName() const { return m_MeshInstanceName; }

    //! Accesseur sur les clefs d'animation
    QList<CMeshKey>& keys() { return m_Keys; }

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "MeshAnimation"; }

private:

    //! Nom du mesh auquel s'applique l'animation
    QString m_MeshInstanceName;

    //! Clefs d'animation
    QList<CMeshKey> m_Keys;
};

#endif // CMESHANIMATION_H
