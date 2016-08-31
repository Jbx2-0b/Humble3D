#include "CMeshGeometry.h"

//-----------------------------------------------------------------------------------------
CMeshGeometry::CMeshGeometry()
: m_bNeedBoundingBoxUpdate(true)
{

}

//-----------------------------------------------------------------------------------------
CMeshGeometry::~CMeshGeometry()
{
	clearMeshBuffers();
}

//-----------------------------------------------------------------------------------------
// Gestions des vertex buffers
//-----------------------------------------------------------------------------------------
CMeshBuffer* CMeshGeometry::createMeshBuffer()
{ 
	m_bNeedBoundingBoxUpdate = true;
	CMeshBuffer* pBuffer = new CMeshBuffer;
	m_MeshBuffers.append(pBuffer);
	return pBuffer;
}

//-----------------------------------------------------------------------------------------
void CMeshGeometry::removeMeshBuffer(CMeshBuffer* pBuffer)
{ 
	m_bNeedBoundingBoxUpdate = true; 
	m_MeshBuffers.removeAll(pBuffer);
	delete pBuffer;
}

//-----------------------------------------------------------------------------------------
void CMeshGeometry::clearMeshBuffers()
{
	foreach (CMeshBuffer* pBuffer, m_MeshBuffers)
	{
		m_MeshBuffers.removeAll(pBuffer);
		delete pBuffer;
	}

	m_bNeedBoundingBoxUpdate = true;
}

//-----------------------------------------------------------------------------------------
// BoundingBox
//-----------------------------------------------------------------------------------------
const CBox3D& CMeshGeometry::getBoundingBox()
{ 
	if (m_bNeedBoundingBoxUpdate)
	{
		computeBoundingBox();
		m_bNeedBoundingBoxUpdate = false;
	}

	return m_BoundingBox;
}

//-----------------------------------------------------------------------------------------
void CMeshGeometry::computeBoundingBox()
{
	m_BoundingBox.setNull();

	if (m_MeshBuffers.count() == 0)
		return;

	m_BoundingBox = m_MeshBuffers[0]->getBoundingBox();

	for (int b = 1; b < m_MeshBuffers.count(); ++b)
	{	    
		m_BoundingBox.united(m_MeshBuffers[b]->getBoundingBox());
	}
}

//-----------------------------------------------------------------------------------------
void CMeshGeometry::update(bool bMultithread)
{
	if (bMultithread)
	{
		s_GeometryInstancer.addMeshGeometry(this);
	}
	else
	{
		updateGeometry();
	}

	setDirty();
}

//-----------------------------------------------------------------------------------------

CMeshGeometry::CGeometryInstancer CMeshGeometry::s_GeometryInstancer;

CMeshGeometry::CGeometryInstancer::CGeometryInstancer(QObject *parent)
	: QThread(parent)
	, m_Mutex(QMutex::Recursive)
{
	start();
}

//-----------------------------------------------------------------------------------------
CMeshGeometry::CGeometryInstancer::~CGeometryInstancer()
{
	quit();
	wait();
}

//-----------------------------------------------------------------------------------------
void CMeshGeometry::CGeometryInstancer::addMeshGeometry(CMeshGeometry* pMesh)
{
	QMutexLocker locker(&m_Mutex);
	m_MeshGeometryList.append(pMesh);
}

//-----------------------------------------------------------------------------------------
CMeshGeometry* CMeshGeometry::CGeometryInstancer::getAvailableMeshGeometry()
{
	QMutexLocker locker(&m_Mutex);

	if (!m_MeshGeometryList.isEmpty())
	{
		for (int i = 0; i < m_MeshGeometryList.size(); ++i)
		{
			CMeshGeometry* pMesh = m_MeshGeometryList[i];
			if (!pMesh->isLocked())
			{
				m_MeshGeometryList.removeAt(i);
				return pMesh;
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------------------
void CMeshGeometry::CGeometryInstancer::run()
{
	while (true)
	{
		if (CMeshGeometry* pMesh = getAvailableMeshGeometry())
		{
			QMutexLocker locker(&m_Mutex);
			pMesh->lock();
			pMesh->updateGeometry();
			pMesh->unlock();
		}
		else
		{
			msleep(s_iWaitTime);
		}
	}
}