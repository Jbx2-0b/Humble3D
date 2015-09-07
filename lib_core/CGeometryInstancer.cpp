#include "CGeometryInstancer.h"
#include "CMesh.h"

//-----------------------------------------------------------------------------------------
CGeometryInstancer::CGeometryInstancer()
: QThread()
, m_Mutex(QMutex::Recursive)
{
    start();
}

//-----------------------------------------------------------------------------------------
CGeometryInstancer::~CGeometryInstancer()
{
    quit();
    wait();
}

//-----------------------------------------------------------------------------------------
void CGeometryInstancer::addMesh(CMesh* pMesh)
{
    QMutexLocker locker(&m_Mutex);
    m_MeshList.append(pMesh);
}

//-----------------------------------------------------------------------------------------
CMesh* CGeometryInstancer::getAnAvailableMesh()
{
    if (!m_MeshList.isEmpty())
    {
        int i = 0;
        while (true)
        {
            CMesh* pMesh = m_MeshList[i];
            if (!pMesh->isLocked())
            {
                QMutexLocker locker(&m_Mutex);
                m_MeshList.removeAt(i);
                return pMesh;
            }
            else
            {
                msleep(s_iWaitTime);
            }

            if (++i == m_MeshList.size()) i = 0;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------------------
void CGeometryInstancer::run()
{
    pLog->addMessage(eINFO, QString("Geometry Instancer Thread: %1")
                     .arg((qlonglong)QThread::currentThreadId()));

    while (true)
    {
        if (CMesh* pMesh = getAnAvailableMesh())
        {
            pMesh->lock();
            pMesh->updateGeometry();
            pMesh->setDirty();

//            pLog->addMessage(eDEBUGTHREAD, QString("CGeometryInstancer (Thread: %1) Mesh %2")
//                             .arg((qlonglong)QThread::currentThreadId())
//                             .arg(pMesh->getName()));
            pMesh->unlock();
        }
        else
        {
            msleep(s_iWaitTime);
        }
    }
}

