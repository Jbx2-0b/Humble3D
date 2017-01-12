#include "TestQml.h"
#include "CCamera.h"
#include "CSceneManager.h"
#include "Shapes/CSphereMesh.h"
#include "Shapes/CCoordinateSystem.h"
#include "CSkyBox.h"
#include "Shapes/CPolyLine.h"
#include "CMaterialManager.h"
#include "CTextureManager.h"

// Qt
#include <QTimer>

//-----------------------------------------------------------------------------------------
TestQml::TestQml()
{
    // On cree un noeud afin d'y placer une lumiere
    CSceneNode* pRootNode = getSceneManager()->getRootNode();

    // On cree une lumiere diffuse bleue
    CLight* pLight = getSceneManager()->createLight();
    pLight->setDiffuseColor(1.0, 1.0, 1.0);
    pLight->setAmbientColor(1.0, 1.0, 1.0);
    pLight->setDirection(QVector3D(-1, 0, 0));
    pLight->setSpecularColor(1.0f, 1.0f, 1.0f);

    // On l'associe au noeud
    pRootNode->addItem(pLight);

    // SkyBox
    CSkyBox* pSkyBoxMesh = CMeshManager::getInstance().createCustomMesh<CSkyBox>("CSkyBox", "SkyBoxMesh");

    CMeshInstance* pSkyBox = getSceneManager()->createMeshInstance(pSkyBoxMesh, "SkyBox");
    pSkyBox->setSelectable(false);
    CSceneNode* pSkyBoxNode = pRootNode->createChild("SkyBoxNode");
    pSkyBoxNode->scale(400.);
    pSkyBoxNode->addItem(pSkyBox);


    CMaterial* pSkyBoxMat = CMaterialManager::getInstance().createMaterial("SkyBoxMaterial");
    pSkyBox->setMaterialName(pSkyBoxMat->getName());

    QStringList fileNames;
    fileNames << "://Resources/xpos.png" << "://Resources/xneg.png" << "://Resources/ypos.png"
              << "://Resources/yneg.png" << "://Resources/zpos.png" << "://Resources/zneg.png";

    ATexture* pSkyBoxTexture = CTextureManager::getInstance().createTextureCube("SkyBoxTexCube", fileNames);
    pSkyBoxMat->addTexture(pSkyBoxTexture, eDiffuse);

#ifdef EMBEDDED_TARGET
    CShader* pShader = CShaderManager::getInstance().createShader(
                "SkyBoxShader",
                "://Resources/skyboxES.vertex.glsl",
                "",
                "://Resources/skyboxES.fragment.glsl");
#else
    CShader* pShader = CShaderManager::getInstance().createShader(
                "SkyBoxShader",
                "://Resources/skybox.vertex.glsl",
                "",
                "://Resources/skybox.fragment.glsl");
#endif

    pSkyBoxMat->getRenderPass(0)->setShaderName(pShader->getName());
    pSkyBoxMat->getRenderPass(0)->renderStates().setFaceCulling(CFaceCulling(false));

    // Particules
    CBillboard* pBillboard = getSceneManager()->createBillboard();
    QVector<QVector3D> pos;
    for (int i = 0; i < 1000; ++i)
        pos << QVector3D(
                   Math::randDouble(-200., +200.),
                   Math::randDouble(-200., +200.),
                   Math::randDouble(-200., +200.));
    pBillboard->addPositions(pos);

    CMaterial* pBillboardMat = CMaterialManager::getInstance().createMaterial("BillboardMaterial");
    pBillboardMat->getRenderPass(0)->renderStates().setFaceCulling(CFaceCulling(false));
    CBlending blending;
    blending.setEnabled(true);
    blending.setBlendEquation(eAdd, eAdd);

    blending.setBlendSeparateFunction(Source::eSourceAlpha, Destination::eOneMinusSourceAlpha, Source::eOne, Destination::eZero);
    pBillboardMat->getRenderPass(0)->renderStates().setBlending(blending);

    ATexture* pBillboardTexture = CTextureManager::getInstance().createTexture2D("BillBoardTex", "://Resources/particle.png");
    pBillboardMat->addTexture(pBillboardTexture, eDiffuse);

    CShader* pBillboardShader = CShaderManager::getInstance().createShader("BillboardShader",
        "://Resources/billboard.vertex.glsl",
        "://Resources/billboard.geometry.glsl",
        "://Resources/billboard.fragment.glsl");

    pBillboardMat->getRenderPass(0)->setShaderName(pBillboardShader->getName());
    pBillboard->setMaterialName(pBillboardMat->getName());
    pBillboardShader->setUniformValue("halfSize", 1.);

    CSceneNode* pBillboardNode = pRootNode->createChild("BillboardNode");
    pBillboardNode->addItem(pBillboard);

    // Orbites
    CPolyLine* pPolyLine = CMeshManager::getInstance().createCustomMesh<CPolyLine>("CPolyLine", "CPolyLine");

    QList<QVector3D> pts;

    for (int i = 0; i <= 360; ++i)
    {
        pts << QVector3D(cos(Math::degToRad((real)i)), 0., sin(Math::degToRad((real)i)));
    }

    pPolyLine->addPoints(pts);

    CSphereMesh* pSphereMesh = CMeshManager::getInstance().createCustomMesh<CSphereMesh>("CSphereMesh", "SphereMesh");

    CMeshInstance* pSun = getSceneManager()->createMeshInstance(pSphereMesh, "Sun");
    CMaterial* pSunMat = CMaterialManager::getInstance().createMaterial("SunMat");
    pSunMat->setAmbientColor(1., 1., 1.);

    CTexture2D* pSunTexture = CTextureManager::getInstance().createTexture2D("SunTex", ":/Resources/sun.png");
    pSunMat->addTexture(pSunTexture, eDiffuse);
    pSun->setMaterialName(pSunMat->getName());

    CMeshInstance* pEarth = getSceneManager()->createMeshInstance(pSphereMesh, "Earth");
    CMaterial* pEarthMat = CMaterialManager::getInstance().createMaterial("EarthMat");

    CTexture2D* pEarthTexture = CTextureManager::getInstance().createTexture2D("EarthTex", ":/Resources/earth.png");
    pEarthMat->addTexture(pEarthTexture, eDiffuse);

    pEarthMat->setAmbientColor(0.1, 0.1, 0.1);
    pEarthMat->setDiffuseColor(1.0, 1.0, 1.0);
    pEarthMat->setShininessFactor(10);
    pEarth->setMaterialName(pEarthMat->getName());

    CMeshInstance* pJupiter = getSceneManager()->createMeshInstance(pSphereMesh, "Jupiter");
    CMaterial* pJupiterMat = CMaterialManager::getInstance().createMaterial("JupiterMat");
    pJupiterMat->setAmbientColor(0.4, 0.4, 0.4);

    CTexture2D* pJupiterTexture = CTextureManager::getInstance().createTexture2D("JupiterTex", ":/Resources/jupiter.png");
    pJupiterMat->addTexture(pJupiterTexture, eDiffuse);
    pJupiter->setMaterialName(pJupiterMat->getName());

    CMeshInstance* pMoon = getSceneManager()->createMeshInstance(pSphereMesh, "Moon");
    CMaterial* pMoonMat = CMaterialManager::getInstance().createMaterial("MoonMat");
    CTexture2D* pMoonTexture = CTextureManager::getInstance().createTexture2D("MoonTex", ":/Resources/moon.png");
    pMoonMat->addTexture(pMoonTexture, eDiffuse);
    pMoon->setMaterialName(pMoonMat->getName());

    CCoordinateSystem* pCoordinateSystemMesh = CMeshManager::getInstance().createCustomMesh<CCoordinateSystem>("CCoordinateSystem", "CCoordinateSystem");
    CMeshInstance* pCoordinateSystem = getSceneManager()->createMeshInstance(pCoordinateSystemMesh, "CoordinateSystem");
    pRootNode->addItem(pCoordinateSystem);
    pRootNode->addItem(pSkyBox);

    m_pSolarSystemNode = pRootNode->createChild(QVector3D(0., 0., 0.));

    m_pSunNode = m_pSolarSystemNode->createChild(QVector3D(0., 0., 0.));
    m_pSunNode->scale(4.0);
    m_pSunNode->addItem(pSun);

    m_pSunToEarthNode = m_pSolarSystemNode->createChild();
    m_pSunToJupiterNode = m_pSolarSystemNode->createChild();

    CMeshInstance* pSunToEarthPLine = getSceneManager()->createMeshInstance(pPolyLine, "SunToEarth");
    pSunToEarthPLine->setMaterialName(CMaterialManager::getInstance().getMaterialNameByColor(Color::eWhite));
    m_pSunToEarthNode->addItem(pSunToEarthPLine);
    m_pSunToEarthNode->scale(10.0);
    m_pEarthNode = m_pSolarSystemNode->createChild(QVector3D(10.0, 0., 0.));
    m_pEarthNode->scale(1.0);
    m_pEarthNode->addItem(pEarth);

    CMeshInstance* pSunToJupiterPLine = getSceneManager()->createMeshInstance(pPolyLine, "SunToJupiter");
    pSunToJupiterPLine->setMaterialName(CMaterialManager::getInstance().getMaterialNameByColor(Color::eWhite));
    m_pSunToJupiterNode->addItem(pSunToJupiterPLine);
    m_pSunToJupiterNode->scale(20);
    m_pJupiterNode = m_pSolarSystemNode->createChild(QVector3D(20.0, 0., 0.));
    m_pJupiterNode->scale(4.0);
    m_pJupiterNode->addItem(pJupiter);

    m_pEarthToMoonNode = m_pEarthNode->createChild();
    CMeshInstance* pEarthToMoonPLine = getSceneManager()->createMeshInstance(pPolyLine, "EarthToMoon");
    pEarthToMoonPLine->setMaterialName(CMaterialManager::getInstance().getMaterialNameByColor(Color::eWhite));
    m_pEarthToMoonNode->addItem(pEarthToMoonPLine);
    m_pEarthToMoonNode->scale(2.0);
    m_pEarthToMoonNode->rotate(QVector3D(1.0, 0.0, 0.0), 30);
    m_pMoonNode = m_pEarthToMoonNode->createChild(QVector3D(1.0, 0.0, 0.));
    m_pMoonNode->scale(0.2);
    m_pMoonNode->addItem(pMoon);

    QTimer* pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    pTimer->start(5);

    m_GlobalTime.start();
}

//-----------------------------------------------------------------------------------------
TestQml::~TestQml()
{
}

//-----------------------------------------------------------------------------------------
void TestQml::onTimeout()
{
    float dt = m_GlobalTime.elapsed() / 5.;
    m_GlobalTime.start();

    m_pSolarSystemNode->rotate(QVector3D(0, 1, 0), 0.104 * dt);
    m_pSunToEarthNode->rotate(QVector3D(0, 1, 0), 0.104 * dt);
    m_pSunToJupiterNode->rotate(QVector3D(0, 1, 0), 0.046 * dt);
    m_pEarthNode->rotate(QVector3D(0, 1, 0), 0.104 * dt);
    m_pJupiterNode->rotate(QVector3D(0, 1, 0), 0.046 * dt);
    m_pEarthToMoonNode->rotate(QVector3D(0, 1, 0), 0.2 * dt);
    m_pMoonNode->rotate(QVector3D(0, 1, 0), 0.2 * dt);
}
