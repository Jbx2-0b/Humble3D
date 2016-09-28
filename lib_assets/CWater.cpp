#include "CWater.h"
#include "CSceneManager.h"
#include "CMath.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CShaderManager.h"

//-----------------------------------------------------------------------------------------
CWater::CWater(real dLength,
               real dWidth,
               int iLengthQuadCount,
               int iWidthQuadCount)
    : CMesh("Water")
    , m_dLength(dLength)
    , m_dWidth(dWidth)
    , m_iLengthQuadCount(iLengthQuadCount)
    , m_iWidthQuadCount(iWidthQuadCount)
{

}

//-----------------------------------------------------------------------------------------
CWater::~CWater()
{

}

//-----------------------------------------------------------------------------------------
void CWater::updateGeometry()
{
    /*
    setSelectable(false);

    m_pMaterial = CMaterialManager::getInstance().createMaterial("WaterMaterial");

    QStringList fileNames;
    fileNames << "xpos.png" << "xneg.png" << "ypos.png"
              << "yneg.png" << "zpos.png" << "zneg.png";

    CTextureCube* pTexture = CTextureManager::getInstance().createTextureCube("WaterTexCube", fileNames);
    m_pMaterial->addTexture(pTexture, eDiffuse);

    m_pMaterial->setOpacity(0.5);
    CShader* pShader = CShaderManager::getInstance().createShader("WaterShader",
                            "water.vertex.glsl",
                            "",
                            "water.fragment.glsl");

    pShader->setUniformValue("waterHeight", DefaultWaterHeight);
    pShader->setUniformValue("numWaves", DefaultNumWaves);

    for (int i = 0; i < DefaultNumWaves; ++i)
    {
        real amplitude = 0.05f / (real)(i + 1);
        pShader->setUniformValue(QString("amplitude[%1]").arg(i), amplitude);

        real wavelength = 0.8 * M_PI / (real)(i + 1);
        pShader->setUniformValue(QString("wavelength[%1]").arg(i), wavelength);

        real speed = 0.01f + 0.02 * (real)i;
        pShader->setUniformValue(QString("speed[%1]").arg(i), speed);

        real angle = Math::randDouble(-Math::Pi / 3., +Math::Pi / 3.);
        pShader->setUniformValue(QString("direction[%1]").arg(i), QVector2D(cos(angle), sin(angle)));
    }


    m_pMaterial->getRenderPass(0)->setShader(pShader->getName());
    m_pMaterial->setTwoSided(true);

    setMaterialName(m_pMaterial->getName());

    pShader->dumpUniformValues();*/

    m_pBuffer = createSubMesh();
    m_pBuffer->setAutoComputeNormals(false);

    m_dCenterL = m_dLength / 2.;
    m_dCenterW = m_dWidth / 2.;

    CBuffer<QVector3D>& posBuffer = m_pBuffer->positionsBuffer();
    CBuffer<IndiceType>& idBuffer = m_pBuffer->indicesBuffer();

    int iBaseIndex = 0;

    for (unsigned int l = 0; l < m_iLengthQuadCount; ++l)
    {
        for (unsigned int w = 0; w < m_iWidthQuadCount; ++w)
        {
            posBuffer << getPosition(l, w);
            posBuffer << getPosition(l, w + 1);
            posBuffer << getPosition(l + 1, w);
            posBuffer << getPosition(l + 1, w + 1);

            idBuffer << iBaseIndex;
            idBuffer << iBaseIndex + 3;
            idBuffer << iBaseIndex + 2;

            idBuffer << iBaseIndex;
            idBuffer << iBaseIndex + 1;
            idBuffer << iBaseIndex + 3;

            iBaseIndex += 4;
        }
    }

    setDirty();
}

//-----------------------------------------------------------------------------------------
QVector3D CWater::getPosition(int l, int w) const
{
    return QVector3D((real)l * m_dLength / (real)m_iLengthQuadCount - m_dCenterL, DefaultWaterHeight, (real)w * m_dWidth / (real)m_iWidthQuadCount  - m_dCenterW);
}
