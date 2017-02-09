#include "CTextureManager.h"


//-----------------------------------------------------------------------------------------
CTextureManager::CTextureManager()
{

}

//-----------------------------------------------------------------------------------------
CTextureManager &CTextureManager::getInstance()
{
    static CTextureManager instance;
    return instance;
}

//-----------------------------------------------------------------------------------------
CTextureManager::~CTextureManager()
{
    clearTextures();
}

//-----------------------------------------------------------------------------------------
// Listeners
//-----------------------------------------------------------------------------------------
void CTextureManager::registerListener(ITextureManagerListener* pListener)
{
    m_TextureManagerListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CTextureManager::unregisterListener(ITextureManagerListener* pListener)
{
    m_TextureManagerListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CTextureManager::onUpdate(ATexture* pTexture)
{
    notifyUpdate(pTexture);
}

//-----------------------------------------------------------------------------------------
void CTextureManager::onDelete(ATexture* pTexture)
{
    notifyDelete(pTexture);
}

//-----------------------------------------------------------------------------------------
ATexture* CTextureManager::getTextureByName(const QString& name) const
{ 
    if (m_TextureByName.contains(name))
    {
        return m_TextureByName.value(name);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
ATexture* CTextureManager::getTextureByID(int iID) const
{ 
    if (m_TextureByID.contains(iID))
    {
        return m_TextureByID.value(iID);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CTexture2D* CTextureManager::createTexture2D(const QString& name)
{
    LogManager.addMessage(eDEBUG, "TextureManager: create texture 2D : " + name);
    CTexture2D* pTexture = new CTexture2D(name);
    m_TextureByName[pTexture->getName()] = pTexture;
    m_TextureByID[pTexture->getID()] = pTexture;
    pTexture->registerListener(this);
    notifyUpdate(pTexture);
    return pTexture;
}

//-----------------------------------------------------------------------------------------
CTexture2D* CTextureManager::createTexture2D(const QString& name, const QSize& size)
{
    LogManager.addMessage(eDEBUG, "TextureManager: create texture 2D : " + name);
    CTexture2D* pTexture = new CTexture2D(name, size);
    m_TextureByName[pTexture->getName()] = pTexture;
    m_TextureByID[pTexture->getID()] = pTexture;
    pTexture->registerListener(this);
    notifyUpdate(pTexture);
    return pTexture;
}

//-----------------------------------------------------------------------------------------
CTexture2D* CTextureManager::createTexture2D(const QString& name, const QString& textureFileName)
{
    if (!textureFileName.isEmpty())
    {
        LogManager.addMessage(eDEBUG, "TextureManager create texture2D : " + name);
        CTexture2D* pTexture = new CTexture2D(name, textureFileName);
        m_TextureByName[pTexture->getName()] = pTexture;
        m_TextureByID[pTexture->getID()] = pTexture;
        pTexture->registerListener(this);
        notifyUpdate(pTexture);
        return pTexture;
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CTextureTarget* CTextureManager::createTextureTarget(const QString& name, const QSize& size, EnumInternalFormat eFormat)
{
    LogManager.addMessage(eDEBUG, "TextureManager: create texture target : " + name);
    CTextureTarget* pTexture = new CTextureTarget(name, size, eFormat);
    m_TextureByName[pTexture->getName()] = pTexture;
    m_TextureByID[pTexture->getID()] = pTexture;
    pTexture->registerListener(this);
    notifyUpdate(pTexture);
    return pTexture;
}

//-----------------------------------------------------------------------------------------
CTextureCube* CTextureManager::createTextureCube(const QString& name, const QStringList& textureFileNames)
{
    if (!name.isEmpty() && textureFileNames.size() > 0)
    {
        LogManager.addMessage(eDEBUG, "TextureManager: create texture cube : " + name);
        CTextureCube* pTexture = new CTextureCube(name, textureFileNames);
        m_TextureByName[pTexture->getName()] = pTexture;
        m_TextureByID[pTexture->getID()] = pTexture;
        pTexture->registerListener(this);
        notifyUpdate(pTexture);
        return pTexture;
    }
    return 0;
}

//-----------------------------------------------------------------------------------------
CTextureCube* CTextureManager::createTextureCube(
        const QString& name,
        const QString& fileNamePosX,
        const QString& fileNameNegX,
        const QString& fileNamePosY,
        const QString& fileNameNegY,
        const QString& fileNamePosZ,
        const QString& fileNameNegZ)
{
    return CTextureManager::createTextureCube(name, QStringList()
                                              << fileNamePosX
                                              << fileNameNegX
                                              << fileNamePosY
                                              << fileNameNegY
                                              << fileNamePosZ
                                              << fileNameNegZ);
}

//-----------------------------------------------------------------------------------------
void CTextureManager::removeTexture(ATexture* pTexture)
{
    pTexture->unregisterListener(this);
    notifyDelete(pTexture);
    m_TextureByName.remove(pTexture->getName());
    m_TextureByID.remove(pTexture->getID());
    delete pTexture;
}

//-----------------------------------------------------------------------------------------
void CTextureManager::clearTextures()
{
    for (ATexture* pTexture : m_TextureByID)
    {
        pTexture->unregisterListener(this);
        notifyDelete(pTexture);
        delete pTexture;
    }

    m_TextureByName.clear();
    m_TextureByID.clear();
}

//-----------------------------------------------------------------------------------------
bool CTextureManager::isTextureExist(const QString& name) const
{
    return m_TextureByName.contains(name);
}

//-----------------------------------------------------------------------------------------
bool CTextureManager::isTextureExist(int iID) const
{
    return m_TextureByID.contains(iID);
}

//-----------------------------------------------------------------------------------------
void CTextureManager::notifyUpdate(ATexture* pTexture)
{
    for (ITextureManagerListener* pListener : m_TextureManagerListeners)
    {
        pListener->onUpdateTexture(pTexture);
    }
}

//-----------------------------------------------------------------------------------------
void CTextureManager::notifyDelete(ATexture* pTexture)
{
    for (ITextureManagerListener* pListener : m_TextureManagerListeners)
    {
        pListener->onDeleteTexture(pTexture);
    }
}


