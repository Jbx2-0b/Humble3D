#ifndef CFRAMEBUFFER_H
#define CFRAMEBUFFER_H

// Foundations
#include "AEntity.h"
#include "CTextureManager.h"
#include "CRenderBufferManager.h"

// Qt
#include <QList>
#include <QHash>

class LIB_CORE_SHARED_EXPORT CFrameBuffer : public AEntity
{
public:

    //! Constructeur
    CFrameBuffer(int iWidth, int iHeight)
        : AEntity("FrameBuffer")
        , m_iWidth(iWidth)
        , m_iHeight(iHeight)
    {
    }

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "FrameBuffer"; }

    //! Retourne la largeur
    int getWidth() const { return m_iWidth; }

    //! Retourne la hauteur
    int getHeight() const { return m_iHeight; }

    void setDrawBuffers(const QList<EnumDrawBuffer>& drawBuffers)
    {
        m_DrawBuffers = drawBuffers;
    }

    void setDrawBuffer(EnumDrawBuffer eDrawBuffer)
    {
        m_DrawBuffers.clear();
        m_DrawBuffers << eDrawBuffer;
    }

    const QList<EnumDrawBuffer>& getDrawBuffers() const
    {
        return m_DrawBuffers;
    }

    ATexture* createRenderTexture(EnumAttachment eAttachment, EnumInternalFormat eInternalFormat)
    {
        if (m_TextureTargets.contains(eAttachment))
        {
            CTextureManager::getInstance()->removeTexture(m_TextureTargets[eAttachment]);
        }

        if (m_RenderBuffers.contains(eAttachment))
        {
            CRenderBufferManager::getInstance()->removeRenderBuffer(m_RenderBuffers[eAttachment]);
        }

        CTextureTarget* pTexture = CTextureManager::getInstance()->createTextureTarget("RenderTexture", QSize(m_iWidth, m_iHeight), eInternalFormat);
        m_TextureTargets[eAttachment] = pTexture;

        return pTexture;
    }

    CRenderBuffer* createRenderBuffer(EnumAttachment eAttachment, EnumInternalFormat eInternalFormat)
    {
        if (m_TextureTargets.contains(eAttachment))
        {
            CTextureManager::getInstance()->removeTexture(m_TextureTargets[eAttachment]);
        }

        if (m_RenderBuffers.contains(eAttachment))
        {
            CRenderBufferManager::getInstance()->removeRenderBuffer(m_RenderBuffers[eAttachment]);
        }

        CRenderBuffer* pRenderBuffer = CRenderBufferManager::getInstance()->createRenderBuffer("RenderBuffer", eInternalFormat);
        m_RenderBuffers[eAttachment] = pRenderBuffer;

        return pRenderBuffer;
    }

    const QHash<EnumAttachment, CTextureTarget*>& getTextureTargets() const { return m_TextureTargets; }
    const QHash<EnumAttachment, CRenderBuffer*>& getRenderBuffers() const { return m_RenderBuffers; }

private:


    int m_iWidth, m_iHeight;
    QHash<EnumAttachment, CTextureTarget*> m_TextureTargets;
    QHash<EnumAttachment, CRenderBuffer*> m_RenderBuffers;

    QList<EnumDrawBuffer> m_DrawBuffers;
};

#endif // CFRAMEBUFFER_H
