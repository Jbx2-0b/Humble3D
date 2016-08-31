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
    foreach (CSceneNode* pSceneNode, pSceneManager->getSceneNodes())
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
    foreach (COctreeNode* pOctreeNode, m_SceneNodes.values(pSceneNode))
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

        // Le noeud n'a pas encore d'enfants
        if (!pOctreeNode->hasChildNodes())
        {
            QVector3D octreeNodeBBoxSize = octreeNodeBBox.getSize();
            // Si le noeud est saturé, et qu'on a pas atteint les limites de l'arbre, on lui crée des enfants
            if ((octreeNodeBBoxSize.x() > 1 || octreeNodeBBoxSize.y() > 1 || octreeNodeBBoxSize.z() > 1) &&
                    pOctreeNode->getPolygonCount() > m_uiPolygonCountThreshold &&
                    uiDepth < m_uiMaxDepth)
            {
                pOctreeNode->createChilds();

                // On reporte les précédents noeuds sur les noeuds enfants
                foreach (CSceneNode* pPrevSceneNode, sceneNodes)
                {
                    foreach (COctreeNode* pChildNode, pOctreeNode->getChildNodes())
                    {
                        recursiveAddSceneNode(pChildNode, pPrevSceneNode, uiDepth + 1);
                    }
                }
            }
        }

        if (pOctreeNode->hasChildNodes())
        {
            // On cherche l'intersection avec chacun des enfants pour placer le SceneNode
            foreach (COctreeNode* pChildNode, pOctreeNode->getChildNodes())
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

    if (frustum.isSphereInFrustum(CSphere(octreeNodeBBox)) != eOutside)
    {
        EnumIntersectionType eIntersectionType = frustum.isBoxInFrustum(octreeNodeBBox);

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
                foreach (CSceneNode* pSceneNode, pOctreeNode->getSceneNodes())
                {
                    const CBox3D& sceneNodeBBox = pSceneNode->getLocalAxisAlignedBoundingBox();
                    if (frustum.isSphereInFrustum(CSphere(sceneNodeBBox)) != eOutside)
                    {
                        if (frustum.isBoxInFrustum(sceneNodeBBox) != eOutside)
                        {
                            visibleSceneNodes.insert(pSceneNode);
                        }
                    }
                }
            }
            // It's not a leaf : let's test childs
            else if (!pOctreeNode->getSceneNodes().isEmpty())
            {
                foreach (const COctreeNode* pChild, pOctreeNode->getChildNodes())
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


