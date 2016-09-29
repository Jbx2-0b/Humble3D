#ifndef CSIERPINSKITETRAHEDRONFACTORY_H
#define CSIERPINSKITETRAHEDRONFACTORY_H

// Lib
#include "lib_assets_global.h"

// Core
#include "CMeshInstance.h"
#include "Shapes/CTetrahedron.h"


class LIB_ASSETS_SHARED_EXPORT CSierpinskiTetrahedronFactory
{
public:
    CSierpinskiTetrahedronFactory(CSceneManager* pSceneManager);

    void compute(real size, int rank);
private:

    int m_Rank;
    int m_Count;
    void recursiveComputation(const QVector3D& center, real size, int currentIteration);
    CSceneManager* m_pSceneManager;
    CTetrahedron* m_pTetrahedronMesh;
    CMaterial* m_pTetrahedronMaterial;
};

#endif // CSIERPINSKITETRAHEDRONFACTORY_H
