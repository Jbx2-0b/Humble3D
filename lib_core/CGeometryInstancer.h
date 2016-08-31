#ifndef CGEOMETRYINSTANCER_H
#define CGEOMETRYINSTANCER_H

// Lib
#include "lib_core_global.h"

// Core
class CMesh;

// Qt
#include <QThread>
#include <QMutex>
#include <QList>

class LIB_CORE_SHARED_EXPORT CGeometryInstancer : public QThread
{
public:
    CGeometryInstancer();

    virtual ~CGeometryInstancer();

    void addMesh(CMesh* pMesh);

protected:

    virtual void run();

    CMesh* getAnAvailableMesh();

    QList<CMesh*> m_MeshList;

    static const int s_iWaitTime = 10;

    mutable QMutex m_Mutex;

};

#endif // CGEOMETRYINSTANCER_H
