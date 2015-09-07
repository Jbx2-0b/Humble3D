#include "ARenderableItem.h"
#include "CMaterialManager.h"
#include "COctree.h"

//-----------------------------------------------------------------------------------------
ARenderableItem::ARenderableItem(CSceneManager* pSceneManager, const QString& name)
: ASceneNodeItem(pSceneManager, name)
, m_bIsRenderingInit(false) {}

//-----------------------------------------------------------------------------------------
void ARenderableItem::setMaterialName(const QString& materialName)
{
    m_MaterialName = materialName;
    notifyMaterialChanged();
}

//-----------------------------------------------------------------------------------------
const QString& ARenderableItem::getMaterialName() const
{
    return m_MaterialName;
}

//-----------------------------------------------------------------------------------------
bool ARenderableItem::hasMaterial() const
{
    return CMaterialManager::getInstance()->isMaterialExist(m_MaterialName);
}
