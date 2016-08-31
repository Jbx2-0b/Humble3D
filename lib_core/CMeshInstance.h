#ifndef CMESHINSTANCE_H
#define CMESHINSTANCE_H

// Lib
#include "lib_core_global.h"

// Fundations
#include "ASceneNodeItem.h"
#include "CMesh.h"
#include "CSubMeshInstance.h"

class LIB_CORE_SHARED_EXPORT CMeshInstance
        : public ASceneNodeItem
        , public CMesh::IMeshListener
{
    friend class CSubMeshInstance;

public:

    //! Constructeur
    CMeshInstance(CSceneManager* pSceneManager, CMesh* pMesh, const QString& name);

    //! Destructeur
    virtual ~CMeshInstance();

    //! Retourne le mesh associé
    CMesh* getMesh() { return m_pMesh; }

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IMeshInstanceListener
    {
        friend class CMeshInstance;

    protected:
        virtual void onUpdate(CMeshInstance* pMeshInstance) = 0;
    };

    //! Enregistre un listener
    void registerListener(IMeshInstanceListener* pListener);

    //! Libére un listener
    void unregisterListener(IMeshInstanceListener* pListener);

    //-------------------------------------------------------------------------------------------------
    // Gestion des sous-objets
    //-------------------------------------------------------------------------------------------------

    //! Accesseur sur les submeshs
    QList<CSubMeshInstance*> subMeshInstances() const { return m_SubMeshInstanceMap.values(); }

    //--------------------------------------------------------------------------------------------
    // Setters
    //--------------------------------------------------------------------------------------------

    //! Définit le materiau
    void setMaterialName(const QString& materialName);

    //! Définit les paramétres de rendu
    void setRenderStates(const CRenderStates& renderStates);

    //! Définit le repère local à partir desquelles sont effectuées les transformations de squelettes
    void setLocalRootNode(CSceneNode* pLocalRootNode) { m_pLocalRootNode = pLocalRootNode; }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "MeshInstance"; }

    //! Retourne le nom du materiau
    const QString& getMaterialName() const { return m_MaterialName; }

    //! Retourne le repère local à partir desquelles sont effectuées les transformations de squelettes
    CSceneNode* getLocalRootNode() const { return m_pLocalRootNode; }

    //! Retourne la boite englobante
    virtual const CBox3D& getBoundingBox(void) const { return m_pMesh->getBoundingBox(); }

    //! Retourne le nombre de polygones
    virtual unsigned int getPolygonCount() const { return m_pMesh->getPolygonCount(); }

    //--------------------------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------------------------

    //! Crée une string contenant les propriétés de l'objet
    virtual QString toString() const
    {
        QString result;
        result += "[MeshInstance \n";
        result += ASceneNodeItem::toString();
        result += QString("Mesh name: %1\n").arg(m_pMesh->getName());
        result += QString("Material name: %1\n").arg(m_MaterialName);
        foreach (CSubMeshInstance* pSubMeshInstance, m_SubMeshInstanceMap.values())
        {
            result += pSubMeshInstance->toString();
        }
        result += "]\n";
        return result;
    }

protected:

    //! Transmet le message de mise à jour
    virtual void notifyUpdate();

    //! Liste des listeners
    QSet<IMeshInstanceListener*> m_MeshInstanceListeners;


    //-------------------------------------------------------------------------------------------------
    // Implémentation de CMesh::IMeshListener
    //-------------------------------------------------------------------------------------------------

    virtual void onUpdate(CMesh* pMesh);
    virtual void onDelete(CMesh* pMesh);

    //-------------------------------------------------------------------------------------------------

    //! Mesh associé
    CMesh* m_pMesh;

    //! Nom du matériau
    QString m_MaterialName;

    //! Etat pour le rendu
    CRenderStates renderStates;

    //! Conteneur d'instance de sous objet
    QHash<CSubMesh*, CSubMeshInstance*> m_SubMeshInstanceMap;

    //! Crée une instance de sous objet
    CSubMeshInstance* createSubMeshInstance(CSubMesh* pSubMesh);

    //! Supprime une instance de sous objet
    void removeSubMeshInstance(CSubMesh* pSubMesh);

    //! Repère local à partir desquelles sont effectuées les transformations de squelettes
    CSceneNode* m_pLocalRootNode;
};

#endif // CMESHINSTANCE_H
