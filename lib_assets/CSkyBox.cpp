#include "CSkyBox.h"
#include "CSceneManager.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"

//-----------------------------------------------------------------------------------------
CSkyBox::CSkyBox()
    : CMesh("SkyBoxMesh")
{

}

//-----------------------------------------------------------------------------------------
CSkyBox::~CSkyBox()
{

}

//-----------------------------------------------------------------------------------------
void CSkyBox::updateGeometry()
{
    CSubMesh* pBuffer = createSubMesh();
    pBuffer->setAutoComputeNormals(true);

    CBuffer<QVector3D>& posBuffer = pBuffer->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = pBuffer->indicesBuffer();

    real dHalfSize = dDefaultSkyBoxSize / 2.;

    // Left face
    posBuffer << QVector3D(-dHalfSize, -dHalfSize, -dHalfSize);
    posBuffer << QVector3D(-dHalfSize, -dHalfSize, +dHalfSize);
    posBuffer << QVector3D(-dHalfSize, +dHalfSize, +dHalfSize);
    posBuffer << QVector3D(-dHalfSize, +dHalfSize, -dHalfSize);

    // Top face
    posBuffer << QVector3D(-dHalfSize, +dHalfSize, -dHalfSize);
    posBuffer << QVector3D(-dHalfSize, +dHalfSize, +dHalfSize);
    posBuffer << QVector3D(+dHalfSize, +dHalfSize, +dHalfSize);
    posBuffer << QVector3D(+dHalfSize, +dHalfSize, -dHalfSize);

    // Right face
    posBuffer << QVector3D(+dHalfSize, +dHalfSize, -dHalfSize);
    posBuffer << QVector3D(+dHalfSize, +dHalfSize, +dHalfSize);
    posBuffer << QVector3D(+dHalfSize, -dHalfSize, +dHalfSize);
    posBuffer << QVector3D(+dHalfSize, -dHalfSize, -dHalfSize);

    // Bottom face
    posBuffer << QVector3D(+dHalfSize, -dHalfSize, -dHalfSize);
    posBuffer << QVector3D(+dHalfSize, -dHalfSize, +dHalfSize);
    posBuffer << QVector3D(-dHalfSize, -dHalfSize, +dHalfSize);
    posBuffer << QVector3D(-dHalfSize, -dHalfSize, -dHalfSize);

    // Front face
    posBuffer << QVector3D(+dHalfSize, -dHalfSize, +dHalfSize);
    posBuffer << QVector3D(+dHalfSize, +dHalfSize, +dHalfSize);
    posBuffer << QVector3D(-dHalfSize, +dHalfSize, +dHalfSize);
    posBuffer << QVector3D(-dHalfSize, -dHalfSize, +dHalfSize);

    // Back face
    posBuffer << QVector3D(+dHalfSize, +dHalfSize, -dHalfSize);
    posBuffer << QVector3D(+dHalfSize, -dHalfSize, -dHalfSize);
    posBuffer << QVector3D(-dHalfSize, -dHalfSize, -dHalfSize);
    posBuffer << QVector3D(-dHalfSize, +dHalfSize, -dHalfSize);

    pBuffer->translate(0., -dHalfSize/3., 0.);

    idBuffer << 0  << 1  << 2  << 0  << 2  << 3	  // Left face
             << 4  << 5  << 6  << 4  << 6  << 7   // Top face
             << 8  << 9  << 10 << 8  << 10 << 11  // Right face
             << 12 << 14 << 15 << 12 << 13 << 14  // Bottom face
             << 16 << 17 << 18 << 16 << 18 << 19  // Front face
             << 20 << 22 << 23 << 20 << 21 << 22; // Back face
}
