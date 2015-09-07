#ifndef ARENDERER_H_
#define ARENDERER_H_

// Lib
#include "lib_core_global.h"

// Foundations
#include "ARenderableItem.h"
#include "AEntity.h"
#include "AWashable.h"
#include "CGeometryGlobal.h"
#include "CSceneManager.h"
#include "CMeshManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"


// Qt
#include <QVector4D>
#include <QDebug>


//////////////////////////////////////////////////////
/// \class ARenderer
/// \brief Interface pour les renderers
//////////////////////////////////////////////////////
class LIB_CORE_SHARED_EXPORT ARenderer 
    : public AEntity
    , public AWashable
    , public CShaderManager::IShaderManagerListener
    , public CMeshManager::IMeshBufferListener
    , public CSceneManager::ISceneManagerListener
    , public CTextureManager::ITextureManagerListener
{
public:

	//! Constructeur
    ARenderer(CSceneManager* pSceneManager)
		: AEntity("Renderer")
        , m_isInitialized(false)
		, m_pSceneManager(pSceneManager)
		, m_bEnabled(false)
		, m_eRasterizationMode(eRasterizationFill)
        , m_iWidth(DefaultResolutionWidth)
        , m_iHeight(DefaultResolutionHeight)
        , m_bFrustumCullingEnabled(true)
        , m_pCurrentCamera(0)
	{
	}

	//! Destructeur
    virtual ~ARenderer() {}

	//! Effectue le rendu de l'ensemble des objets du gestionnaire de scene
    virtual void render() = 0;

	//! Initialise le renderer
    inline virtual bool init() = 0;

	//! Effectue le rendu d'un vertexbuffer
	virtual void renderMeshBuffer(CMeshBuffer* pBuffer) = 0;

	//! Effectue le rendu d'une liste de vertex buffer
	virtual void renderMeshBuffer(const QList<CMeshBuffer*>& buffers) = 0;

    //! Définit la résolution
    virtual void setResolution(int iWidth, int iHeight)                 { m_iWidth = iWidth; m_iHeight = iHeight; setDirty(); }

    //! Définit la couleur du fond
    void setBackgroundColor(const QVector4D& color)                     { m_BackgroundColor = color; }

    //! Retourne la largeur
    int getWidth() const                                                { return m_iWidth; }

    //! Retourne la hauteur
    int getHeight() const                                               { return m_iHeight; }

	//! Démarre le renderer
    void run()                                                          { m_bEnabled = true; }

    //! Arrête le renderer
    void stop()                                                         { m_bEnabled = false; }

    //! Définit l'état du renderer
    void setEnabled(bool bEnabled)                                      { m_bEnabled = bEnabled; }

	//! Retourne true si est démarré
    bool isEnabled() const												{ return m_bEnabled; }
    
	//! Retourne true si le renderer est initialisé
    virtual bool isInit() const											{ return m_isInitialized; }

	//! Accesseur sur le gestionnaire de scene
	CSceneManager* getSceneManager()									{ return m_pSceneManager; }

	//! Invalide le gestionnaire de scene
	virtual void invalidSceneManager()									{ m_pSceneManager = 0; }

	//! Accesseur sur les états
	CRenderStates& renderStates()										{ return m_RequestRenderStates; }

	//! Accesseur sur les états
    void setRenderStates(const CRenderStates& states)					{ m_RequestRenderStates = states; onUpdateRenderStates(); }

	//! Définit le mode de rasterization
	void setRasterizationMode(EnumRasterizationMode eRasterizationMode)	{ m_eRasterizationMode = eRasterizationMode; }
	
	//! Retourne le mode de rasterization
	EnumRasterizationMode getRasterizationMode() const					{ return m_eRasterizationMode; }

    //! Active ou desactive le frustum culling.
    void setFrustumCullingEnabled(bool bEnabled)                        { m_bFrustumCullingEnabled = bEnabled; }

    //! Active ou desactive le frustum culling.
    bool isFrustumCullingEnabled() const                                { return m_bFrustumCullingEnabled; }

    //! Met à jour les états de rendu
    virtual void onUpdateRenderStates() = 0;

    //! Définit la caméra utilisée
    void setCurrentCamera(CCamera* pCamera)                             { m_pCurrentCamera = pCamera; }

    //! Retourne la caméra utilisée
    CCamera* getCurrentCamera()                                         { return m_pCurrentCamera; }

    //! Retourne la caméra utilisée
    const CCamera* getCurrentCamera() const                             { return m_pCurrentCamera; }

protected:   

	//! True si initialisé
	bool m_isInitialized;

	//! Pointeur sur le gestionnaire de scene
	CSceneManager* m_pSceneManager;

	//! Si vrai, effectue le rendu
	bool m_bEnabled;

    //! Couleur d'effacement
    QVector4D m_BackgroundColor;

	//! Etat en cours
	CRenderStates m_CurrentRenderStates;

	//! Etat désiré
	CRenderStates m_RequestRenderStates;

	//! Mode de rasterization
	EnumRasterizationMode m_eRasterizationMode;

    //! Largeur
    int m_iWidth;

    //! Hauteur
    int m_iHeight;

    //! Si vrai, effectue le frustum culling
    bool m_bFrustumCullingEnabled;

    //! Caméra utilisée
    CCamera* m_pCurrentCamera;
};

bool ARenderer::init()
{
    m_isInitialized = true;
    return true;
}

#endif //ARENDERER_H_

