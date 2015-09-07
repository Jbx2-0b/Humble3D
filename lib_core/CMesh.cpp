#include "CMesh.h"
#include "CBox3D.h"
#include "CLogManager.h"
#include "CTriangle.h"
#include "CSubMesh.h"
#include "CSceneManager.h"



#ifdef ENABLE_MULTITHREADING
CGeometryInstancer CMesh::s_GeometryInstancer;
#endif

//-----------------------------------------------------------------------------------------
CMesh::CMesh(const QString& name)
    : AEntity(name)
    , m_bNeedUpdateBoundingBox(true)
    , m_bNeedUpdateBoundingSphere(true)
    , m_bMultithreadingEnabled(false)
{
}

//-----------------------------------------------------------------------------------------
CMesh::~CMesh(void)
{
    foreach (CSubMesh* pSubMesh, m_SubMeshs)
    {
        removeSubMesh(pSubMesh);
    }
}

//--------------------------------------------------------------------------
void CMesh::registerListener(IMeshListener* pListener)
{
    m_MeshListeners.insert(pListener);
}

//--------------------------------------------------------------------------
void CMesh::unregisterListener(IMeshListener* pListener)
{
    m_MeshListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
// SubMeshs
//-----------------------------------------------------------------------------------------
CSubMesh* CMesh::createSubMesh()
{
    CSubMesh* pSubMesh = new CSubMesh(this);
    m_SubMeshs.append(pSubMesh);
    setDirty();
    notifyUpdate();
    return pSubMesh;
}

//-----------------------------------------------------------------------------------------
void CMesh::removeSubMesh(CSubMesh* pSubMesh)
{
    m_SubMeshs.removeOne(pSubMesh);
    setDirty();
    notifyUpdate();
    delete pSubMesh;
}

//-----------------------------------------------------------------------------------------
void CMesh::clearSubMeshs()
{
    foreach (CSubMesh* pSubMesh, m_SubMeshs)
    {
        removeSubMesh(pSubMesh);
    }
    setDirty();
}

//-----------------------------------------------------------------------------------------
void CMesh::setMaterialName(const QString& materialName)
{
    foreach (CSubMesh* pSubMesh, m_SubMeshs)
    {
        pSubMesh->setMaterialName(materialName);
    }
}

//-----------------------------------------------------------------------------------------
void CMesh::setMultithreadingEnabled(bool bEnabled)
{
    m_bMultithreadingEnabled = bEnabled;
}

//-----------------------------------------------------------------------------------------
const CBox3D& CMesh::getBoundingBox() const
{
    if (m_bNeedUpdateBoundingBox)
    {
        m_BoundingBox.setNull();

        if (!m_SubMeshs.isEmpty())
        {
            m_BoundingBox = m_SubMeshs[0]->getBoundingBox();

            for (int b = 1; b < m_SubMeshs.count(); ++b)
            {
                m_BoundingBox.united(m_SubMeshs[b]->getBoundingBox());
            }
        }

        m_bNeedUpdateBoundingBox = false;
    }

    return m_BoundingBox;
}

//-----------------------------------------------------------------------------------------
const CSphere& CMesh::getBoundingSphere() const
{
    if (m_bNeedUpdateBoundingBox || m_bNeedUpdateBoundingSphere)
    {
        m_BoundingSphere = CSphere(getBoundingBox());

        m_bNeedUpdateBoundingSphere = false;
    }
    return m_BoundingSphere;
}

//-----------------------------------------------------------------------------------------
bool CMesh::hasSkeleton() const
{
    foreach (const CSubMesh* pSubMesh, m_SubMeshs)
    {
        if (pSubMesh->hasSkeleton())
            return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------
unsigned int CMesh::getPolygonCount() const
{
    unsigned int iPolygonCount = 0;

    foreach (CSubMesh* pSubMesh, m_SubMeshs)
    {
        iPolygonCount += pSubMesh->getPolygonCount();
    }

    return iPolygonCount;
}

//-----------------------------------------------------------------------------------------
// Bones
//-----------------------------------------------------------------------------------------
QMultiHash<CSceneNode*, CBone> CMesh::getBoneNodes(CSceneManager* pSceneManager) const
{
    QMultiHash<CSceneNode*, CBone> nodeMap;

    foreach (const CSubMesh* pSubMesh, m_SubMeshs)
    {
        foreach (const CBone& bone, pSubMesh->bonesBuffer())
        {
            if (CSceneNode* pNode = pSceneManager->findNode(bone.getNodeName()))
            {
                nodeMap.insert(pNode, bone);
            }
        }
    }

    return nodeMap;
}

//-----------------------------------------------------------------------------------------
// Fonctions mathématiques
//-----------------------------------------------------------------------------------------
void CMesh::computeNormals()
{
    foreach (CSubMesh* pSubMesh, m_SubMeshs)
    {
        if (pSubMesh->getAutoComputeNormals())
        {
            pSubMesh->computeNormals();
        }
    }
}

//-----------------------------------------------------------------------------------------
void CMesh::computeTangents()
{
    // TODO
}

//-----------------------------------------------------------------------------------------
bool CMesh::intersection(const CRay& ray, real* dDistance, const QMatrix4x4& transformation /*= QMatrix4x4()*/) const
{
    bool bIntersect = false;

    foreach (CSubMesh* pSubMesh, m_SubMeshs)
    {
        real dD = std::numeric_limits<real>::infinity();

        if (pSubMesh->intersection(ray, &dD, transformation))
        {
            if (dD < *dDistance)
            {
                *dDistance = dD;
                bIntersect = true;
            }
        }
    }

    return bIntersect;
}

//-----------------------------------------------------------------------------------------
bool CMesh::intersects(const CRay &ray, const QMatrix4x4& transformation /*= QMatrix4x4()*/) const
{
    foreach (CSubMesh* pSubMesh, m_SubMeshs)
    {
        if (pSubMesh->intersects(ray, transformation))
        {
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------------------
void CMesh::update()
{
#ifdef ENABLE_MULTITHREADING
    if (m_bMultithreadingEnabled && ENABLE_MULTITHREADING)
    {
        s_GeometryInstancer.addMesh(this);
    }
    else
    {
        updateGeometry();
        setDirty();
    }
#else
    updateGeometry();
    setDirty();
#endif
}

//--------------------------------------------------------------------------
void CMesh::setDirty()
{
    foreach (CSubMesh* pSubMesh, m_SubMeshs)
    {
        pSubMesh->setDirty();
    }

    m_bNeedUpdateBoundingBox = true;
    m_bNeedUpdateBoundingSphere = true;
    AWashable::setDirty();
}

//--------------------------------------------------------------------------
void CMesh::notifyUpdate()
{
    if (m_bNotificationsEnabled)
    {
        foreach (IMeshListener* pListener, m_MeshListeners)
        {
            pListener->onUpdate(this);
        }

        AEntity::notifyUpdate();
    }
}

//--------------------------------------------------------------------------
void CMesh::notifyDelete()
{
    foreach (IMeshListener* pListener, m_MeshListeners)
    {
        pListener->onDelete(this);
    }

    AEntity::notifyDelete();
}
