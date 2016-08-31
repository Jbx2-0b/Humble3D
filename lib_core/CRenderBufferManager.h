#ifndef CRENDERBUFFERMANAGER_H
#define CRENDERBUFFERMANAGER_H


// Lib
#include "lib_core_global.h"

// Foundations
#include "CRenderBuffer.h"
#include "CGeometryGlobal.h"

// Qt
#include <QMutex>
#include <QMutexLocker>

class LIB_CORE_SHARED_EXPORT CRenderBufferManager : public CRenderBuffer::IRenderBufferListener
{
public:

    //! Accesseur a l'instance de la classe.
    static CRenderBufferManager* getInstance()
    {
        if (s_pInstance == 0)
        {
            QMutexLocker locker(&s_Mutex);

            if (s_pInstance == 0)
            {
                s_pInstance = new CRenderBufferManager();
            }
        }

        return s_pInstance;
    }

    //! Destructeur de l'instance de la classe.
    static void killInstance()
    {
        if (s_pInstance)
        {
            QMutexLocker locker(&s_Mutex);
            delete s_pInstance;
            s_pInstance = 0;
        }
    }

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class IRenderBufferManagerListener
    {
    public:

        //! Prévient de l'ajout ou de la mise à jour d'un render buffer
        virtual void onUpdateRenderBuffer(CRenderBuffer* pRenderBuffer) = 0;

        //! Prévient de la suppression d'un render buffer
        virtual void onDeleteRenderBuffer(CRenderBuffer* pRenderBuffer) = 0;
    };

    //! Enregistre un listener
    void registerListener(IRenderBufferManagerListener* pListener);

    //! Libére un listener
    void unregisterListener(IRenderBufferManagerListener* pListener);

    //! Retourne le nombre de RenderBuffers
    int getRenderBufferCount() const                                        { return m_RenderBuffers.size(); }

    //! Retourne la collection de RenderBuffers
    const QList<CRenderBuffer*> getRenderBuffers() const								{ return m_RenderBuffers.values(); }

    //! Retourne la collection des de RenderBuffers disponible
    const QList<QString> getRenderBuffersNames() const							{ return m_RenderBuffers.keys(); }

    //! Retourne une RenderBuffer en fonction de son nom
    CRenderBuffer* getRenderBufferByName(const QString& name) const;

    //! Retourne une RenderBuffer en fonction de son nom
    CRenderBuffer* getRenderBufferByID(int iID) const;

    //! Retourne true si la RenderBuffer existe
    bool isRenderBufferExist(const QString& name) const;

    //! Retourne true si la RenderBuffer existe
    bool isRenderBufferExist(int iID) const;

    //! Crée une nouvelle RenderBuffer et retourne son pointeur
    CRenderBuffer* createRenderBuffer(const QString& name, EnumInternalFormat eFormat);

    //! Supprime une RenderBuffer
    void removeRenderBuffer(CRenderBuffer* pRenderBuffer);

    //! Libére l'ensemble des RenderBuffers du manager
    void clearRenderBuffers();


protected:

    //! Implémentation de CRenderBuffer::IRenderBufferListener
    virtual void onUpdate(CRenderBuffer* pRenderBuffer);

    //! Implémentation de CRenderBuffer::IRenderBufferListener
    virtual void onDelete(CRenderBuffer* pRenderBuffer);

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    //! Liste des listeners
    QSet<IRenderBufferManagerListener*> m_RenderBufferManagerListeners;

    //! Notifie les listeners qu'un RenderBuffer a été ajouté
    void notifyUpdate(CRenderBuffer* pRenderBuffer);

    //! Notifie les listeners qu'un RenderBuffer a été supprimé
    void notifyDelete(CRenderBuffer* pRenderBuffer);

private:

    //! Constructeur
    CRenderBufferManager();

    //! Destructeur
    virtual ~CRenderBufferManager();

    //! Conteneur des RenderBuffers
    QMap<QString, CRenderBuffer*> m_RenderBuffers;

    //! Conteneur des RenderBuffers
    QMap<int, CRenderBuffer*> m_RenderBufferIDs;

    static CRenderBufferManager* s_pInstance;
    static QMutex s_Mutex;
};

#endif // CRENDERBUFFERMANAGER_H
