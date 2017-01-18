#ifndef OGLRENDERER_H
#define OGLRENDERER_H

// Lib
#include "lib_opengl_global.h"

// Lib geometry
#include "ARenderer.h"
#include "CMath.h"
#include "AEntity.h"
#include "CSceneManager.h"
#include "CMaterialManager.h"
#include "CFrameBuffer.h"
#include "CShader.h"

// Lib OpenGL
#include "CGLMeshBuffer.h"
#include "CGLFrameBuffer.h"
#include "CRenderQueue.h"

// App
#include "CGLGlobal.h"
#include "CGLHelper.h"
#include "CGLContext.h"
#include "AGLTexture.h"



//-----------------------------------------------------------------------------------------
// \class CGLRenderer
// \brief Classe de rendu OpenGL
//-----------------------------------------------------------------------------------------
class LIB_OPENGLSHARED_EXPORT CGLRenderer : public ARenderer, protected CGLFunctions
{
public:

    //! Ctor
    CGLRenderer(CSceneManager* pSceneManager);

    //! Dtor
    virtual ~CGLRenderer();

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IGLRendererListener
    {
        friend class CGLRenderer;

    protected:

        virtual void onNewMessage(bool bIsError, const QString& message) = 0;
    };

    //! Enregistre un listener
    void registerListener(IGLRendererListener* pListener);

    //! Libére un listener
    void unregisterListener(IGLRendererListener* pListener);

    //--------------------------------------------------------------------------------------------
    // Implémentation ARenderer
    //--------------------------------------------------------------------------------------------

    //! Initialise le renderer
    virtual bool init();

    //! Effectue le rendu de l'ensemble des objets du gestionnaire de scene
    virtual void render();

    //! Effectue le rendu d'un vertex buffer
    virtual void renderMeshBuffer(CMeshBuffer* pBuffer);

    //! Effectue le rendu d'une liste de vertex buffer
    virtual void renderMeshBuffer(const QList<CMeshBuffer*>& buffers);

    //! Retourne le nom du type
    virtual QString getTypeName() const	{ return "GLRenderer"; }

    //! Met à jour les états de rendu
    virtual void onUpdateRenderStates();


    //--------------------------------------------------------------------------------------------
    // Fin implémentation ARenderer
    //--------------------------------------------------------------------------------------------

    //! Retourne le nombre de draw call par frame
    int getDrawCallCount() const { return m_iDrawCalls; }

    //! Retourne le nombre de polygones par frame
    int getPolygonCount() const { return m_iPolygonsPerFrame; }

    //! Retourne le type du GPU
    QString getGPUName();

    //! Retourne la version de GLSL
    QString getGLSLVersion();

    //! Retourne la version d'OpenGL
    QString getOpenGLVersion();

    //! Retourne vrai si l'extension OpenGL est supportée
    bool isExtensionSupported(const QString& strExtension);


protected:

    //! Conteneur associatif qui permet de trouver un buffer GPU décrivant un item
    QHash<CMeshBuffer*, CGLMeshBuffer*> m_HardwareBuffers;

    //! Shader program
    QHash<QString, CGLShaderProgram*> m_ShaderPrograms;

    //! Conteneur de texture
    QHash<QString, AGLTexture*> m_Textures;

    //! Conteneur de buffer d'images
    QHash<CFrameBuffer*, CGLFrameBuffer*> m_FrameBuffers;

    //--------------------------------------------------------------------------------------------
    // Listeners
    //--------------------------------------------------------------------------------------------

    //! Implémentation - Interface CTextureManager::ITextureManagerListener
    virtual void onUpdateTexture(ATexture* pTexture);

    //! Implémentation - Interface CTextureManager::ITextureManagerListener
    virtual void onDeleteTexture(ATexture* pTexture);

    //! Implémentation - Interface CMeshManager::IMeshBufferListener
    virtual void onDeleteMeshBuffer(CMeshBuffer* pBuffer);

    //! Implémentation - Interface CShaderManager::IShaderManagerListener
    virtual void onUpdateShader(CShader* pShader);

    //! Implémentation - Interface CShaderManager::IShaderManagerListener
    virtual void onDeleteShader(CShader* pShader);

    //! Implémentation - Interface CSceneManager::ISceneManagerListener
    virtual void onUpdateCamera(CCamera* pCamera);

    //! Implémentation - Interface CSceneManager::ISceneManagerListener
    virtual void onUpdateAnimation(CAnimation* pAnimation);

    //! Implémentation - Interface CSceneManager::ISceneManagerListener
    virtual void onCreateSceneNode(CSceneNode* pSceneNode);

    //! Implémentation - Interface CSceneManager::ISceneManagerListener
    virtual void onUpdateSceneNode(CSceneNode* pSceneNode);

    //! Implémentation - Interface CSceneManager::ISceneManagerListener
    virtual void onDeleteSceneNode(CSceneNode* pSceneNode);

    //--------------------------------------------------------------------------------------------
    // Méthodes de rendu
    //--------------------------------------------------------------------------------------------

    //! Rendu des items
    void renderItems(const CRenderQueue& renderQueue);

    //! Rendu dans un framebuffer
    inline void renderIntoFrameBuffer(CFrameBuffer* pFrameBuffer);

    //! Rendu des meshs en fonction de leur VBO
    inline void renderVertexBufferObject(CMeshBuffer* pBuffer);

    //! Efface les buffers de rendu
    void clearBuffers(unsigned int& uiFlags);

    //! Lie les paramétres temporels
    inline void bindTime();

    //! Lie les paramétres de camera (modéle, vue, projection, normal...) au shader
    inline void bindCamera(const CCamera *pCamera);

    //! Lie les matrices du noeud au shader
    inline void bindNode(const CCamera *pCamera, CSceneNode* pNode);

    //! Lie les attributs des lumiéres au shader
    inline void bindLights(const CCamera* pCamera);

    //! Lie l'attribut d'un materiau au shader
    inline void bindMaterial(CMaterial* pMaterial);

    //! Lie un shader
    inline void bindShader(CShader* pShader);

    //! Crée un VBO en fonction d'un VertexBuffer
    void createVertexBufferObject(CMeshBuffer* pBuffer);

    //! Met à jour un VBO
    void updateVertexBufferObject(CMeshBuffer* pBuffer);

    //! Retourne une texture
    inline AGLTexture* getTexture(const ATexture* pTexture);

    //! Retourne une buffer d'images
    inline CGLFrameBuffer* getFrameBuffer(CFrameBuffer* pFrameBuffer);

    //! Delie un materiau
    inline void releaseMaterial(CMaterial* pMaterial);

    //! Envoie les informations définies par l'utilisateur au GPU
    inline void bindUserUniformValues(const CRenderPass* pPass);

    //! Envoie les informations relatives au squelette d'animation au GPU
    void bindSkeleton(ARenderableItem* pItem);

    //! Fait la correspondance entre un model de shading et un shader
    CGLShaderProgram* getShaderProgram(const CRenderPass* pPass);

    //! Ajoute une texture 2D
    void createTexture(const ATexture* pTexture);

    //! Ajoute un buffer d'images
    void createFrameBuffer(CFrameBuffer* pFrameBuffer);

private:

    //! Notifie les listeners qu'une les propriétés de la caméra ont changées
    virtual void notifyNewMessage(bool bIsError, const QString& message);

    //! Liste des listeners
    QSet<IGLRendererListener*> m_GLRendererListeners;

    //! Shader courant
    CGLShaderProgram* m_pCurrentShader;

    //! Temps
    QTime m_Time;

    //! Retourne vrai si au moins un items est visible et renderable
    inline bool isOneRenderable(const QList<ARenderableItem*>& items) const
    {
        for (ARenderableItem* pItem : items)
        {
            if (!pItem->isLocked())
            {
                if (pItem->isVisible() && pItem->isRenderable() && !pItem->getNodes().isEmpty())
                    return true;
            }
        }

        return false;
    }

    void forceGLStates(const CRenderStates& renderStates);

    int m_iDrawCalls;
    int m_iPolygonsPerFrame;
    int m_iMaterialBind;
    int m_iShaderBind;

    CGLContext m_PrincipalContext;
    CGLContext* m_pSharedContext;

    QMap<EnumMaterialParameter, int> m_MaterialParameterCount;

    void checkError(const QString& message);


#ifdef MULTITHREAD_RENDERING

public:
    class CGLGeometryInstancer : public QThread
    {
    public:
        CGLGeometryInstancer();

        void init(CGLRenderer* pRenderer, CGLContext* pPrincipalContext);

        void stop();

        virtual ~CGLGeometryInstancer();

        void addMeshBuffer(CGLMeshBuffer* pGLMeshBuffer, CMeshBuffer* pBuffer);

        CGLContext* getOpenGLContext() { return m_pSharedContext; }

        QSurface* getSurface() { return m_pSurface; }

    protected:

        virtual void run();

        QPair<CGLMeshBuffer*, CMeshBuffer*> getAnAvailableMeshBuffer();
        QList<QPair<CGLMeshBuffer*, CMeshBuffer*> > m_MeshBufferList;
        static const int s_iWaitTime = 10;
        CGLContext* m_pSharedContext;
        QSurface* m_pSurface;
        CGLRenderer* m_pRenderer;
        bool m_bRun;
        mutable QMutex m_Mutex;
    };

private:
    CGLGeometryInstancer m_GeometryInstancer;

#endif // MULTITHREAD_RENDERING
};



#endif // OGLRENDERER_H




