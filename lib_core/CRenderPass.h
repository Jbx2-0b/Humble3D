#ifndef CRENDERPASS_H
#define CRENDERPASS_H


// Lib
#include "lib_core_global.h"

// Fundations
#include "AEntity.h"
#include "CGeometryGlobal.h"
#include "CRenderStates.h"
#include "CFrameBuffer.h"
#include "CCamera.h"

class CMaterial;

class CRenderOperation
{
public:

    CRenderOperation(CFrameBuffer* pSource, CFrameBuffer* pDestination, unsigned int masks, EnumFilterType eFilter)
        : m_pSource(pSource)
        , m_pDestination(pDestination)
        , m_uiMaskFlags(masks)
        , m_eFilter(eFilter)
    {}

    //! Retourne la source
    CFrameBuffer* getSource() const { return m_pSource; }

    //! Retourne la destination
    CFrameBuffer* getDestination() const { return m_pDestination; }

    //! Retourne vrai si doit effacer le buffer de couleur
    bool maskColorBuffer() const { return m_uiMaskFlags & ColorBuffer; }

    //! Retourne vrai si doit effacer le buffer de profondeur
    bool maskDepthBuffer() const { return m_uiMaskFlags & DepthBuffer; }

    //! Retourne vrai si doit effacer le buffer de stencil
    bool maskStencilBuffer() const { return m_uiMaskFlags & StencilBuffer; }

    //! Retourne les flags descrivant les buffers qui doivent être copié
    unsigned int maskFlags() const { return m_uiMaskFlags; }

    //! Retourne les flags descrivant les buffers qui doivent être copié
    unsigned int& maskFlags() { return m_uiMaskFlags; }

    //! Retourne le type de filtrage
    EnumFilterType getFilter() const { return m_eFilter; }

private:
    CFrameBuffer* m_pSource;
    CFrameBuffer* m_pDestination;
    unsigned int m_uiMaskFlags;
    EnumFilterType m_eFilter;
};

class LIB_CORE_SHARED_EXPORT CRenderPass : public AEntity
{
public:

    //! Constructeur
    CRenderPass(
            CMaterial* pMaterial,
            EnumTarget eTarget = eTargetScreen,
            const QSize& size = QSize(1024, 1024),
            const CRenderStates& states = CRenderStates());

    //! Destructeur
    virtual ~CRenderPass();

    void addPreOperation(const CRenderOperation& operation) { m_PreOperations << operation; }

    void addPostOperation(const CRenderOperation& operation) { m_PostOperations << operation; }

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Définit le mode shading utilisateur
    void setShaderName(const QString& shaderName);

    //! Définit le flag d'effacement des buffers
    void setClearBufferFlags(unsigned int uiClearBufferFlags) { m_uiClearBufferFlags = uiClearBufferFlags; }

    //! Définit la couleur pour effacer le buffer
    void setClearColor(const QVector4D& color) { m_ClearColor = color; }

    //! Définit la caméra depuis laquelle le rendu est effectué
    void setCamera(CCamera* pCamera);

    //! Accesseur sur la pass précédente
    CRenderPass* previousPass();

    //! Accesseur sur la pass suivante
    CRenderPass* nextPass();

    //! Accesseur sur la pass précédente
    const CRenderPass* previousPass() const;

    //! Accesseur sur la pass suivante
    const CRenderPass* nextPass() const;

    //! Définit la résolution du rendu
    void setSize(const QSize& size);

    //! Retourne la résolution du rendu
    QSize getSize() const {	return m_Size; }

    //! Définit le type de cible
    void setTargetType(EnumTarget eTarget);

    //! Retourne le type de cible
    EnumTarget getTargetType() const { return m_eTarget; }

    //! Retourne le buffer d'image
    CFrameBuffer* getFrameBuffer() { return m_pFrameBuffer; }

    //! Définit si doit être sauvegardé sous forme de bitmap
    void setSaveBitmapEnabled(bool bEnabled);

    //! Définit le repertoire de sauvegarde
    void setSaveBitmapFilePathName(const QString& filePathName);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le type de l'objet
    QString getTypeName() const { return "RenderingPass"; }

    //! Retourne l'index de la pass
    int getIndex() const;

    //! Retourne les états de rendu
    const CRenderStates& getRenderStates() const;

    //! Retourne les états de rendu
    CRenderStates& renderStates();

    //! Retourne le mode de shading utilisateur
    const QString& getShaderName() const;

    //! Retourne le flag d'effacement des buffers
    unsigned int getClearBufferFlags() const { return m_uiClearBufferFlags; }

    //! Retourne vrai si doit effacer le buffer de couleur
    bool clearColorBuffer() const { return m_uiClearBufferFlags & ColorBuffer; }

    //! Retourne vrai si doit effacer le buffer de profondeur
    bool clearDepthBuffer() const { return m_uiClearBufferFlags & DepthBuffer; }

    //! Retourne vrai si doit effacer le buffer de stencil
    bool clearStencilBuffer() const { return m_uiClearBufferFlags & StencilBuffer; }

    //! Retourne la couleur pour effacer le buffer
    QVector4D getClearColor() const { return m_ClearColor; }

    //! Retourne les flags descrivant quels buffers doivent être éffacés
    unsigned int clearBufferFlags() const { return m_uiClearBufferFlags; }

    //! Retourne les flags descrivant quels buffers doivent être éffacés
    unsigned int& clearBufferFlags() { return m_uiClearBufferFlags; }

    //! Retourne la caméra depuis laquelle le rendu est effectué
    const CCamera* getCamera() const { return m_pCamera; }

    const QList<CRenderOperation>& getPreOperations() const { return m_PreOperations; }

    const QList<CRenderOperation>& getPostOperations() const { return m_PostOperations; }

    //! Retourne vrai si doit être sauvegardé sous forme de bitmap
    bool isSaveBitmapEnabled() const;

    //! Retourne le repertoire de sauvegarde
    QString getSaveBitmapFilePathName() const;

    //! Affiche les propriétés sur la console
    virtual QString toString() const
    {
        QString result;
        result += "[Rendering Pass \n";
        result += QString("Index: %1\n").arg(getIndex());
        result += QString("Shader Name: %1\n").arg(m_ShaderName);
        result += QString("Clear Color Buffer: %1\n").arg(clearColorBuffer());
        result += QString("Clear Depth Buffer: %1\n").arg(clearDepthBuffer());
        result += QString("Clear Stencil Buffer: %1\n").arg(clearStencilBuffer());
        result += "]\n";
        return result;
    }

private:

    //! Vrai si doit effacer le buffer de couleur
    unsigned int m_uiClearBufferFlags;

    //! Materiau parent
    CMaterial* m_pMaterial;

    //! Etats du rendu
    CRenderStates m_RenderingStates;

    //! Mode de shading utilisateur
    QString m_ShaderName;

    //! Couleur pour effacer le buffer
    QVector4D m_ClearColor;

    //! Type de cible
    EnumTarget m_eTarget;

    //! Résolution de la cible
    QSize m_Size;

    // FBO
    CFrameBuffer* m_pFrameBuffer;

    // Camera
    CCamera* m_pCamera;

    //! Operations
    QList<CRenderOperation> m_PreOperations;

    //! Operations
    QList<CRenderOperation> m_PostOperations;

    //! Sauvegarde sous forme d'image
    bool m_bSaveBitmapEnabled;

    //! Emmplacement pour la sauvegarde
    QString m_SaveBitmapFilePathName;
};

#endif // CRENDERPASS_H
