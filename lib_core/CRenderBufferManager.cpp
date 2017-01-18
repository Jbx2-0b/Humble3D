#include "CRenderBufferManager.h"


//-----------------------------------------------------------------------------------------
CRenderBufferManager::CRenderBufferManager()
{

}

//-----------------------------------------------------------------------------------------
CRenderBufferManager &CRenderBufferManager::getInstance()
{
    static CRenderBufferManager instance;
    return instance;
}

//-----------------------------------------------------------------------------------------
CRenderBufferManager::~CRenderBufferManager()
{
    clearRenderBuffers();
}

//-----------------------------------------------------------------------------------------
// Listeners
//-----------------------------------------------------------------------------------------
void CRenderBufferManager::registerListener(IRenderBufferManagerListener* pListener)
{
    m_RenderBufferManagerListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CRenderBufferManager::unregisterListener(IRenderBufferManagerListener* pListener)
{
    m_RenderBufferManagerListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CRenderBufferManager::onUpdate(CRenderBuffer* pRenderBuffer)
{
    notifyUpdate(pRenderBuffer);
}

//-----------------------------------------------------------------------------------------
void CRenderBufferManager::onDelete(CRenderBuffer* pRenderBuffer)
{
    notifyDelete(pRenderBuffer);
}

//-----------------------------------------------------------------------------------------
CRenderBuffer* CRenderBufferManager::getRenderBufferByName(const QString& name) const
{
    if (m_RenderBuffers.contains(name))
    {
        return m_RenderBuffers.value(name);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CRenderBuffer* CRenderBufferManager::getRenderBufferByID(int iID) const
{
    if (m_RenderBufferIDs.contains(iID))
    {
        return m_RenderBufferIDs.value(iID);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CRenderBuffer* CRenderBufferManager::createRenderBuffer(const QString& name, EnumInternalFormat eFormat)
{
    LogManager.addMessage(eDEBUG, "CRenderBufferManager::createRenderBuffer() : " + name);
    CRenderBuffer* pRenderBuffer = new CRenderBuffer(name, eFormat);
    m_RenderBuffers[pRenderBuffer->getName()] = pRenderBuffer;
    m_RenderBufferIDs[pRenderBuffer->getID()] = pRenderBuffer;
    pRenderBuffer->registerListener(this);
    notifyUpdate(pRenderBuffer);
    return pRenderBuffer;
}

//-----------------------------------------------------------------------------------------
void CRenderBufferManager::removeRenderBuffer(CRenderBuffer* pRenderBuffer)
{
    pRenderBuffer->unregisterListener(this);
    notifyDelete(pRenderBuffer);
    m_RenderBuffers.remove(pRenderBuffer->getName());
    m_RenderBufferIDs.remove(pRenderBuffer->getID());
    delete pRenderBuffer;
}

//-----------------------------------------------------------------------------------------
void CRenderBufferManager::clearRenderBuffers()
{
    for (CRenderBuffer* pRenderBuffer : m_RenderBuffers)
    {
        removeRenderBuffer(pRenderBuffer);
    }
}

//-----------------------------------------------------------------------------------------
bool CRenderBufferManager::isRenderBufferExist(const QString& name) const
{
    return m_RenderBuffers.contains(name);
}

//-----------------------------------------------------------------------------------------
bool CRenderBufferManager::isRenderBufferExist(int iID) const
{
    return m_RenderBufferIDs.contains(iID);
}

//-----------------------------------------------------------------------------------------
void CRenderBufferManager::notifyUpdate(CRenderBuffer* pRenderBuffer)
{
    for (IRenderBufferManagerListener* pListener : m_RenderBufferManagerListeners)
    {
        pListener->onUpdateRenderBuffer(pRenderBuffer);
    }
}

//-----------------------------------------------------------------------------------------
void CRenderBufferManager::notifyDelete(CRenderBuffer* pRenderBuffer)
{
    for (IRenderBufferManagerListener* pListener : m_RenderBufferManagerListeners)
    {
        pListener->onDeleteRenderBuffer(pRenderBuffer);
    }
}
