#ifndef CRENDERBUFFER_H
#define CRENDERBUFFER_H

#include "AEntity.h"
#include "CGeometryGlobal.h"

class LIB_CORE_SHARED_EXPORT CRenderBuffer : public AEntity
{

public:

    CRenderBuffer(const QString& name, EnumInternalFormat eFormat)
        : AEntity(name)
        , m_eFormat(eFormat)
    {
    }

    //! Retourne le nom du type
    virtual QString getTypeName() const					{ return "RenderBuffer"; }

    // Retourne le format
    EnumInternalFormat getInternalFormat() const        { return m_eFormat; }

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IRenderBufferListener
    {
        friend class CRenderBuffer;

    protected:
        virtual void onDelete(CRenderBuffer* pRenderBuffer) = 0;
        virtual void onUpdate(CRenderBuffer* pRenderBuffer) = 0;
    };

    //! Enregistre un listener
    void registerListener(IRenderBufferListener* pListener)
    {
        m_RenderBufferListeners.insert(pListener);
    }

    //! Libére un listener
    void unregisterListener(IRenderBufferListener* pListener)
    {
        m_RenderBufferListeners.remove(pListener);
    }

protected:

    EnumInternalFormat m_eFormat;

    //! Transmet le message de mise à jour
    virtual void notifyUpdate()
    {
        if (m_bNotificationsEnabled)
        {
            foreach (IRenderBufferListener* pListener, m_RenderBufferListeners)
            {
                pListener->onUpdate(this);
            }

            AEntity::notifyUpdate();
        }
    }

    //! Transmet le message de suppression
    virtual void notifyDelete()
    {
        if (m_bNotificationsEnabled)
        {
            foreach (IRenderBufferListener* pListener, m_RenderBufferListeners)
            {
                pListener->onDelete(this);
            }

            AEntity::notifyDelete();
        }
    }

    //! Liste des listeners
    QSet<IRenderBufferListener*> m_RenderBufferListeners;
};

#endif // CRENDERBUFFER_H
