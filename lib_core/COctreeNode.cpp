#include "COctreeNode.h"


//-----------------------------------------------------------------------------------------
COctreeNode::COctreeNode(const CBox3D& bbox, COctreeNode* pParent /*= 0*/)
    : CNode(pParent)
    , m_BoundingBox(bbox)
    , m_Mutex(QMutex::Recursive)
{ }

//-----------------------------------------------------------------------------------------
COctreeNode::~COctreeNode()
{
}

//-----------------------------------------------------------------------------------------
const CBox3D& COctreeNode::getBoundingBox() const
{
    return m_BoundingBox;
}

//-----------------------------------------------------------------------------------------
const QList<COctreeNode*>& COctreeNode::createChilds()
{
    m_ChildNodes.clear();
    QVector3D size(m_BoundingBox.getSize().x() / 2.0f, m_BoundingBox.getSize().y() / 2.0f, m_BoundingBox.getSize().z() / 2.0f);

    for (int x = 0; x < 2; ++x)
    {
        for (int y = 0; y < 2; ++y)
        {
            for (int z = 0; z < 2; ++z)
            {
                QVector3D min(m_BoundingBox.getMinimum().x() + x * size.x(), m_BoundingBox.getMinimum().y() + y * size.y(), m_BoundingBox.getMinimum().z() + z * size.z());
                createChild(CBox3D(min, min + size));
            }
        }
    }
    return m_ChildNodes;
}

//-----------------------------------------------------------------------------------------
unsigned int COctreeNode::getPolygonCount() const
{
    unsigned int uiPolygonCount = 0;

    foreach (CSceneNode* pSceneNode, m_SceneNodes)
    {
        uiPolygonCount += pSceneNode->getLocalPolygonCount();
    }

    return uiPolygonCount;
}

//-----------------------------------------------------------------------------------------
void COctreeNode::addSceneNode(CSceneNode* pSceneNode)
{
    m_SceneNodes.insert(pSceneNode);
}

//-----------------------------------------------------------------------------------------
void COctreeNode::removeSceneNode(CSceneNode* pSceneNode)
{
    m_SceneNodes.remove(pSceneNode);
}

//-----------------------------------------------------------------------------------------
const QSet<CSceneNode*>& COctreeNode::getSceneNodes() const
{
    return m_SceneNodes;
}

//-----------------------------------------------------------------------------------------
QSet<CSceneNode*>& COctreeNode::getSceneNodes()
{
    return m_SceneNodes;
}

//-----------------------------------------------------------------------------------------
COctreeNode* COctreeNode::createChild(const CBox3D& bbox)
{
    COctreeNode* pChild = new COctreeNode(bbox, this);
    m_ChildNodes << pChild;
    return pChild;
}

//-----------------------------------------------------------------------------------------
void COctreeNode::recursiveDump(int iLevel) const
{
	QString branchNode = "|";
	QString branchItems = " ";

	for (int i = 0; i < iLevel; ++i)
	{
		branchNode += "-";
		branchItems += " ";
	}

	qDebug() << branchNode + "O";


    foreach (COctreeNode* pChild, m_ChildNodes)
	{
		pChild->recursiveDump(iLevel + 1);
	}
}

