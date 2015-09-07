#include "CQuadtree.h"

//-----------------------------------------------------------------------------------------
CQuadtree::CQuadtree(CMapDictionary* pMapDictionary, int iTerrainSize, int iLevel, int iLODCount)
: m_iLODCount(iLODCount)
{
	int iWorldWidth = iTerrainSize * pMapDictionary->getMapColumnCount();
	int iWorldLength = iTerrainSize * pMapDictionary->getMapRowCount();
	int iWorldHeight = iTerrainSize;

	CBox3D worldBoundingBox(QVector3D(), QVector3D(iWorldWidth, iWorldHeight, iWorldLength));
	m_pRootNode = new CQuadtreeNode(worldBoundingBox);

	QTime time;
	time.start();
	decompose(worldBoundingBox, m_pRootNode, iLevel);
	qDebug() << time.elapsed();

	time.start();
    const CArray2D<CMap*>& maps = pMapDictionary->maps();

	foreach (CMap* pMap, maps.get())
	{
		recursiveAttachToNode(m_pRootNode, pMap);
	}

	qDebug() << time.elapsed();
}

//-----------------------------------------------------------------------------------------
CQuadtree::~CQuadtree()
{
	delete m_pRootNode;
}

//-----------------------------------------------------------------------------------------
void CQuadtree::decompose(const CBox3D& bbox, CQuadtreeNode* pNode, int iLevel)
{
	QVector3D size(bbox.getSize().x() / 2.0f, bbox.getSize().y(), bbox.getSize().z() / 2.0f);
	iLevel--;

	for (int x = 0; x < 2; ++x)
	{
		for (int z = 0; z < 2; ++z)
		{
			QVector3D min(bbox.getMinimum().x() + x * size.x(), bbox.getMinimum().y(), bbox.getMinimum().z() + z * size.z());
			CQuadtreeNode* pChild = pNode->createChild(CBox3D(min, min + size));

			if (iLevel > 0)
			{
				decompose(pChild->getBoundingBox(), pChild, iLevel);
			}
			else // 0
			{
				m_LeafNodes << pChild;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
void CQuadtree::clearItems()
{
	foreach (CQuadtreeNode* pNode, m_LeafNodes)
	{
		pNode->clearItems();
	}
}

//-----------------------------------------------------------------------------------------
QSet<AQuadtreeItem*> CQuadtree::getProximityItems(const CFrustum& frustum) const
{
	QSet<AQuadtreeItem*> visibleItems;

	recursiveTestIntersection(m_pRootNode, visibleItems, frustum);

	return visibleItems;
}

//-----------------------------------------------------------------------------------------
QSet<AQuadtreeItem*> CQuadtree::getProximityItems(const CBox3D& bbox) const
{
	QSet<AQuadtreeItem*> visibleItems;

	recursiveTestIntersection(m_pRootNode, visibleItems, bbox);

	return visibleItems;
}

//-----------------------------------------------------------------------------------------
QSet<AQuadtreeItem*> CQuadtree::getProximityItems(const CSphere3D& sphere) const
{
	QSet<AQuadtreeItem*> visibleItems;
	recursiveTestIntersection(m_pRootNode, visibleItems, sphere);
	return visibleItems;
}

//-----------------------------------------------------------------------------------------
void CQuadtree::recursiveTestIntersection(CQuadtreeNode* pNode, QSet<AQuadtreeItem*>& items, const CSphere3D& sphere) const
{
	if (sphere.intersects(pNode->getBoundingBox()))
	{
		if (pNode->getChildren().size() == 0)
		{
			//real dRadius = sphere.getRadius();

			foreach (AQuadtreeItem* pItem, pNode->getItems())
			{
				/*
				real dDistance = pItem->getDistance(sphere.getCenter());

				if (dDistance < dRadius / 3.)
					pItem->setLOD(1);
				else if (dDistance < 2 * dRadius / 3.)
					pItem->setLOD(2);
				else
					pItem->setLOD(3);*/

				items << pItem;
			}
		}
		else
		{
			foreach (CQuadtreeNode* pChild, pNode->getChildren())
			{
				recursiveTestIntersection(pChild, items, sphere);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
void CQuadtree::onSearchIntersectionFinished(CIntersectionFinder* pFinder)
{
	m_IntersectionFinders.removeAll(pFinder);
	emit sigIntersect(pFinder->getVisibleItems());
	delete pFinder;
}

//-----------------------------------------------------------------------------------------
void CQuadtree::recursiveAttachToNode(CQuadtreeNode* pNode, AQuadtreeItem* pItem)
{
	CBox3D nodeBbox = pNode->getBoundingBox();
	CBox3D itemBbox = pItem->getBoundingBox();

	bool bIntersect = nodeBbox.intersects(itemBbox);
	bool bContains = nodeBbox.contains(itemBbox);

	if (bIntersect || bContains)
	{
		if (pNode->getChildren().size() == 0)
		{
			pNode->addItem(pItem);
		}
		else
		{
			foreach (CQuadtreeNode* pChild,  pNode->getChildren())
			{
				recursiveAttachToNode(pChild, pItem);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
void CQuadtree::recursiveTestIntersection(CQuadtreeNode* pNode, QSet<AQuadtreeItem*>& items, const CFrustum& frustum) const
{
	if (frustum.isBoxInFrustum(pNode->getBoundingBox()) != eOutside)
	{
		if (pNode->getChildren().size() == 0)
		{
			foreach (AQuadtreeItem* pItem, pNode->getItems())
			{
				items << pItem;
			}
		}
		else
		{
			foreach (CQuadtreeNode* pChild, pNode->getChildren())
			{
				recursiveTestIntersection(pChild, items, frustum);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
void CQuadtree::recursiveTestIntersection(CQuadtreeNode* pNode, QSet<AQuadtreeItem*>& items, const CBox3D& bbox) const
{
	if (pNode->getBoundingBox().intersects(bbox) || pNode->getBoundingBox().contains(bbox))
	{
		if (pNode->getChildren().size() == 0)
		{
			foreach (AQuadtreeItem* pItem, pNode->getItems())
			{
				items << pItem;
			}
		}
		else
		{
			foreach (CQuadtreeNode* pChild, pNode->getChildren())
			{
				recursiveTestIntersection(pChild, items, bbox);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
void CQuadtree::dumpLeaf()
{
    /*foreach (CQuadtreeNode* pNode, m_LeafNodes)
	{
		foreach (AQuadtreeItem* pItem, pNode->getItems())
		{
			//qDebug() << QString("Leaf Map (") + pMap->getKey().first + ", " + pMap->getKey().second + ")";
		}
    }*/
}

//-----------------------------------------------------------------------------------------
void CQuadtree::dumpNodeTree()
{
	m_pRootNode->dumpNodeTree();
}

