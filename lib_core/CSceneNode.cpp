#include "CSceneNode.h"
#include "ASceneNodeItem.h"
#include "CMeshInstance.h"
#include "CSceneManager.h"

//-----------------------------------------------------------------------------------------
CSceneNode::CSceneNode(CSceneManager* pSceneManager, CSceneNode* pParent /*= 0*/)
    : CNode(pParent)
    , m_pSceneManager(pSceneManager)
    , m_LocalScaling(1., 1., 1.)
    , m_bNeedUpdateGlobalTransformation(true)
    , m_bNeedUpdateLocalBoundingBox(true)
    , m_bNeedUpdateGlobalBoundingBox(true)
{
    pSceneManager->addSceneNode(this);
}

//-----------------------------------------------------------------------------------------
CSceneNode::CSceneNode(CSceneManager* pSceneManager, const QString& name, CSceneNode* pParent /*= 0*/)
    : CNode(name, pParent)
    , m_pSceneManager(pSceneManager)
    , m_LocalScaling(1., 1., 1.)
    , m_bNeedUpdateGlobalTransformation(true)
    , m_bNeedUpdateLocalBoundingBox(true)
    , m_bNeedUpdateGlobalBoundingBox(true)
{
    pSceneManager->addSceneNode(this);
}

//-----------------------------------------------------------------------------------------
CSceneNode::~CSceneNode()
{
    for (CSceneNode* pNode : m_ChildNodes)
    {
        // On prévient les NodeItems qu'on détruit le noeud
        for (ASceneNodeItem* pNodeItem : m_NodeItems)
        {
            pNodeItem->removeNode(pNode);
        }
    }
}

//-----------------------------------------------------------------------------------------
CSceneNode* CSceneNode::clone() const
{
    CSceneNode* pClone = 0;

    if (m_pParent)
    {
        pClone = m_pParent->createChild(m_LocalTransformation);
    }
    else
    {
        LogManager.addMessage(eERROR, "Can't clone root node !");
    }

    pClone->addItems(m_NodeItems);

    return pClone;
}

//-----------------------------------------------------------------------------------------
const CBox3D& CSceneNode::getLocalAxisAlignedBoundingBox(bool bTransformed /*= true*/) const
{
    if (m_bNeedUpdateLocalBoundingBox)
    {
        m_LocalBoundingBox.setNull();
        for (ASceneNodeItem* pItem : m_NodeItems)
        {
            const CBox3D& meshBbox = pItem->getBoundingBox();
            m_LocalBoundingBox = m_LocalBoundingBox.united(meshBbox);
        }

        m_LocalTransformedBoundingBox = m_LocalBoundingBox.transformed(getGlobalTransformation());
        m_bNeedUpdateLocalBoundingBox = false;
        m_bNeedUpdateGlobalBoundingBox = true;
    }

    return bTransformed ? m_LocalTransformedBoundingBox : m_LocalBoundingBox;
}

//-----------------------------------------------------------------------------------------
const CBox3D& CSceneNode::getGlobalAxisAlignedBoundingBox() const
{
    if (m_bNeedUpdateGlobalBoundingBox)
    {
        for (ASceneNodeItem* pItem : m_NodeItems)
        {
            CBox3D meshBbox = pItem->getBoundingBox();
            m_GlobalBoundingBox = m_GlobalBoundingBox.united(meshBbox);
        }

        for (CSceneNode* pNode : m_ChildNodes)
        {
            m_GlobalBoundingBox = m_GlobalBoundingBox.united(pNode->getGlobalAxisAlignedBoundingBox());
        }

        m_GlobalBoundingBox.transform(getGlobalTransformation());

        m_bNeedUpdateGlobalBoundingBox = false;
    }

    return m_GlobalBoundingBox;
}

//-----------------------------------------------------------------------------------------
real CSceneNode::getLocalMass() const
{
    real dMass = 0.;

    for (ASceneNodeItem* pItem : m_NodeItems)
    {
        dMass += pItem->getMass();
    }

    return dMass;
}

//-----------------------------------------------------------------------------------------
real CSceneNode::getGlobalMass() const
{
    real dMass = getLocalMass();

    for (CSceneNode* pNode : m_ChildNodes)
    {
        dMass += pNode->getGlobalMass();
    }

    return dMass;
}

//-----------------------------------------------------------------------------------------
unsigned int CSceneNode::getLocalPolygonCount() const
{
    unsigned int iPolygonCount = 0;

    for (ASceneNodeItem* pItem : m_NodeItems)
    {
        iPolygonCount += pItem->getPolygonCount();
    }

    return iPolygonCount;
}

//-----------------------------------------------------------------------------------------
unsigned int CSceneNode::getGlobalPolygonCount() const
{
    unsigned int iPolygonCount = getLocalPolygonCount();


    for (CSceneNode* pNode : m_ChildNodes)
    {
        iPolygonCount += pNode->getGlobalPolygonCount();
    }

    return iPolygonCount;
}

//-----------------------------------------------------------------------------------------
void CSceneNode::addItem(ASceneNodeItem* pNodeItem)
{
    m_NodeItems.append(pNodeItem);
    pNodeItem->addNode(this);
    pNodeItem->registerListener(this);
    needUpdateBoundingBox();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::addItems(const QList<ASceneNodeItem*>& nodeItems)
{
    for (ASceneNodeItem* pNodeItem : nodeItems)
    {
        m_NodeItems.append(pNodeItem);
        pNodeItem->addNode(this);
        pNodeItem->registerListener(this);
    }
    needUpdateBoundingBox();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::removeItem(ASceneNodeItem* pNodeItem)
{
    m_NodeItems.removeOne(pNodeItem);
    pNodeItem->unregisterListener(this);
    needUpdateBoundingBox();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
QList<ASceneNodeItem*> CSceneNode::getAllChildItems() const
{
    QList<ASceneNodeItem*> items = m_NodeItems;

    for (CSceneNode* pSceneNode : m_ChildNodes)
    {
        items += pSceneNode->getAllChildItems();
    }

    return items;
}

//-----------------------------------------------------------------------------------------
CSceneNode* CSceneNode::createChild(const QVector3D& vTranslation /*= QVector3D()*/, const QQuaternion& qRotation /*= QQuaternion()*/)
{
    CSceneNode* pNode = new CSceneNode(m_pSceneManager, "Node", this);
    pNode->setNotificationsEnabled(false);
    QMatrix4x4 mTransformation;
    mTransformation.translate(vTranslation);
    mTransformation.rotate(qRotation);
    pNode->setLocalTransformation(mTransformation);
    m_ChildNodes.append(pNode);
    pNode->setNotificationsEnabled(true);
    notifyUpdate();
    return pNode;
}

//-----------------------------------------------------------------------------------------
CSceneNode* CSceneNode::createChild(const QMatrix4x4& transformation)
{
    CSceneNode* pNode = new CSceneNode(m_pSceneManager, "Node", this);
    pNode->setNotificationsEnabled(false);
    pNode->setLocalTransformation(transformation);
    m_ChildNodes.append(pNode);
    pNode->setNotificationsEnabled(true);
    notifyUpdate();
    return pNode;
}

//-----------------------------------------------------------------------------------------
CSceneNode* CSceneNode::createChild(const QString& nodeName, const QVector3D& vTranslation /*= QVector3D()*/, const QQuaternion& qRotation /*= QQuaternion()*/)
{
    CSceneNode* pNode = createChild(vTranslation, qRotation);
    pNode->setName(nodeName);
    return pNode;
}

//-----------------------------------------------------------------------------------------
CSceneNode* CSceneNode::createChild(const QString& nodeName, const QMatrix4x4& transformation)
{
    CSceneNode* pNode = createChild(transformation);
    pNode->setName(nodeName);
    return pNode;
}

//-----------------------------------------------------------------------------------------
void CSceneNode::setLocalTransformation(const QMatrix4x4& mTransformation)
{
    m_LocalTransformation = mTransformation;
    m_LocalInverseTransformation = mTransformation.inverted();
    needUpdateTransformationMatrix();
    needUpdateBoundingBox();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
const QMatrix4x4& CSceneNode::getLocalTransformation() const
{
    return m_LocalTransformation;
}

//-----------------------------------------------------------------------------------------
const QMatrix4x4& CSceneNode::getLocalInverseTransformation() const
{
    return m_LocalInverseTransformation;
}

//-----------------------------------------------------------------------------------------
const QMatrix4x4& CSceneNode::getGlobalTransformation() const
{
    if (m_bNeedUpdateGlobalTransformation)
    {
        const CSceneNode* pCurrent = this;

        m_GlobalTransformation.setToIdentity();

        while (pCurrent)
        {
            m_GlobalTransformation = pCurrent->getLocalTransformation() * m_GlobalTransformation;
            pCurrent = pCurrent->getParent();
        }

        m_GlobalInverseTransformation = m_GlobalTransformation.inverted();
        m_bNeedUpdateGlobalTransformation = false;
    }

    return m_GlobalTransformation;
}

//-----------------------------------------------------------------------------------------
const QMatrix4x4& CSceneNode::getGlobalInverseTransformation() const
{
    if (m_bNeedUpdateGlobalTransformation)
    {
        const CSceneNode* pCurrent = this;

        m_GlobalTransformation.setToIdentity();

        while (pCurrent)
        {
            m_GlobalTransformation = pCurrent->getLocalTransformation() * m_GlobalTransformation;
            pCurrent = pCurrent->getParent();
        }

        m_GlobalInverseTransformation = m_GlobalTransformation.inverted();
        m_bNeedUpdateGlobalTransformation = false;
    }

    return m_GlobalInverseTransformation;
}

//-----------------------------------------------------------------------------------------
QVector3D CSceneNode::getLocalScaling() const
{
    return m_LocalScaling;
}

//-----------------------------------------------------------------------------------------
QVector3D CSceneNode::getGlobalPosition() const
{
    return getGlobalTransformation().column(3).toVector3D();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::resetTransformation()
{
    m_LocalTransformation.setToIdentity();
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::translate(const QVector3D &vTranslate)
{
    m_LocalTransformation.translate(vTranslate);
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::translate(real dX, real dY, real dZ)
{
    m_LocalTransformation.translate(dX, dY, dZ);
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::scale(const QVector3D &vScale)
{
    m_LocalScaling *= vScale;
    m_LocalTransformation.scale(vScale);
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::scale(real dX, real dY, real dZ)
{
    m_LocalScaling *= QVector3D(dX, dY, dZ);
    m_LocalTransformation.scale(dX, dY, dZ);
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::scale(real dS)
{
    m_LocalScaling *= QVector3D(dS, dS, dS);
    m_LocalTransformation.scale(dS, dS, dS);
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::rotate(const QVector3D &vAxis, real dAngle)
{
    m_LocalTransformation.rotate(QQuaternion::fromAxisAndAngle(vAxis, dAngle));
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::rotate(const QQuaternion &qRotation)
{
    m_LocalTransformation.rotate(qRotation);
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::pitch(real dAngle)
{
    m_LocalTransformation.rotate(dAngle, QVector3D(1., 0., 0.));
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::roll(real dAngle)
{
    m_LocalTransformation.rotate(dAngle, QVector3D(0., 0., 1.));
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::yaw(real dAngle)
{
    m_LocalTransformation.rotate(dAngle, QVector3D(0., 1., 0.));
    needUpdateTransformationMatrix();
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::dumpNodeTree() const
{
    recursiveDump(0);
}

//-----------------------------------------------------------------------------------------
void CSceneNode::recursiveDump(int iLevel) const
{
    QString branchNode = "|";
    QString branchItems = " ";

    for (int i = 0; i < iLevel; ++i)
    {
        branchNode += "-";
        branchItems += " ";
    }
    qDebug() << branchNode << "Node name : " << getName();

    for (ASceneNodeItem* pItem : m_NodeItems)
    {
        qDebug() << branchItems << "Node [type: " << pItem->getTypeName() << ", name: " << pItem->getName() << "]";
    }

    for (CSceneNode* pChild : m_ChildNodes)
    {
        pChild->recursiveDump(iLevel + 1);
    }
}

//-----------------------------------------------------------------------------------------
void CSceneNode::onUpdate(ASceneNodeItem*)
{
    needUpdateBoundingBox();
}

//-----------------------------------------------------------------------------------------
void CSceneNode::onDelete(ASceneNodeItem*)
{

}
