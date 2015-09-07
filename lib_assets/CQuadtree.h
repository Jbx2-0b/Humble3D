#ifndef CQUADTREE_H
#define CQUADTREE_H

// Lib
#include "lib_geometry_global.h"

// Foundations
#include "CBox3D.h"
#include "CQuadtreeNode.h"
#include "AQuadtreeItem.h"
#include "CMapDictionary.h"
#include "CFrustum.h"
#include "CSphere3D.h"

// Qt
#include <QMutex>
#include <QThread>

class CIntersectionFinder : public QThread
{
	Q_OBJECT

public:

	CIntersectionFinder(CQuadtreeNode* pRootNode, const CSphere3D& cameraRange)
		: m_pRootNode(pRootNode)
		, m_CameraRange(cameraRange)
	{
	}

	virtual void run()
	{
		recursiveTestIntersection(m_pRootNode, m_VisibleItems, m_CameraRange);
		emit sigFinish(this);
	}

	void recursiveTestIntersection(CQuadtreeNode* pNode, QSet<AQuadtreeItem*>& items, const CSphere3D& sphere) const
	{
		if (sphere.intersects(pNode->getBoundingBox()))
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
				foreach (CQuadtreeNode* pChild,  pNode->getChildren())
				{
					recursiveTestIntersection(pChild, items, sphere);
				}
			}
		}
	}

	const QSet<AQuadtreeItem*>& getVisibleItems() const { return m_VisibleItems; }

signals:

	void sigFinish(CIntersectionFinder* pThis);

private:

	CQuadtreeNode* m_pRootNode;

	QSet<AQuadtreeItem*> m_VisibleItems;

	CSphere3D m_CameraRange;
};


class LIB_GEOMETRYSHARED_EXPORT COctree : public QObject
{
	Q_OBJECT

public:

	//! Constructeur - Construit un quadtree depuis un dictionnaire de map
	COctree(CMapDictionary* pMapDictionary, int iTerrainSize, int iLevel, int iLODCount);

	//! Destructeur
	~COctree();

	void clearItems();

	QSet<AQuadtreeItem*> getProximityItems(const CFrustum& frustum) const;
	QSet<AQuadtreeItem*> getProximityItems(const CBox3D& bbox) const;
	QSet<AQuadtreeItem*> getProximityItems(const CSphere3D& sphere) const;


	void dumpLeaf();

	void dumpNodeTree();

	void searchIntersection(const CSphere3D& cameraRange)
	{
		CIntersectionFinder* pFinder = new CIntersectionFinder(m_pRootNode, cameraRange);
		m_IntersectionFinders << pFinder;
		connect(pFinder, SIGNAL(sigFinish(CIntersectionFinder*)), this, SLOT(onSearchIntersectionFinished(CIntersectionFinder*)));
		pFinder->start();
	}

signals:

	void sigIntersect(const QSet<AQuadtreeItem*>& items);

public slots:

	void onSearchIntersectionFinished(CIntersectionFinder* pFinder);

private:

	void decompose(const CBox3D& bbox, CQuadtreeNode* pNode, int iLevel);

	void recursiveAttachToNode(CQuadtreeNode* pNode, AQuadtreeItem* pItem);


	void recursiveTestIntersection(CQuadtreeNode* pNode, QSet<AQuadtreeItem*>& items, const CFrustum& frustum) const;
	void recursiveTestIntersection(CQuadtreeNode* pNode, QSet<AQuadtreeItem*>& items, const CBox3D& bbox) const;
	void recursiveTestIntersection(CQuadtreeNode* pNode, QSet<AQuadtreeItem*>& items, const CSphere3D& sphere) const;

	CQuadtreeNode* m_pRootNode;

	QList<CQuadtreeNode*> m_LeafNodes;

	QList<CIntersectionFinder*> m_IntersectionFinders;

	int m_iLODCount;
};




#endif // CQUADTREE_H
