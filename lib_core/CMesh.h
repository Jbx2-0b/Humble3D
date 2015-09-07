#ifndef CMESH_H
#define CMESH_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMath.h"

// Current
#include "AEntity.h"
#include "CBox3D.h"
#include "CSphere.h"
#include "CSubMesh.h"

class CSceneManager;
class CMesh;
class CSceneNode;

typedef QList<CMesh*> CMeshGroup;

//#define ENABLE_MULTITHREADING

#ifdef ENABLE_MULTITHREADING
#include "CGeometryInstancer.h"
#endif // ENABLE_MULTITHREADING

//-----------------------------------------------------------------------------------------
/// \class CMesh
/// \brief Classe représentant un maillage 3D
//-----------------------------------------------------------------------------------------
class LIB_CORE_SHARED_EXPORT CMesh : public AEntity, public AWashable
{
    friend class CMeshManager;
    friend class CGeometryInstancer;

public:

    //! Constructeur
    CMesh(const QString& name);

    //! Destructeur
    virtual ~CMesh();

    //! Initialise l'objet
    // TODO : varitric arguments ?
    //void init(const QVector<QString>& args) {}

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IMeshListener
    {
        friend class CMesh;

    protected:
        virtual void onUpdate(CMesh* pMesh) = 0;
        virtual void onDelete(CMesh* pMesh) = 0;
    };

    //! Enregistre un listener
    void registerListener(IMeshListener* pListener);

    //! Libére un listener
    void unregisterListener(IMeshListener* pListener);


    //-------------------------------------------------------------------------------------------------
    // Gestion des sous-objets
    //-------------------------------------------------------------------------------------------------

    //! Crée un sous-objet
    CSubMesh* createSubMesh();

    //! Supprime un sous-objet
    void removeSubMesh(CSubMesh* pSubMesh);

    //! Supprime l'ensemble des sous-objets
    void clearSubMeshs();

    //! Accesseur sur les submeshs
    const QList<CSubMesh*>& subMeshs() const { return m_SubMeshs; }

    //--------------------------------------------------------------------------------------------
    // Setters
    //--------------------------------------------------------------------------------------------

    //! Définit le materiau
    void setMaterialName(const QString& materialName);

    //! Définit le multithreading est activé
    void setMultithreadingEnabled(bool bEnabled);

    //--------------------------------------------------------------------------------------------
    // Getters
    //--------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "Mesh"; }

    //! Retourne la boîte englobant l'objet
    const CBox3D& getBoundingBox() const;

    //! Retourne la sphere englobant l'objet
    const CSphere& getBoundingSphere() const;

    //! Retourne true si le mesh a un squelette d'animation
    bool hasSkeleton() const;

    //! Retourne le nombre de face du mesh
    unsigned int getPolygonCount() const;

    //! Retourne la liste de noeuds associés aux bones
    QMultiHash<CSceneNode*, CBone> getBoneNodes(CSceneManager* pSceneManager) const;


    //--------------------------------------------------------------------------------------------
    // Fonctions mathématiques
    //--------------------------------------------------------------------------------------------

    //! Calcule les normales
    virtual void computeNormals();

    //! Calcule les tangeantes
    virtual void computeTangents();

    //! Calcule l'intersection avec un rayon
    virtual bool intersection(const CRay& ray, real* dDistance, const QMatrix4x4& transformation = QMatrix4x4()) const;

    //! Calcule l'intersection avec un rayon
    bool intersects(const CRay &ray, const QMatrix4x4& transformation = QMatrix4x4()) const;

    //! Crée une string contenant les propriétés de l'objet
    virtual QString toString() const
    {
        QString result;
        result += "[Mesh \n";
        result += AEntity::toString();
        result += QString("SubMesh count: %1\n").arg(m_SubMeshs.size());
        result += QString("Face count: %1\n").arg(getPolygonCount());
        result += "]\n";
        return result;
    }

    //! Met à jour le mesh (appelle updateGeometry)
    void update();

    //--------------------------------------------------------------------------------------------
    // Implémentation AWashable
    //--------------------------------------------------------------------------------------------

    //! Passe l'entitée à sale = nécessite une mise à jour
    virtual void setDirty();

protected:

    //! Transmet le message de mise à jour
    virtual void notifyUpdate();

    //! Transmet le message de suppression
    virtual void notifyDelete();

    //! Liste des listeners
    QSet<IMeshListener*> m_MeshListeners;

    //! Boîte englobante
    mutable CBox3D m_BoundingBox;

    //! Sphère englobante
    mutable CSphere m_BoundingSphere;

    //! Si vrai la boite englobante doit etre mise à jour
    mutable bool m_bNeedUpdateBoundingBox;

    //! Si vrai la sphere englobante doit etre mise à jour
    mutable bool m_bNeedUpdateBoundingSphere;

    //! Conteneur des submeshs
    QList<CSubMesh*> m_SubMeshs;

    //! Vrai si le multithreading est activé
    bool m_bMultithreadingEnabled;

    //! Fonction appelée à l'initialisation de l'objet
    virtual void updateGeometry() {}

private:

    CMesh() {}

#ifdef ENABLE_MULTITHREADING
    static CGeometryInstancer s_GeometryInstancer;
#endif // ENABLE_MULTITHREADING
};

#endif // CMESH_H
