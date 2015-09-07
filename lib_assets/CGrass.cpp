#include "CGrass.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"
#include "CMath.h"
#include "CHeightMap.h"
#include "CShaderManager.h"
#include "CMeshManager.h"
#include "CSceneManager.h"

//-----------------------------------------------------------------------------------------
CGrass::CGrass()
: CMesh("Grass")
, m_pHeightMap(0)
{
    //setMultithreadingEnabled(true);
    /*
    setSelectable(false);

    m_pGrassMaterial = CMaterialManager::getInstance()->createMaterial("Grass");
    m_pGrassMaterial->setAmbientColor(0.5f, 0.7f, 0.4f);
    m_pGrassMaterial->setDiffuseColor(0.3f, 0.2f, 0.1f);
    m_pGrassMaterial->setSpecularColor(0.3f, 0.2f, 0.2f);
    m_pGrassMaterial->setShininessFactor(128);

    CShader* pShader = CShaderManager::getInstance()->createShader("grass",
        "grass.vertex.glsl",
        "grass.geometry.glsl",
        "grass.fragment.glsl");
    m_pGrassMaterial->getRenderPass(0)->setShader(pShader->getName());

    //m_pGrassMaterial->getRenderingPass(0)->setShadingModel(eShadingModelColor);

    ATexture* pDiffuseTexture = CTextureManager::getInstance()->createTexture2D("GrassDiffuseTex", DefaultDiffuseGrassTexture);
    m_pGrassMaterial->addTexture(pDiffuseTexture, eDiffuse);

    ATexture* pNoiseTexture = CTextureManager::getInstance()->createTexture2D("NoiseTex", DefaultNormalsTexture);
    m_pGrassMaterial->addTexture(pNoiseTexture, eNormals);

    m_pGrassMaterial->setTwoSided(true);



    setMaterial(m_pGrassMaterial->getName());

    /*
    CUserBuffer* randMatColumnBuffer[4];

    for (int i = 0; i < 4; ++i)
    {
        randMatColumnBuffer[i] = m_pGrassBuffer->createUserBuffer("randMatColumn" + QString::number(i), 4);
    }*/

    /*
    //    6
    //	  /\
    //	 /  \
    //	4----5  > < = dF2ndFloorW
    //	|\   |
    //	| \  |
    //	|  \ |
    //	2----3  > < = dF1stFloorW
    //	|\   |
    //	| \  |
    //	|  \ |
    //	0----1
    */

    /*
    QVector<QVector4D> offset0;

    offset0 << QVector4D(- 1./2. * dBladeWidth, 0., 0., 1.);
    offset0 << QVector4D(+ 1./2. * dBladeWidth, 0., 0., 1.);
    offset0 << QVector4D(- dF1stFloorW / 2. * dBladeWidth, dF1stFloorH * dBladeHeight, - 0.3  * dZShift, 1.);
    offset0 << QVector4D(+ dF1stFloorW / 2. * dBladeWidth, dF1stFloorH * dBladeHeight, - 0.3 * dZShift, 1.);
    offset0 << QVector4D(- dF2ndFloorW / 2. * dBladeWidth, dF2ndFloorH * dBladeHeight, - dZShift, 1.);
    offset0 << QVector4D(+ dF2ndFloorW / 2. * dBladeWidth, dF2ndFloorH * dBladeHeight, - dZShift, 1.);
    offset0 << QVector4D(0., dBladeHeight, - 2. * dZShift, 1.);

    pShader->setUniformValueArray("offset0", offset0);


    QVector3D offset0N1 = QVector3D::normal(offset0[0].toVector3D(), offset0[1].toVector3D(), offset0[2].toVector3D());
    QVector3D offset0N2 = QVector3D::normal(offset0[2].toVector3D(), offset0[3].toVector3D(), offset0[4].toVector3D());
    QVector3D offset0N3 = QVector3D::normal(offset0[4].toVector3D(), offset0[5].toVector3D(), offset0[6].toVector3D());

    QVector<QVector3D> normals0;
    normals0 << offset0N1 << offset0N1 << (offset0N1 + offset0N2) / 2. << (offset0N1 + offset0N2) / 2. << (offset0N2 + offset0N3) / 2. << (offset0N2 + offset0N3) / 2.;

    pShader->setUniformValueArray("normals0", normals0);

    QVector<QVector3D> texCoord0;
    texCoord0 << QVector3D(0., 0., 0.);
    texCoord0 << QVector3D(1., 0., 0.);
    texCoord0 << QVector3D(1./6., 1./3., 0.);
    texCoord0 << QVector3D(5./6., 1./3., 0.);
    texCoord0 << QVector3D(1./3., 2./3., 0.);
    texCoord0 << QVector3D(2./3., 2./3., 0.);
    texCoord0 << QVector3D(0.5, 1., 0.);

    pShader->setUniformValueArray("texCoord0", texCoord0);

    /*
    //                  6
    //                 /|
    //                / |
    //               /  |
    //	0           /   |
    //	|\         /    |
    //	| \      4------5
    //  |  \    /      /
    //	|   \1 /      /
    //  |   /\/      /
    //  |  /  \     /
    //	| /    \   /
    //	|/      \ /
    //	2---------3
    //
    */

    /*
    QVector<QVector4D> offset1;
    offset1 << QVector4D(- dF2ndFloorW * dBladeWidth, dF2ndFloorH * dBladeHeight, + 0.6 * dZShift, 1.);
    offset1 << QVector4D(+ dF2ndFloorW * dBladeWidth, 0.5 * dF2ndFloorH * dBladeHeight, + 0.3 * dZShift, 1.);
    offset1 << QVector4D(- dBladeWidth, 0., 0., 1.);
    offset1 << QVector4D(+ dBladeWidth, 0., 0., 1.);
    offset1 << QVector4D(- dF2ndFloorW * dBladeWidth, dF2ndFloorH * dBladeHeight, - dZShift, 1.);
    offset1 << QVector4D(+ dF2ndFloorW * dBladeWidth, dF2ndFloorH * dBladeHeight, - dZShift, 1.);
    offset1 << QVector4D(0., dBladeHeight, - 2. * dZShift, 1.);

    pShader->setUniformValueArray("offset1", offset1);


    QVector3D offset1N1 =  - QVector3D::normal(offset1[0].toVector3D(), offset1[1].toVector3D(), offset1[2].toVector3D());
    QVector3D offset1N2 = - QVector3D::normal(offset1[2].toVector3D(), offset1[3].toVector3D(), offset1[4].toVector3D());
    QVector3D offset1N3 = QVector3D::normal(offset1[4].toVector3D(), offset1[5].toVector3D(), offset1[6].toVector3D());

    QVector<QVector3D> normals1;
    normals1 << offset1N1 << offset1N1 << (offset1N1 + offset1N2) / 2. << (offset1N1 + offset1N2) / 2. << (offset1N2 + offset1N3) / 2. << (offset1N2 + offset1N3) / 2.;

    pShader->setUniformValueArray("normals1", normals1);

    QVector<QVector3D> texCoord1;
    texCoord1 << QVector3D(0.5, 1., 0.);
    texCoord1 << QVector3D(0.80, 0.5, 0.);
    texCoord1 << QVector3D(0., 0., 0.);
    texCoord1 << QVector3D(1., 0., 0.);
    texCoord1 << QVector3D(1./3., 2./3., 0.);
    texCoord1 << QVector3D(2./3., 2./3., 0.);
    texCoord1 << QVector3D(0.5, 1., 0.);

    pShader->setUniformValueArray("texCoord1", texCoord1);

    /*
    //    4
    //	  /\
    //	 /  \
    //	2----3  > < = dF1stFloorW x 2
    //	|\   |
    //	| \  |
    //	|  \ |
    //	0----1
    */

    /*
    QVector<QVector4D> offset2;
    offset2 << QVector4D(-dBladeWidth, 0., 0., 1.);
    offset2 << QVector4D(dBladeWidth, 0., 0., 1.);
    offset2 << QVector4D(- dF2ndFloorW * dBladeWidth, dF2ndFloorH * dBladeHeight, - 1. * dZShift, 1.);
    offset2 << QVector4D(+ dF2ndFloorW * dBladeWidth, dF2ndFloorH * dBladeHeight, - 1. * dZShift, 1.);
    offset2 << QVector4D(0., dBladeHeight, - 2. * dZShift, 1.);

    pShader->setUniformValueArray("offset2", offset2);

    QVector3D offset2N1 = QVector3D::normal(offset2[0].toVector3D(), offset2[1].toVector3D(), offset2[2].toVector3D());
    QVector3D offset2N2 = QVector3D::normal(offset2[2].toVector3D(), offset2[3].toVector3D(), offset2[4].toVector3D());

    QVector<QVector3D> normals2;
    normals2 << offset2N1 << offset2N1 << (offset2N1 + offset2N2) / 2. << (offset2N1 + offset2N2) / 2. << offset2N2;

    pShader->setUniformValueArray("normals2", normals2);

    QVector<QVector3D> texCoord2;
    texCoord2 << QVector3D(0., 0., 0.);
    texCoord2 << QVector3D(1., 0., 0.);
    texCoord2 << QVector3D(1./3., 2./3., 0.);
    texCoord2 << QVector3D(2./3., 2./3., 0.);
    texCoord2 << QVector3D(0.5, 1., 0.);

    pShader->setUniformValueArray("texCoord2", texCoord2);

    /*
    //    2
    //	  /\
    //	 /  \
    //	0----1
    */

    /*
    QVector<QVector4D> offset3;
    offset3 << QVector4D(-dBladeWidth * 2., 0., 0., 1.0);
    offset3 << QVector4D(dBladeWidth * 2., 0., 0., 1.0);
    offset3 << QVector4D(0., dBladeHeight, - 2. * dZShift, 1.0);

    pShader->setUniformValueArray("offset3", offset3);

    QVector3D offset3N = QVector3D::normal(offset3[0].toVector3D(), offset3[1].toVector3D(), offset3[2].toVector3D());

    pShader->setUniformValue("normal3", offset3N);

    QVector<QVector3D> texCoord3;
    texCoord3 << QVector3D(0., 0., 0.);
    texCoord3 << QVector3D(1., 0., 0.);
    texCoord3 << QVector3D(0.5, 1., 0.);

    pShader->setUniformValueArray("texCoord3", texCoord3);*/
}

//-----------------------------------------------------------------------------------------
CGrass::~CGrass()
{

}

//-----------------------------------------------------------------------------------------
void CGrass::setHeightMap(CHeightMap* pHeightMap)
{
	m_pHeightMap = pHeightMap;
    update();
}

//-----------------------------------------------------------------------------------------
void CGrass::updateGeometry()
{
	if (m_pHeightMap)
	{
        m_pGrassBuffer = createSubMesh();
        m_pGrassBuffer->setPrimitiveType(ePrimitivePoint);

        CBuffer<QVector3D>& posBuffer = m_pGrassBuffer->positionsBuffer();
        CBuffer<IndiceType>& idBuffer = m_pGrassBuffer->indicesBuffer();
        CBuffer<QVector3D>& nBuffer = m_pGrassBuffer->normalsBuffer();

        int i = 0;

        for (real l = - m_pHeightMap->getLength() / 2.; l < m_pHeightMap->getLength() / 2.; l += dBladeStep)
        {
            for (real w = - m_pHeightMap->getWidth() / 2.; w < m_pHeightMap->getWidth() / 2.; w += dBladeStep)
            {
                real dL = l + Math::randDouble(-l/2., l/2.);
                real dW = w + Math::randDouble(-w/2., w/2.);

                QVector3D pos(dL, m_pHeightMap->getAltitude(dL, dW), dW);

                QVector3D normal = m_pHeightMap->getNormal(dL, dW);

                real iSlope = 1. - normal.y();
                if (pos.y() > 0.3 && iSlope < 0.05)
                {
                    posBuffer << pos;
                    idBuffer << i++;
                    nBuffer << normal;

                    /*
                    QMatrix4x4 T, R, S;
                    T.translate(Math::randNorm() * 0.03, 0., Math::randNorm() * 0.03);
                    R.rotate(Math::randDouble(0., 30.), 1., 0., 0.);
                    R.rotate(Math::randDouble(0., 360.), 0., 1., 0.);
                    R.rotate(Math::randDouble(0., 30.), 0., 0., 1.);
                    S.scale(Math::randNorm() + 0.5);

                    QMatrix4x4 transf = T * R * S;

                    for (int i = 0; i < 4; ++i)
                    {
                        QVector4D vColumn = transf.column(i);
                        *randMatColumnBuffer[i] << vColumn.x() << vColumn.y() << vColumn.z() << vColumn.w();
                    }*/
                }
            }
        }

        qDebug() << i - 1;
	}
}
