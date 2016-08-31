#ifndef CDOMEMESH_H
#define CDOMEMESH_H

// Lib
#include "lib_assets_global.h"

// Foundations
#include "CMesh.h"
#include "CSceneManager.h"

class LIB_ASSETS_SHARED_EXPORT CDomeMesh : public CMesh, public CSceneManager::ISceneManagerListener
{
public:	

    CDomeMesh(real dRadius = 200000., int iRings = 24, int iSectors = 48);

    void setRadius(real dRadius)
    {
        m_dRadius = dRadius;
        update();
    }

    void setRingCount(int iRings)
    {
        m_iRings = iRings;
        update();
    }

    void setSectorCount(int iSectors)
    {
        m_iSectors = iSectors;
        update();
    }

    virtual void onUpdateCamera(CCamera* pCamera)
    {
        /*
        if (CSceneNode* pNode = getNode())
        {
            pNode->resetTransformation();
            pNode->translate(pCamera->getEyePosition().x(), 0., pCamera->getEyePosition().z());
        }*/
    }

    virtual ~CDomeMesh();

protected:

    virtual void updateGeometry();

private:

    CSubMesh* m_pBuffer;

    CMaterial* m_pMaterial;

    real m_dRadius;
    int m_iRings;
    int m_iSectors;
};

#endif // CDOMEMESH_H
