#include "ASceneNodeItem.h"
#include "CSceneManager.h"
#include "CSceneNode.h"

//-----------------------------------------------------------------------------------------
ASceneNodeItem::ASceneNodeItem(CSceneManager* pSceneManager, const QString& name)
    : AEntity(name)
    , m_pSceneManager(pSceneManager)
    , m_bVisible(true)
    , m_bSelectable(true)
    , m_bSelected(false)
    , m_ePhysicShape(eBoxShape)
    , m_dMass(0.)
    , m_dRestitution(0)
    , m_dFriction(0.5)
    , m_dRollingFriction(0)

{
}

//-----------------------------------------------------------------------------------------
ASceneNodeItem::~ASceneNodeItem()
{
    // Previent les abonnés que l'objet va être détruit
    notifyDelete();

    // On prévient les noeuds qu'on supprime l'item
    foreach (CSceneNode* pNode, m_Nodes)
    {
        pNode->removeItem(this);
    }
}

//-----------------------------------------------------------------------------------------
// Listeners
//-----------------------------------------------------------------------------------------
void ASceneNodeItem::registerListener(ISceneNodeItemListener* pListener)
{
    m_SceneNodeListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::unregisterListener(ISceneNodeItemListener* pListener)
{
    m_SceneNodeListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::notifyUpdate()
{
    if (m_bNotificationsEnabled)
    {
        foreach (ISceneNodeItemListener* pListener, m_SceneNodeListeners)
        {
            pListener->onUpdate(this);
        }

        AEntity::notifyUpdate();
    }
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::notifyDelete()
{
    if (m_bNotificationsEnabled)
    {
        foreach (ISceneNodeItemListener* pListener, m_SceneNodeListeners)
        {
            pListener->onDelete(this);
        }

        AEntity::notifyDelete();
    }
}

//-----------------------------------------------------------------------------------------
// Setters
//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setVisible(bool bVisible)
{
    m_bVisible = bVisible;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setSelectable(bool bSelectable)
{
    m_bSelectable = bSelectable;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setSelected(bool bSelected)
{
    if (m_bSelectable)
    {
        m_bSelected = bSelected;
        notifyUpdate();
    }
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setPhysicShape(EnumPhysicShape ePhysicShape)
{
    m_ePhysicShape = ePhysicShape;
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setMass(real dMass)
{
    m_dMass = dMass;
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setRestitution(real dRestitution)
{
    m_dRestitution = dRestitution;
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setFriction(real dFriction)
{
    m_dFriction = dFriction;
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setRollingFriction(real dRollingFriction)
{
    m_dRollingFriction = dRollingFriction;
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setLinearVelocity(const QVector3D& linearVelocity)
{
    m_LinearVelocity = linearVelocity;
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::setAngularVelocity(const QVector3D& angularVelocity)
{
    m_AngularVelocity = angularVelocity;
}

//-----------------------------------------------------------------------------------------
// Getters
//-----------------------------------------------------------------------------------------
CSceneManager* ASceneNodeItem::getSceneManager() const
{ 
    return m_pSceneManager;
}

//-----------------------------------------------------------------------------------------
bool ASceneNodeItem::isVisible() const
{
    return m_bVisible;
}

//-----------------------------------------------------------------------------------------
bool ASceneNodeItem::isSelectable() const
{
    return m_bSelectable;
}

//-----------------------------------------------------------------------------------------
bool ASceneNodeItem::isSelected() const
{
    return m_bSelected;
}

//-----------------------------------------------------------------------------------------
const QSet<CSceneNode*>& ASceneNodeItem::getNodes() const
{
    return m_Nodes;
}

//-----------------------------------------------------------------------------------------
unsigned int ASceneNodeItem::getNodeCount() const
{
    return getNodes().count();
}

//-----------------------------------------------------------------------------------------
CSceneNode* ASceneNodeItem::getNode() const
{
    return getNodes().size() ? *getNodes().constBegin() : 0;
}

//-----------------------------------------------------------------------------------------
EnumPhysicShape ASceneNodeItem::getPhysicShape() const
{
    return m_ePhysicShape;
}

//-----------------------------------------------------------------------------------------
real ASceneNodeItem::getMass() const
{
    return m_dMass;
}

//-----------------------------------------------------------------------------------------
real ASceneNodeItem::getRestitution() const
{
    return m_dRestitution;
}

//-----------------------------------------------------------------------------------------
real ASceneNodeItem::getFriction() const
{
    return m_dFriction;
}

//-----------------------------------------------------------------------------------------
real ASceneNodeItem::getRollingFriction() const
{
    return m_dRollingFriction;
}

//-----------------------------------------------------------------------------------------
QVector3D ASceneNodeItem::getLinearVelocity() const
{
    return m_LinearVelocity;
}

//-----------------------------------------------------------------------------------------
QVector3D ASceneNodeItem::getAngularVelocity() const
{
    return m_AngularVelocity;
}

//-----------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------
void ASceneNodeItem::addNode(CSceneNode* pNode)
{
    m_Nodes.insert(pNode);
}

//-----------------------------------------------------------------------------------------
void ASceneNodeItem::removeNode(CSceneNode* pNode)
{
    m_Nodes.remove(pNode);
}

