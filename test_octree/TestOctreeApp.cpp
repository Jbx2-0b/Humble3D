#include "TestOctreeApp.h"
#include "Shapes/CSphereMesh.h"
#include "CMath.h"
#include "COctree.h"

TestOctreeApp::TestOctreeApp()
    : m_pController(0)
    , m_pController2(0)
{
    // On crée une camera
    CCamera* pCamera = getSceneManager()->createCamera();
    pCamera->setEyePosition(QVector3D(0, 0, -50));
    pCamera->setCenter(QVector3D(0., 0., 0.));
    pCamera->setHorizontalFOV(Math::Pi / 2.);
    pCamera->setNear(10.0);
    pCamera->setFar(1000.0);

    CWidget3D* pView = createWidget3D(pCamera);
    m_pController = new Controller(pView);

    pView->getFPSManager()->setDesiredFPS(1000);
    pView->setForceUpdate(true);

    getSceneManager()->buildOctree(CBox3D(QVector3D(-500, -500, -500), QVector3D(+500, +500, +500)), 1000, 5);

    // On crée une camera
    //    CCamera* pCamera2 = getSceneManager()->createCamera();
    //    pCamera2->setEyePosition(QVector3D(0, 0, -50));
    //    pCamera2->setCenter(QVector3D(0., 0., 0.));
    //    pCamera2->setHorizontalFOV(Math::Pi / 2.);
    //    pCamera2->setNear(10.0);
    //    pCamera2->setFar(500.0);

    //    CWidget3D* pAnotherView = createWidget3D(pCamera2);
    //    m_pController2 = new Controller(pAnotherView);


    // On crée une lumiére
    CLight* pLight = getSceneManager()->createLight();
    pLight->setDiffuseColor(1.0, 1.0, 1.0);
    pLight->setAmbientColor(1.0, 1.0, 1.0);
    pLight->setDirection(QVector3D(-1, 0, 0));
    pLight->setSpecularColor(1.0f, 1.0f, 1.0f);

    // On l'associe au noeud
    getSceneManager()->getRootNode()->addItem(pLight);

    CMaterial* pGreenMat = CMaterialManager::getInstance()->createMaterial("GreenMat");
    pGreenMat->setAmbientColor(0.25, 0.87, 0.21);
    pGreenMat->getRenderPass(0)->setShaderName("toon");


    //qDebug() << "Sphere poly count: " << pMesh->getPolygonCount();

    int size = 1000;
    for (int i = 0; i < 400; i++)
    {
        CSphereMesh* pMesh = CMeshManager::getInstance()->createCustomMesh<CSphereMesh>("CSphereMesh", "SphereMesh");
        pMesh->setRadius(1.0);

        CMeshInstance* pMeshInstance = getSceneManager()->createMeshInstance(pMesh, "Sphere");
        pMeshInstance->setMaterialName("GreenMat");

        QVector3D pos(Math::randDouble(-size, +size), Math::randDouble(-size, +size), Math::randDouble(-size, +size));

        CSceneNode* pSceneNode = getSceneManager()->getRootNode()->createChild(pos);
        pSceneNode->scale(Math::randDouble(10., 50.));
        pSceneNode->addItem(pMeshInstance);
    }

    //qDebug() << getSceneManager()->getOctree()->getVisibleSceneNodes(pCamera->getFrustum()).count();
}

TestOctreeApp::~TestOctreeApp()
{
    delete m_pController;
    delete m_pController2;
}
