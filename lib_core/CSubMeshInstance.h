#ifndef CSUBMESHINSTANCE_H
#define CSUBMESHINSTANCE_H

#include "ARenderableItem.h"
#include "CSubMesh.h"
class CMeshInstance;

class CSubMeshInstance
        : public ARenderableItem
        , public CSubMesh::ISubMeshListener
{
public:

    //! Constructeur
    CSubMeshInstance(CSceneManager* pSceneManager, CMeshInstance* pParent, CSubMesh* pSubMesh);

    //! Destructeur
    virtual ~CSubMeshInstance();

    //! Retourne l'instance du mesh parent
    CMeshInstance* getParentInstance() { return m_pParentInstance; }

    //! Retourne le submesh associ�
    CSubMesh* getSubMesh() { return m_pSubMesh; }

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "SubMeshInstance"; }

    //! Retourne la liste de noeuds auquel l'item est associ�
    virtual const QSet<CSceneNode*>& getNodes() const;

    //! D�finit si l'item est visible
    virtual bool isVisible() const;

    //! D�finit si le sous-objet h�rite des param�tres de vibilit� de son mesh parent
    void setInheritVisibility(bool bInherit) { m_bInheritVisibility = bInherit; }

    //! Retourne vrai si peut �tre rendu
    virtual bool isRenderable() const { return m_pSubMesh->isRenderable(); }

    //! Retourne vrai si poss�de un squelette d'animation
    virtual bool hasSkeleton() const { return m_pSubMesh->hasSkeleton(); }

    //! Retourne un conteneur contenant l'ensemble des matrices d'offset des bones
    virtual QVector<QMatrix4x4> getBoneTransformations() const;

    //! Retourne la boite englobante
    virtual const CBox3D& getBoundingBox(void) const { return m_pSubMesh->getBoundingBox(); }

    //! Retourne le nombre de polygones
    virtual unsigned int getPolygonCount() const { return m_pSubMesh->getPolygonCount(); }

protected:

    //-------------------------------------------------------------------------------------------------
    // Impl�mentation ARenderableItem
    //-------------------------------------------------------------------------------------------------

    //! Effectue le rendu
    virtual void render(ARenderer* pRenderer)
    {
        m_pSubMesh->render(pRenderer);
    }

    //-------------------------------------------------------------------------------------------------
    // Impl�mentation CSubMesh::ISubMeshListener
    //-------------------------------------------------------------------------------------------------

    void onMaterialChanged(CSubMesh* pSubMesh);

    //-------------------------------------------------------------------------------------------------

    //! L'instance du mesh parent
    CMeshInstance* m_pParentInstance;

    //! Submesh associ�
    CSubMesh* m_pSubMesh;

    //! Vrai si h�rite des param�tres de vibilit� de son mesh parent
    bool m_bInheritVisibility;

};

#endif // CSUBMESHINSTANCE_H
