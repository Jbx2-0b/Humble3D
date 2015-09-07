#ifndef CSUBMESH_H
#define CSUBMESH_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "AEntity.h"
#include "CMeshBuffer.h"
#include "CRenderStates.h"

// Qt
#include <QVector3D>
#include <QVector4D>

class CMesh;
class ARenderer;

class LIB_CORE_SHARED_EXPORT CSubMesh : public CMeshBuffer

{
public:

    //! Ctor
    CSubMesh(CMesh* pParentMesh);

    //! Dtor
    virtual ~CSubMesh();

    //! Retourne le parent
    const CMesh* getParentMesh() const { return m_pParentMesh; }

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "SubMesh"; }

    //! Retourne vrai si peut �tre rendu
    bool isRenderable() const { return !m_PositionsBuffer.isEmpty(); }

    //! Effectue le rendu
    void render(ARenderer* pRenderer);

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class ISubMeshListener
    {
        friend class CSubMesh;

    protected:
        virtual void onMaterialChanged(CSubMesh* pSubMesh) = 0;
    };

    //! Enregistre un listener
    void registerListener(ISubMeshListener* pListener)
    {
        m_SubMeshListeners.insert(pListener);
    }

    //! Lib�re un listener
    void unregisterListener(ISubMeshListener* pListener)
    {
        m_SubMeshListeners.remove(pListener);
    }

    //! Pr�vient les listeners que le mat�riau a chang�
    void notifyMaterialChanged()
    {
        foreach (ISubMeshListener* pListener, m_SubMeshListeners)
        {
            pListener->onMaterialChanged(this);
        }
    }

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! D�finit le materiau
    void setMaterialName(const QString& materialName);

    //! D�finit les param�tres de rendu
    void setRenderParameters(const CRenderStates& renderStates)         { m_RenderStates = renderStates; }

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne l'ID du materiau
    const QString& getMaterialName() const								{ return m_MaterialName; }

    //! Retourne true si a un mat�riau
    bool hasMaterial() const;

    //! Retourne les param�tres de rendu
    const CRenderStates& getRenderParameters() const					{ return m_RenderStates; }

protected:

    //! Liste des listeners
    QSet<ISubMeshListener*> m_SubMeshListeners;

    //! Mesh parent
    CMesh* m_pParentMesh;

    //! ID du mat�riau
    QString m_MaterialName;

    //! Param�tres de rendu
    CRenderStates m_RenderStates;
};

QDataStream& operator << (QDataStream& out, const CSubMesh& mesh);
QDataStream& operator >> (QDataStream& in,	CSubMesh& mesh);

#endif // CSUBMESH_H
