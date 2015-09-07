#include "CQuadtreeNode.h"

//-----------------------------------------------------------------------------------------
CQuadtreeNode::CQuadtreeNode(const CBox3D& bbox)
: m_pParent(0)
, m_Bbox(bbox)
, m_Mutex(QMutex::Recursive)
{
}

//-----------------------------------------------------------------------------------------
CQuadtreeNode::CQuadtreeNode(CQuadtreeNode* pParent, const CBox3D& bbox)
: m_pParent(pParent)
, m_Bbox(bbox)
, m_Mutex(QMutex::Recursive)
{
}

//-----------------------------------------------------------------------------------------
CQuadtreeNode::~CQuadtreeNode()
{
	foreach (CQuadtreeNode* pNode, m_Children)
	{
		// Si ce noeud a un parent on le prévient de la suppression
		if (CQuadtreeNode* pParent = pNode->getParent())
		{
			pParent->removeChild(this);
		}

		delete pNode;
	}
}

//-----------------------------------------------------------------------------------------
void CQuadtreeNode::setParent(CQuadtreeNode* pParent)
{
	m_pParent = pParent;
}

//-----------------------------------------------------------------------------------------
CQuadtreeNode* CQuadtreeNode::getParent() const
{
	return m_pParent;
}

//-----------------------------------------------------------------------------------------
const QList<CQuadtreeNode*>& CQuadtreeNode::getChildren() const
{
	return m_Children;
}


//-----------------------------------------------------------------------------------------
QList<CQuadtreeNode*>& CQuadtreeNode::getChildren()
{
	return m_Children;
}

//-----------------------------------------------------------------------------------------
void CQuadtreeNode::setBoudingBox(const CBox3D& bbox)
{
	m_Bbox = bbox;
}

//-----------------------------------------------------------------------------------------
const CBox3D& CQuadtreeNode::getBoundingBox() const
{
	return m_Bbox;
}

//-----------------------------------------------------------------------------------------
bool CQuadtreeNode::isLinked(CQuadtreeNode* pNode)
{
	bool bLinked = false;

	if (pNode)
	{
		if (pNode == m_pParent)
		{
			bLinked = true;
		}
		else
		{
			foreach (CQuadtreeNode* pChild, m_Children)
			{
				if (pNode == pChild)
				{
					bLinked = true;
					break;
				}
			}
		}
	}

	return bLinked;
}

//-----------------------------------------------------------------------------------------
unsigned int CQuadtreeNode::getDepth() const
{
	unsigned int uiLevel = 0;

	const CQuadtreeNode* pCurrent = this;

	while (pCurrent != 0)
	{
		uiLevel++;
		pCurrent = pCurrent->getParent();
	}

	return uiLevel;
}

//-----------------------------------------------------------------------------------------
CQuadtreeNode* CQuadtreeNode::createChild(const CBox3D& bbox)
{
	CQuadtreeNode* pChild = new CQuadtreeNode(this, bbox);
	m_Children << pChild;
	return pChild;
}

//-----------------------------------------------------------------------------------------
void CQuadtreeNode::removeChild(CQuadtreeNode* pNode)
{
	m_Children.removeAll(pNode);
	pNode->setParent(0);
}

//-----------------------------------------------------------------------------------------
void CQuadtreeNode::addItem(AQuadtreeItem* pQuadtreeItem)
{
	m_Items << pQuadtreeItem;
}

//-----------------------------------------------------------------------------------------
void CQuadtreeNode::clearItems()
{
	m_Items.clear();
}

//-----------------------------------------------------------------------------------------
void CQuadtreeNode::dumpNodeTree() const
{
	recursiveDump(0);
}

//-----------------------------------------------------------------------------------------
void CQuadtreeNode::recursiveDump(int iLevel) const
{
	QString branchNode = "|";
	QString branchItems = " ";

	for (int i = 0; i < iLevel; ++i)
	{
		branchNode += "-";
		branchItems += " ";
	}

	qDebug() << branchNode + "O";


	foreach (CQuadtreeNode* pChild, m_Children)
	{
		pChild->recursiveDump(iLevel + 1);
	}
}

