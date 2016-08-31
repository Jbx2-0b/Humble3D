#ifndef CDEBUGMANAGER_H
#define CDEBUGMANAGER_H

// Lib
#include "lib_assets_global.h"

#include "CSceneManager.h"
#include "CSphere.h"
#include "CCylinder.h"

class LIB_ASSETS_SHARED_EXPORT CDebugManager
{
public:

    CDebugManager(CSceneManager* pSceneManager);

    virtual ~CDebugManager();

    CMeshInstance* createBox(const CBox3D& box, const Color& color, real scaleFactor = 1.0);

    CMeshInstance* createSphere(const CSphere& sphere, const Color& color, real scaleFactor = 1.0);

    CMeshInstance* createCylinder(const CCylinder& Cylinder, const Color& color, real scaleFactor = 1.0);

    CMeshInstance* createNormals(CMeshInstance* pMeshInstance, const Color& color, real fSize = 1.0);

    CMeshInstance* createFrustum(CCamera* pCamera, const Color& color);

    void setVisible(bool bVisible);

    void remove(CMeshInstance* pMeshInstance);

private:

    CSceneManager* m_pSceneManager;
    QList<CMeshInstance*> m_MeshInstances;
    QList<CMesh*> m_Meshs;
};

#endif // CDEBUGMANAGER_H
