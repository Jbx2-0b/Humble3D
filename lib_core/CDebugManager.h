#ifndef CDEBUGMANAGER_H
#define CDEBUGMANAGER_H

// Lib
#include "lib_core_global.h"

// Math
#include "CCylinder.h"

// Core
#include "CSceneManager.h"
#include "CSphere.h"

// Qt
#include <QMutexLocker>

class LIB_CORE_SHARED_EXPORT CDebugManager
{
public:

    //! Accesseur de l'instance de la classe.
    static CDebugManager* getInstance()
    {
        return s_pInstance;
    }

    //! Crée l'instance de la classe.
    static void createInstance(CSceneManager* pSceneManager)
    {
        if (s_pInstance == 0)
        {
            QMutexLocker locker(&s_Mutex);

            if (s_pInstance == 0)
            {
                s_pInstance = new CDebugManager(pSceneManager);
            }
        }
    }

    //! Destructeur de l'instance de la classe.
    static void killInstance()
    {
        if (s_pInstance)
        {
            QMutexLocker locker(&s_Mutex);
            delete s_pInstance;
            s_pInstance = 0;
        }
    }

    CMeshInstance* createBox(const CBox3D& box, const Color& color, real scaleFactor = 1.0);
    CMeshInstance* createSphere(const CSphere& sphere, const Color& color, real scaleFactor = 1.0);
    CMeshInstance* createCylinder(const CCylinder& Cylinder, const Color& color, real scaleFactor = 1.0);
    CMeshInstance* createNormals(CMeshInstance* pMeshInstance, const Color& color, real fSize = 1.0);
    CMeshInstance* createFrustum(CCamera* pCamera, const Color& color);
    CMeshInstance* createLight(CLight* pLight, const Color& color);
    //CMeshInstance* createSkeleton(CMesh* pMesh, const Color& color);

    void setVisible(bool bVisible);

    void remove(CMeshInstance* pMeshInstance);

private:

    CDebugManager(CSceneManager* pSceneManager);

    virtual ~CDebugManager();

    static CDebugManager* s_pInstance;
    static QMutex s_Mutex;

    CSceneManager* m_pSceneManager;
    QList<CMeshInstance*> m_MeshInstances;
    QList<CMesh*> m_Meshs;
};

#endif // CDEBUGMANAGER_H
