#ifndef CSOFTWARERENDERER_H
#define CSOFTWARERENDERER_H

// Foundations
#include "ARenderer.h"
#include "CSceneManager.h"
#include "CBox2D.h"
#include "CAnimationManager.h"
#include "CRasterizer.h"


class CSoftwareRenderer : public ARenderer
{
public:

    //! Constructeur
    CSoftwareRenderer(CSceneManager* pSceneManager);

    //! Destructeur
    virtual ~CSoftwareRenderer();

    //! Override - D�finit la taille de l'image de rendu
    virtual void setResolution(int iWidth, int iHeight);

    //! Definit la couleur de fond
    void setBackgroundColor(const QVector4D& color)
    {
        m_Rasterizer.setBackgroundColor(color);
    }

    //! Retourne l'image rendue
    const QImage& getImage() const	{ return m_Rasterizer.getImage(); }

    //--------------------------------------------------------------------------------------------
    // Impl�mentation ARenderer
    //--------------------------------------------------------------------------------------------

    //! Initialise le renderer
    virtual bool init();

    //! Effectue le rendu
    virtual void render();

    //! Effectue le rendu d'un vertex buffer
    virtual void renderMeshBuffer(CMeshBuffer* pBuffer);

    //! Effectue le rendu d'une liste de vertex buffer
    virtual void renderMeshBuffer(const QList<CMeshBuffer*>& buffers);

    //! Retourne le nom du type
    virtual QString getTypeName() const	{ return "GLRenderer"; }

    //--------------------------------------------------------------------------------------------
    // Fin impl�mentation ARenderer
    //--------------------------------------------------------------------------------------------

protected:

    //--------------------------------------------------------------------------------------------
    // Listeners
    //--------------------------------------------------------------------------------------------


    //! Impl�mentation - Interface CTextureManager::ITextureManagerListener
    virtual void onUpdateTexture(ATexture*) {}

    //! Impl�mentation - Interface CTextureManager::ITextureManagerListener
    virtual void onDeleteTexture(ATexture*) {}

    //! Impl�mentation - Interface CMeshManager::IMeshBufferListener
    virtual void onDeleteMeshBuffer(CMeshBuffer*) {}

    //! Impl�mentation - Interface CShaderManager::IShaderManagerListener
    virtual void onUpdateShader(CShader*) {}

    //! Impl�mentation - Interface CShaderManager::IShaderManagerListener
    virtual void onDeleteShader(CShader*) {}

    //! Impl�mentation - Interface CSceneManager::ISceneManagerListener
    virtual void onUpdateCamera(CCamera* pCamera);

    //! Impl�mentation - Interface CSceneManager::ISceneManagerListener
    virtual void onUpdateAnimation(CAnimation* pAnimation);

    //! Impl�mentation - Interface CSceneManager::ISceneManagerListener
    virtual void onCreateSceneNode(CSceneNode*) {}

    //! Impl�mentation - Interface CSceneManager::ISceneManagerListener
    virtual void onUpdateSceneNode(CSceneNode*) {}

    //! Impl�mentation - Interface CSceneManager::ISceneManagerListener
    virtual void onDeleteSceneNode(CSceneNode*) {}

    virtual void onUpdateRenderStates();

private:

    CRasterizer m_Rasterizer;

    bool isInsideScreen(const QVector2D& aV0, const QVector2D& aV1, const QVector2D& aV2)
    {
        return  m_ScreenBBox.intersect(aV0, aV1) ||
                m_ScreenBBox.intersect(aV2, aV0) ||
                m_ScreenBBox.intersect(aV2, aV1);

    }

    CBox2D m_ScreenBBox;
    QMatrix4x4 m_ModelViewProjectionMatrix;
    CMaterial* m_pCurrentMaterial;
};

#endif // CSOFTWARERENDERER_H
