#include "CDomeMesh.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"
#include "CMeshManager.h"


//-----------------------------------------------------------------------------------------
CDomeMesh::CDomeMesh(real dRadius, int iRings, int iSectors)
    : CMesh("Dome")
    , m_dRadius(dRadius)
    , m_iRings(iRings)
    , m_iSectors(iSectors)
{
    m_pBuffer = createSubMesh();
}

//-----------------------------------------------------------------------------------------
CDomeMesh::~CDomeMesh()
{
    /*
    if (m_pSceneManager)
    {
        m_pSceneManager->unregisterListener(this);
    }*/
}

//-----------------------------------------------------------------------------------------
void CDomeMesh::updateGeometry()
{
    //m_pSceneManager->registerCameraListener(this);

    /*
    m_pMaterial = CMaterialManager::getInstance()->createMaterial("DomeMaterial");
    setMaterialName(m_pMaterial->getName());

    QStringList fileNames;
    fileNames << "xpos.png" << "xneg.png" << "ypos.png"
              << "yneg.png" << "zpos.png" << "zneg.png";

    ATexture* pTexture = CTextureManager::getInstance()->createTextureCube("DomeTexCube", fileNames);

    m_pMaterial->addTexture(pTexture, eDiffuse);

    CShader* pShader = CShaderManager::getInstance()->createShader("dome",
                            "dome.vertex.glsl",
                            "",
                            "dome.fragment.glsl");

    pShader->setUniformValue("fog.color", QVector4D(0.75f, 0.81f, 0.87f, 0.0f));
    pShader->setUniformValue("fog.startAltitude", -100.0f);
    pShader->setUniformValue("fog.endAltitude", 10000.0f);

    m_pMaterial->getRenderPass(0)->setShader(pShader->getName());

    // TODO
    m_pMaterial->setTwoSided(true);*/

    m_pBuffer->clear();

    QVector<QVector3D>& posBuffer = m_pBuffer->positionsBuffer();
    QVector<QVector3D>& nBuffer = m_pBuffer->normalsBuffer();
    QVector<QVector3D>& texcoordBuffer = m_pBuffer->texCoordBuffer(0);
    CBuffer<IndiceType>& idBuffer = m_pBuffer->indicesBuffer();

    const real R = 1./(real)(m_iRings - 1);
    const real S = 1./(real)(m_iSectors - 1);
    int r, s;


    for (r = 0; r < m_iRings; r++)
    {
        for(s = 0; s < m_iSectors; s++)
        {
            real y = sin(- Math::HalfPi + Math::Pi * r * R);
            real x = cos(2 * Math::Pi * s * S) * sin(Math::Pi * r * R);
            real z = sin(2 * Math::Pi * s * S) * sin(Math::Pi * r * R);

            posBuffer << QVector3D(x * m_dRadius, y * m_dRadius, z * m_dRadius);
            texcoordBuffer << QVector3D(s * S, r * R, 0.);
            nBuffer << QVector3D(x, y, z);
        }
    }

    for(r = 0; r < m_iRings - 1; r++)
    {
        for(s = 0; s < m_iSectors - 1; s++)
        {
            idBuffer << r * m_iSectors + (s+1) << r * m_iSectors + s << (r+1) * m_iSectors + (s+1);
            idBuffer << (r+1) * m_iSectors + s << (r+1) * m_iSectors + (s+1) << r * m_iSectors + s;
        }
    }

    setDirty();
}
