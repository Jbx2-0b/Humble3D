#ifndef CSCENEMANAGER_H
#define CSCENEMANAGER_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "ASceneNodeItem.h"
#include "CLight.h"
#include "CCamera.h"
#include "CMaterial.h"
#include "CShader.h"
#include "CMeshManager.h"
#include "CAnimation.h"
#include "CMeshInstance.h"
#include "CBillboard.h"
#include "CRenderQueue.h"

class COctree;

static const QVector4D DefaultSceneAmbientColor(1.0, 1.0, 1.0, 1.);

//-----------------------------------------------------------------------------------------
// \class CSceneManager
// \brief Classe permettant de stocker les differentes entitees 3D.
//-----------------------------------------------------------------------------------------
class LIB_CORE_SHARED_EXPORT CSceneManager
        : public AEntity
        , public CMeshInstance::IMeshInstanceListener
        , public CMeshManager::IMeshManagerListener
        , public CCamera::ICameraListener
        , public ARenderableItem::IRenderableItemListener
        , public CAnimation::IAnimationListener
        , public CSceneNode::INodeListener
{
    friend class CSceneNode;

public:

    //! Constructeur
    CSceneManager();

    //! Destructeur
    virtual ~CSceneManager();

    //! Construit un octree
    void buildOctree(const CBox3D& worldBoundingBox, unsigned int uiPolygonCountThreshold, unsigned int iMaxDepth);

    //! Libére l'ensemble des données du SceneManager
    void clearAll();

    //! Libére l'ensemble des meshs du SceneManager
    void clearMeshInstances();

    //! Libére l'ensemble des sources lumineuses du SceneManager
    void clearLights();

    //! Libére l'ensemble des caméras du SceneManager
    void clearCameras();

    //! Libére l'ensemble des animations du SceneManager
    void clearAnimations();

    //! Libére l'ensemble des élements 2D
    void clearBillboards();

    //! Libére l'ensemble des items associés aux noeuds du SceneManager
    void clearNodeItems();

    //! Libére l'ensemble des noeuds du SceneManager
    void clearNodes();

    //! Libére l'ensemble des noeuds inutilisés
    void clearEmptyNodes();

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IAnimationListener
    {
        friend class CSceneManager;

    protected:
        //! Prévient du démarrage d'une animation
        virtual void onStartAnimation(CAnimation* pAnimation) = 0;

        //! Prévient de la mise en pause d'une animation
        virtual void onPauseAnimation(CAnimation* pAnimation) = 0;

        //! Prévient de l'arrêt d'une animation
        virtual void onStopAnimation(CAnimation* pAnimation) = 0;
    };

    class ISceneManagerListener
    {
        friend class CSceneManager;

    protected:
        //! Prévient de la mise à jour d'une caméra
        virtual void onUpdateCamera(CCamera* pCamera) = 0;

        //! Prévient de la mise à jour d'une animation
        virtual void onUpdateAnimation(CAnimation* pAnimation) = 0;

        //! Prévient de l'ajout d'un noeud
        virtual void onCreateSceneNode(CSceneNode* pSceneNode) = 0;

        //! Prévient de la mise à jour d'un noeud
        virtual void onUpdateSceneNode(CSceneNode* pSceneNode) = 0;

        //! Prévient de la mise à jour d'un noeud
        virtual void onDeleteSceneNode(CSceneNode* pSceneNode) = 0;
    };


    //! Enregistre un listener
    void registerAnimationListener(CSceneManager::IAnimationListener* pListener);

    //! Libére un listener
    void unregisterAnimationListener(CSceneManager::IAnimationListener* pListener);

    //! Enregistre un listener
    void registerListener(CSceneManager::ISceneManagerListener* pListener);

    //! Libére un listener
    void unregisterListener(CSceneManager::ISceneManagerListener* pListener);


    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const													{ return "SceneManager"; }

    //! Retourne les noeuds de scene
    const QSet<CSceneNode*>& getSceneNodes() const                                      { return m_SceneNodes; }

    //! Retourne le noeud root
    CSceneNode* getRootNode()															{ return &m_RootNode; }

    //! Retourne le nombre de mesh
    int getMeshInstanceCount() const													{ return m_MeshInstances.size(); }

    //! Retourne le nombre de lumiéres
    int getLightCount() const															{ return m_Lights.size(); }

    //! Retourne le nombre de cameras
    int getCameraCount() const															{ return m_Cameras.size(); }

    //! Retourne le nombre d'animations
    int getAnimationCount() const														{ return m_AnimationByID.size(); }

    //! Retourne le nombre de face
    int getPolygonCount() const;

    //! Retourne la collection d'items
    QList<ASceneNodeItem*> getItems() const                                             { return m_NodeItemByID.values(); }

    //! Retourne la collection d'items
    QList<QString> getItemNames() const;

    //! Retourne la collection d'objets pouvant être rendue
    const QSet<ARenderableItem*>& getRenderableItems() const                            { return m_RenderableItems; }

    //! Retourne la collection de meshs
    QList<CMeshInstance*>& getMeshInstances()											{ return m_MeshInstances; }

    //! Retourne la collection de lumiéres
    QList<CLight*>& getLights()															{ return m_Lights; }

    //! Retourne la collection de caméras
    QList<CCamera*>& getCameras()														{ return m_Cameras; }

    //! Retourne la collection d'animations
    QList<CAnimation*> getAnimations()	const											{ return m_AnimationByID.values(); }

    //! Retourne la collection d'instances de meshs
    const QList<CMeshInstance*>& getMeshInstances() const								{ return m_MeshInstances; }

    //! Retourne la collection de lumiéres
    const QList<CLight*>& getLights() const												{ return m_Lights; }

    //! Retourne la collection de caméras
    const QList<CCamera*>& getCameras() const											{ return m_Cameras; }

    //! Retourne la collection des eléments 2D
    const QList<CBillboard*>& getBillboards() const										{ return m_Billboards; }

    //! Retourne une animation en fonction de son nom
    CAnimation* getAnimationByName(const QString& name)	const;

    //! Retourne la file d'attente des items dont on doit effectuer le rendu
    const CRenderQueue& getRenderQueue() const                                          { return m_RenderQueue; }

    //! Retourne la file d'attente des items transparents dont on doit effectuer le rendu
    const CRenderQueue& getTransparentRenderQueue() const                               { return m_TransparentRenderQueue; }

    //! Retourne la boite englobante de la scene
    CBox3D getWorldAxisAlignedBoundingBox() const;

    //! Retourne la couleur ambiante de la scéne
    QVector4D getAmbientColor() const;

    //! Retourne les matériaux utilisés dans la scene
    const QSet<CMaterial*> getUsedMaterials() const;

    //! Retourne les shaders utilisés dans la scene
    const QSet<CShader*> getUsedShaders() const;

    //! Retourne les shaders utilisés dans la scene
    const QSet<ATexture*> getUsedTextures() const;

    //! Retourne les noeuds visibles pour un frustum particulier. Attention, sans octree, cette méthode peut être lente.
    QSet<CSceneNode*> getVisibleSceneNodes(const CFrustum& frustum);

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Définit la couleur ambiante de la scéne
    void setAmbientColor(const QVector4D& ambientColor);

    //-------------------------------------------------------------------------------------------------
    // Creator & Remover
    //-------------------------------------------------------------------------------------------------

    //! Crée une nouvelle instance de mesh et retourne son pointeur
    CMeshInstance* createMeshInstance(const QString& meshName, const QString& name = "MeshInstance");

    //! Crée une nouvelle instance de mesh et retourne son pointeur
    CMeshInstance* createMeshInstance(CMesh* pMesh, const QString& name = "MeshInstance");

    //! Crée une nouvelle lumiére et retourne son pointeur
    CLight* createLight(const QString& name = "Light");

    //! Crée une nouvelle caméra et retourne son pointeur
    CCamera* createCamera(const QString& name = "Camera");

    //! Crée une nouvelle animation et retourne son pointeur
    CAnimation* createAnimation(const QString& name = "Animation");

    //! Crée un nouvel élément 2D
    CBillboard* createBillboard(const QString& name = "Billboard");

    //! Supprime une instance de mesh
    void deleteMeshInstance(CMeshInstance* pMeshInstance);

    //! Supprime une lumiére
    void deleteLight(CLight* pLight);

    //! Supprime une caméra
    void deleteCamera(CCamera* pCamera);

    //! Supprime une animation
    void deleteAnimation(CAnimation* pAnimation);

    //! Supprime un élement 2D
    void deleteBillboards(CBillboard* pBillboard);

    //-------------------------------------------------------------------------------------------------
    //
    //-------------------------------------------------------------------------------------------------

    //! Retourne l'objet placé sous le pointeur de la souris, 0 sinon
    ASceneNodeItem* pick(CCamera* pCamera, const QPoint& position, const QSize& viewSize);

    //! Recherche un item en fonction de son nom
    ASceneNodeItem* findItemByName(const QString& name);

    //! Recherche un item en fonction de son nom
    template<typename Item> Item* findItemByName(const QString& name);

    //! Recherche un noeud en fonction de son nom
    CSceneNode* findNode(const QString& name);

    //! Retourne un noeud en fonction de son ID
    CSceneNode* findNode(int iNodeID);

protected:

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    //! Conteneur des listeners
    QSet<IAnimationListener*> m_AnimationListeners;

    //! Conteneur des listeners
    QSet<ISceneManagerListener*> m_SceneManagerListeners;

    //! Notifie les listeners qu'une animation est lancée
    void notifyStart(CAnimation* pAnimation);

    //! Notifie les listeners qu'une animation est mit en pause
    void notifyPause(CAnimation* pAnimation);

    //! Notifie les listeners qu'une animation stoppée
    void notifyStop(CAnimation* pAnimation);

    //! Notifie les listeners qu'une caméra a été mise à jour
    void notifyUpdate(CCamera* pCamera);

    //! Implémentation de CMeshInstance::IMeshInstanceListener
    virtual void onUpdate(CMeshInstance* pMeshInstance);

    //! Implémentation de CCamera::ICameraListener
    virtual void onUpdate(CCamera* pCamera);

    //! Implémentation de ARenderableItem::IRenderableItemListener
    virtual void onMaterialChanged(ARenderableItem* pMesh);

    //! Implémentation de CAnimation::IAnimationListener
    virtual void onStart(CAnimation* pAnimation);

    //! Implémentation de CAnimation::IAnimationListener
    virtual void onPause(CAnimation* pAnimation);

    //! Implémentation de CAnimation::IAnimationListener
    virtual void onStop(CAnimation* pAnimation);

    //! Implémentation de CMeshManager::IMeshManagerListener
    virtual void onUpdate(CMesh* pMesh);

    //! Implémentation de CMeshManager::IMeshManagerListener
    virtual void onDelete(CMesh* pMesh);

    //! Implémentation de CSceneNode::INodeListener
    virtual void onUpdate(CSceneNode* pSceneNode);

    //! Implémentation de CSceneNode::INodeListener
    virtual void onDelete(CSceneNode* pSceneNode);

    //-------------------------------------------------------------------------------------------------
    // End listeners
    //-------------------------------------------------------------------------------------------------

    //! Trouve les intersections de maniére récursive en explorant l'arbre des noeuds
    ASceneNodeItem* recursiveFindIntersection(const CRay& ray, CSceneNode* pNode, real& dDistance);

    //! Libére l'ensemble des noeuds inutilisés de maniére récursive
    bool recursiveClearEmptyNodes(CSceneNode* pNode);

    //! Octree
    COctree* m_pOctree;

    //! Couleur ambiante de la scéne
    QVector4D m_AmbientColor;

    //! Conteneur des instances de maillage
    QList<CMeshInstance*> m_MeshInstances;

    //! Conteneur de items dessinable
    QSet<ARenderableItem*> m_RenderableItems;

    //! Conteneur des sources lumineuses
    QList<CLight*> m_Lights;

    //! Conteneur des caméras
    QList<CCamera*> m_Cameras;

    //! Conteneur des éléments 2D
    QList<CBillboard*> m_Billboards;

    //! Conteneur des animations
    QHash<int, CAnimation*> m_AnimationByID;

    //! Conteneur des items associés à un noeud
    QHash<int, ASceneNodeItem*> m_NodeItemByID;

    //! File d'attente des items dont on doit effectuer le rendu
    mutable CRenderQueue m_RenderQueue;

    //! File d'attente des items transparents dont on doit effectuer le rendu
    mutable CRenderQueue m_TransparentRenderQueue;

    //! Ajoute un item à la file d'attente
    void addToRenderQueue(ARenderableItem* pRenderableItem);

    //! Supprime un item de la file d'attente
    void removeFromRenderQueue(ARenderableItem* pRenderableItem);

    //! Conteneur des objets visibles
    mutable QSet<ARenderableItem*> m_VisibleItems;

    //! Ajoute un mesh
    void appendMeshInstance(CMeshInstance* pMeshInstance, const QString& itemName = "");

    //! Ajoute un objet dessinable
    void appendRenderableItem(ARenderableItem* pSubMesh);

    //! Supprime un objet dessinable
    void removeRenderableItem(ARenderableItem* pSubMesh);

    //! Conteneur des noeuds de scene
    QSet<CSceneNode*> m_SceneNodes;

    //! Ajoute un noeud de scene
    void addSceneNode(CSceneNode* pSceneNode);

    //! Noeud racine
    CSceneNode m_RootNode;
};



//-----------------------------------------------------------------------------------------
template<typename Item> Item* CSceneManager::findItemByName(const QString& name)
{
    return dynamic_cast<Item*>(findItemByName(name));
}

#endif // CSCENEMANAGER_H


