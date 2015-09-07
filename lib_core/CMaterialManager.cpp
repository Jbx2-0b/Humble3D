#include "CMaterialManager.h"

CMaterialManager* CMaterialManager::s_pInstance = 0;
QMutex CMaterialManager::s_Mutex(QMutex::Recursive);

//-----------------------------------------------------------------------------------------
CMaterialManager::CMaterialManager()
{
}

//-----------------------------------------------------------------------------------------
CMaterialManager::~CMaterialManager()
{
    clearMaterials();
}

//-----------------------------------------------------------------------------------------
// Listeners
//-----------------------------------------------------------------------------------------
void CMaterialManager::registerListener(IMaterialManagerListener* pListener)
{
    m_MaterialManagerListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CMaterialManager::unregisterListener(IMaterialManagerListener* pListener)
{
    m_MaterialManagerListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CMaterialManager::onUpdate(CMaterial* pMaterial)
{
    notifyUpdate(pMaterial);
}

//-----------------------------------------------------------------------------------------
void CMaterialManager::onDelete(CMaterial* /*pMaterial*/)
{
    // Nothing to do
}

//-----------------------------------------------------------------------------------------
CMaterial* CMaterialManager::getMaterialByName(const QString& name) const
{ 
    if (m_Materials.contains(name))
    {
        return m_Materials.value(name);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CMaterial* CMaterialManager::getMaterialByID(int iID) const
{ 
    if (m_MaterialIDs.contains(iID))
    {
        return m_MaterialIDs.value(iID);
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
CMaterial* CMaterialManager::createMaterial(const QString& name)
{
    CMaterial* pMaterial = new CMaterial(name);
    m_Materials[pMaterial->getName()] = pMaterial;
    m_MaterialIDs[pMaterial->getID()] = pMaterial;
    pMaterial->registerListener(this);
    notifyUpdate(pMaterial);
    return pMaterial;
}

//-----------------------------------------------------------------------------------------
void CMaterialManager::removeMaterial(CMaterial* pMaterial)
{
    pMaterial->unregisterListener(this);
    notifyDelete(pMaterial);
    m_Materials.remove(pMaterial->getName());
    m_MaterialIDs.remove(pMaterial->getID());
    delete pMaterial;
}

//-----------------------------------------------------------------------------------------
void CMaterialManager::clearMaterials()
{
    foreach (CMaterial* pMaterial, m_Materials)
    {
        removeMaterial(pMaterial);
    }
}

//-----------------------------------------------------------------------------------------
void CMaterialManager::initColorMaterials()
{
    static bool bAlreadyInit = false;

    if (!bAlreadyInit)
    {
        foreach (const Color& color, Color::getColorList())
        {
            CMaterial* pMat = createMaterial(color.toString());

            pMat->getRenderPass(0)->setShaderName("color");
            pMat->setAmbientColor(color.toVector4D());
        }

        bAlreadyInit = true;
    }
}

//-----------------------------------------------------------------------------------------
QString CMaterialManager::getMaterialNameByColor(const Color& color)
{
    initColorMaterials();
    if (CMaterial* pMat = getMaterialByName(color.toString()))
    {
        pMat->getRenderPass(0)->setShaderName("color");
        pMat->setAmbientColor(color.toVector4D());

        return pMat->getName();
    }
    return "";
}
//-----------------------------------------------------------------------------------------
bool CMaterialManager::isMaterialExist(const QString& name) const
{
    return m_Materials.contains(name);
}

//-----------------------------------------------------------------------------------------
bool CMaterialManager::isMaterialExist(int iID) const
{
    return m_MaterialIDs.contains(iID);
}

//-----------------------------------------------------------------------------------------
void CMaterialManager::notifyUpdate(CMaterial* pMaterial)
{
    foreach (IMaterialManagerListener* pListener, m_MaterialManagerListeners)
    {
        pListener->onUpdate(pMaterial);
    }
}

//-----------------------------------------------------------------------------------------
void CMaterialManager::notifyDelete(CMaterial* pMaterial)
{
    foreach (IMaterialManagerListener* pListener, m_MaterialManagerListeners)
    {
        pListener->onDelete(pMaterial);
    }
}
