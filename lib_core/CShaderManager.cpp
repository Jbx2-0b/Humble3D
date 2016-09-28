#include "CShaderManager.h"

//-----------------------------------------------------------------------------------------
CShaderManager::CShaderManager()
{

}

//-----------------------------------------------------------------------------------------
CShaderManager &CShaderManager::getInstance()
{
    static CShaderManager instance;
    return instance;
}

//-----------------------------------------------------------------------------------------
CShaderManager::~CShaderManager()
{
    clearShaders();
}

//-----------------------------------------------------------------------------------------
// Listeners
//-----------------------------------------------------------------------------------------
void CShaderManager::registerListener(IShaderManagerListener* pListener)
{
    m_ShaderManagerListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CShaderManager::unregisterListener(IShaderManagerListener* pListener)
{
    m_ShaderManagerListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CShaderManager::onUpdate(CShader* pShader)
{
    notifyUpdate(pShader);
}

//-----------------------------------------------------------------------------------------
void CShaderManager::onDelete(CShader* pShader)
{
    notifyDelete(pShader);
}

//-----------------------------------------------------------------------------------------
CShader* CShaderManager::getShaderByName(const QString& name) const
{
    if (m_Shaders.contains(name))
    {
        return m_Shaders.value(name);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CShader* CShaderManager::getShaderByID(int iID) const
{
    if (m_ShaderIDs.contains(iID))
    {
        return m_ShaderIDs.value(iID);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CShader* CShaderManager::createShader(
        const QString& name,
        const QString& vertexShaderFileName,
        const QString& geometryShaderFileName,
        const QString& fragmentShaderFileName)
{
    LogManager.addMessage(eDEBUG, "ShaderManager: create shader " + name);
    CShader* pShader = new CShader(name, vertexShaderFileName, geometryShaderFileName, fragmentShaderFileName);
    m_Shaders[pShader->getName()] = pShader;
    m_ShaderIDs[pShader->getID()] = pShader;
    pShader->registerListener(this);
    notifyUpdate(pShader);
    return pShader;
}

//-----------------------------------------------------------------------------------------
CShader* CShaderManager::createShaderFromSourceCode(
        const QString& name,
        const QString& vertexShaderFileCode,
        const QString& geometryShaderFileCode,
        const QString& fragmentShaderFileCode)
{
    LogManager.addMessage(eDEBUG, "ShaderManager: create shader " + name);
    CShader* pShader = new CShader(name);
    pShader->setVertexShaderCode(vertexShaderFileCode);
    pShader->setGeometryShaderCode(geometryShaderFileCode);
    pShader->setFragmentShaderCode(fragmentShaderFileCode);
    m_Shaders[pShader->getName()] = pShader;
    m_ShaderIDs[pShader->getID()] = pShader;
    pShader->registerListener(this);
    notifyUpdate(pShader);
    return pShader;
}

//-----------------------------------------------------------------------------------------
void CShaderManager::removeShader(CShader* pShader)
{
    pShader->unregisterListener(this);
    notifyDelete(pShader);
    m_Shaders.remove(pShader->getName());
    m_ShaderIDs.remove(pShader->getID());
    delete pShader;
}

//-----------------------------------------------------------------------------------------
void CShaderManager::clearShaders()
{
    foreach (CShader* pShader, m_Shaders)
    {
        removeShader(pShader);
    }
}

//-----------------------------------------------------------------------------------------
bool CShaderManager::isShaderExist(const QString& name) const
{
    return m_Shaders.contains(name);
}

//-----------------------------------------------------------------------------------------
bool CShaderManager::isShaderExist(int iID) const
{
    return m_ShaderIDs.contains(iID);
}

//-----------------------------------------------------------------------------------------
void CShaderManager::notifyUpdate(CShader* pShader)
{
    foreach (IShaderManagerListener* pListener, m_ShaderManagerListeners)
    {
        pListener->onUpdateShader(pShader);
    }
}

//-----------------------------------------------------------------------------------------
void CShaderManager::notifyDelete(CShader* pShader)
{
    foreach (IShaderManagerListener* pListener, m_ShaderManagerListeners)
    {
        pListener->onDeleteShader(pShader);
    }
}
