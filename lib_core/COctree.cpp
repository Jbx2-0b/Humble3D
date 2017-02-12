#include "COctree.h"
#include "CSceneManager.h"
#include "CTreeExplorer.h"

#include <QDebug>

//-----------------------------------------------------------------------------------------
COctree::COctree(CSceneManager* pSceneManager, const CBox3D& worldBbox, unsigned int uiPolygonCountThreshold, unsigned int iMaxDepth)
    : m_pSceneManager(pSceneManager)
    , m_RootNode(worldBbox)
    , m_uiPolygonCountThreshold(uiPolygonCountThreshold)
    , m_uiMaxDepth(iMaxDepth)
{
    for (CSceneNode* pSceneNode : pSceneManager->getSceneNodes())
    {
        addSceneNode(pSceneNode);
    }

    pSceneManager->registerListener(this);
}

//-----------------------------------------------------------------------------------------
COctree::~COctree()
{
    m_pSceneManager->unregisterListener(this);
}

//-----------------------------------------------------------------------------------------
void COctree::addSceneNode(CSceneNode* pSceneNode)
{
    if (!pSceneNode->getLocalAxisAlignedBoundingBox().isNull())
    {
        recursiveAddSceneNode(&m_RootNode, pSceneNode, 0);
    }
}

//-----------------------------------------------------------------------------------------
void COctree::updateSceneNode(CSceneNode* pSceneNode)
{
    removeSceneNode(pSceneNode);
    addSceneNode(pSceneNode);
}

//-----------------------------------------------------------------------------------------
void COctree::removeSceneNode(CSceneNode* pSceneNode)
{
    for (COctreeNode* pOctreeNode : m_SceneNodes.values(pSceneNode))
    {
        pOctreeNode->removeSceneNode(pSceneNode);

        //        if (!pOctreeNode->getSceneNodes().isEmpty() && !pOctreeNode->hasChildNodes() && pOctreeNode != &m_RootNode)
        //        {
        //            m_SceneNodes.remove(pSceneNode, pOctreeNode);
        //            delete pOctreeNode;
        //        }
    }

    m_SceneNodes.remove(pSceneNode);
}

//-----------------------------------------------------------------------------------------
void COctree::recursiveAddSceneNode(COctreeNode* pOctreeNode, CSceneNode* pSceneNode, unsigned int uiDepth)
{
    const CBox3D& sceneNodeBBox = pSceneNode->getLocalAxisAlignedBoundingBox();
    const CBox3D& octreeNodeBBox = pOctreeNode->getBoundingBox();

    if (octreeNodeBBox.contains(sceneNodeBBox) || octreeNodeBBox.intersects(sceneNodeBBox))
    {
        m_SceneNodes.insert(pSceneNode, pOctreeNode);
        QSet<CSceneNode*> sceneNodes = pOctreeNode->getSceneNodes();
        pOctreeNode->addSceneNode(pSceneNode);

        // Node has no children yet
        if (!pOctreeNode->hasChildNodes())
        {
            QVector3D octreeNodeBBoxSize = octreeNodeBBox.getSize();
            // If the node is saturated, and we have not reached the limits of the tree, we create children
            if ((octreeNodeBBoxSize.x() > 1 || octreeNodeBBoxSize.y() > 1 || octreeNodeBBoxSize.z() > 1) &&
                    pOctreeNode->getPolygonCount() > m_uiPolygonCountThreshold &&
                    uiDepth < m_uiMaxDepth)
            {
                pOctreeNode->createChilds();

                // The previous nodes are transferred to the child nodes
                for (CSceneNode* pPrevSceneNode : sceneNodes)
                {
                    for (COctreeNode* pChildNode : pOctreeNode->getChildNodes())
                    {
                        recursiveAddSceneNode(pChildNode, pPrevSceneNode, uiDepth + 1);
                    }
                }
            }
        }

        if (pOctreeNode->hasChildNodes())
        {
            // We look for the intersection with each child to place the SceneNode
            for (COctreeNode* pChildNode : pOctreeNode->getChildNodes())
            {
                const CBox3D& childNodeBBox = pChildNode->getBoundingBox();

                if (childNodeBBox.contains(sceneNodeBBox) || childNodeBBox.intersects(sceneNodeBBox))
                {
                    recursiveAddSceneNode(pChildNode, pSceneNode, uiDepth + 1);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
QSet<CSceneNode*> COctree::getVisibleSceneNodes(const CFrustum& frustum) const
{
    //QElapsedTimer benchOctreeTimer;
    //benchOctreeTimer.start();
    call = 0;
    QSet<CSceneNode*> visibleSceneNodes;
    recursiveFindVisibleSceneNodes(frustum, &m_RootNode, visibleSceneNodes);
    //qDebug() << "benchOctreeTimer: " << benchOctreeTimer.elapsed() / 1000000.;
    return visibleSceneNodes;
}

//-----------------------------------------------------------------------------------------
void COctree::recursiveFindVisibleSceneNodes(const CFrustum& frustum, const COctreeNode* pOctreeNode, QSet<CSceneNode*>& visibleSceneNodes) const
{
    call++;

    const CBox3D& octreeNodeBBox = pOctreeNode->getBoundingBox();

    if (frustum.intersect(CSphere(octreeNodeBBox)) != eOutside)
    {
        EnumIntersectionType eIntersectionType = frustum.intersect(octreeNodeBBox);

        // Inside : take them all !
        if (eIntersectionType == eInside)
        {
            visibleSceneNodes.unite(pOctreeNode->getSceneNodes());
        }
        else if (eIntersectionType == eIntersect)
        {
            // It's a leaf : We test individualy all the items
            if (pOctreeNode->isLeaf())
            {
                for (CSceneNode* pSceneNode : pOctreeNode->getSceneNodes())
                {
                    const CBox3D& sceneNodeBBox = pSceneNode->getLocalAxisAlignedBoundingBox();
                    if (frustum.intersect(CSphere(sceneNodeBBox)) != eOutside)
                    {
                        if (frustum.intersect(sceneNodeBBox) != eOutside)
                        {
                            visibleSceneNodes.insert(pSceneNode);
                        }
                    }
                }
            }
            // It's not a leaf : let's test childs
            else if (!pOctreeNode->getSceneNodes().isEmpty())
            {
                for (const COctreeNode* pChild : pOctreeNode->getChildNodes())
                {
                    recursiveFindVisibleSceneNodes(frustum, pChild, visibleSceneNodes);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void COctree::onCreateSceneNode(CSceneNode* pSceneNode)
{
    addSceneNode(pSceneNode);
}

//-----------------------------------------------------------------------------------------
void COctree::onUpdateSceneNode(CSceneNode* pSceneNode)
{
    updateSceneNode(pSceneNode);
}

//-----------------------------------------------------------------------------------------
void COctree::onDeleteSceneNode(CSceneNode* pSceneNode)
{
    removeSceneNode(pSceneNode);
}


