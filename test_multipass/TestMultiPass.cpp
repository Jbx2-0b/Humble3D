#include "TestMultiPass.h"
#include "CMaterialManager.h"
#include "Shapes/CPlaneMesh.h"
#include "CRenderPass.h"
#include "Math.h"
#include "CAssimpImporter.h"
#include "CGLRenderer.h"
#include "CWidget3D.h"

//-----------------------------------------------------------------------------------------
TestMultiPass::TestMultiPass()
{
    // On crée une camera
    m_pSceneCamera = getSceneManager()->createCamera();
    m_pSceneCamera->setProjectionType(eProjectionOrtho);
    m_pSceneCamera->setOrtho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 2.f);

    m_pSceneCamera->setEyePosition(QVector3D(0., 0., -1.));
    m_pSceneCamera->setCenter(QVector3D(0., 0., 0.));

    m_pView = createWidget3D(m_pSceneCamera);

    m_pSceneCamera->setAspectRatio((real) m_pView->width() / (real) m_pView->height());

    LogManager.setActive(eDEBUGMEMORY, true);

    CShaderManager::getInstance().createShader("blur", "://blur.vertex.glsl", "", "://blur.fragment.glsl");

    // On charge le modéle
    CSceneNode* pRootNode = m_pSceneManager->getRootNode();

    CSceneNode* pModelNode = pRootNode->createChild("CowNode");

    if (!CAssimpImporter::mergeScene("G:/Humble3D/Deploy/scenes/cow.obj", m_pSceneManager, true, pModelNode).isEmpty())
    {
        // On récupére la bounding box
        CBox3D bbox = pRootNode->getGlobalAxisAlignedBoundingBox();

        real sizeX = bbox.getMaximum().x() - bbox.getMinimum().x();
        real sizeY = bbox.getMaximum().y() - bbox.getMinimum().y();
        real sizeZ = bbox.getMaximum().z() - bbox.getMinimum().z();

        real maxSize = sizeX;
        if (sizeY > maxSize) maxSize = sizeY;
        if (sizeZ > maxSize) maxSize = sizeZ;

        // On redimensionne la scene de façon à ce qu'elle ait une taille de 1x1x1 max
        pModelNode->scale(1. / maxSize);

        CBox3D scaledBbox = pModelNode->getGlobalAxisAlignedBoundingBox();

        // On la centre
        pModelNode->translate(-scaledBbox.getCenter());

        CSceneNode* pLightNode = pRootNode->createChild("LightNode", QVector3D(8.0, 12.0, 0));

        // On crée une lumiére diffuse blanche
        CLight* pLight = m_pSceneManager->createLight("Light 1");
        pLight->setDiffuseColor(0.0f, 0.6f, 0.0f);
        pLight->setAmbientColor(0.0f, 0.7f, 0.0f);
        pLight->setSpecularColor(0.0f, 0.4f, 0.0f);

        // On l'associe au noeud
        pLightNode->addItem(pLight);

        CSceneNode* pLightNode2 = pRootNode->createChild("LightNode2", QVector3D(-0.7, 0., -0.7));

        // On crée une lumiére diffuse bleue
        CLight* pLight2 = m_pSceneManager->createLight("Light 2");
        pLight2->setDiffuseColor(0.4f, 0.0f, 0.0f);
        pLight2->setAmbientColor(0.8f, 0.0f, 0.0f);
        pLight2->setSpecularColor(0.4f, 0.0f, 0.0f);

        // On l'associe au noeud
        pLightNode2->addItem(pLight2);

        CSceneNode* pLightNode3 = pRootNode->createChild("LightNode2", QVector3D(-0.7, 0.0, +0.7));

        // On crée une lumiére diffuse bleue
        CLight* pLight3 = m_pSceneManager->createLight("Light 3");
        pLight3->setDiffuseColor(0.0f, 0.0f, 0.4f);
        pLight3->setAmbientColor(0.0f, 0.0f, 0.8f);
        pLight3->setSpecularColor(0.0f, 0.0f, 0.4f);

        // On l'associe au noeud
        pLightNode3->addItem(pLight3);

        CMaterial* pMat = CMaterialManager::getInstance().getMaterialByName("cow");
        qDebug() << pMat->toString();

        CRenderPass* pPass = pMat->getRenderPass(0);

        pPass->setSize(QSize(512, 512));
        pPass->setTargetType(eTargetTexture);
        pPass->setClearColor(QVector4D(0.0, 0.0, 0.0, 1.0));
        pPass->setClearBufferFlags(ColorBuffer | DepthBuffer);

        CFrameBuffer* pFB = pPass->getFrameBuffer();
        ATexture* pTexture = pFB->createRenderTexture(eAttachmentColor0, eInternalFormatRGB);
        pFB->createRenderBuffer(eAttachmentDepth, eInternalFormatDepthComponent);

        // On crée une camera
        CCamera* pCamera = getSceneManager()->createCamera();
        pCamera->setEyePosition(QVector3D(2., 1., 2.));
        pCamera->setCenter(QVector3D(0., 0., 0.));
        pCamera->setAspectRatio((real)m_pView->width() / (real)m_pView->height());

        pPass->setCamera(pCamera);

        CPlaneMesh* pPlaneMesh = CMeshManager::getInstance().createCustomMesh<CPlaneMesh>("CPlaneMesh", "PlaneMesh");
        pPlaneMesh->init(CPlaneMesh::ePlaneXY, 1, 1, 1, 1);
        CMeshInstance* pPlane = getSceneManager()->createMeshInstance(pPlaneMesh, "Plane");

        CMaterial* pGridMat = CMaterialManager::getInstance().createMaterial("GridMat");
        pGridMat->addTexture(pTexture, eDiffuse);
        pGridMat->getRenderPass(0)->setShaderName("blur");
        pPlane->setMaterialName(pGridMat->getName());
        //pPlaneMesh->setMaterialName(CMaterialManager::getInstance().getMaterialNameByColor(Color::eBlue));

        CSceneNode* pGridNode = pRootNode->createChild("GridNode");
        pGridNode->addItem(pPlane);

        //qDebug() << pPass->getCamera()->toString();

        connect(m_pView->getFPSManager(), SIGNAL(FPSChanged(int)), this, SLOT(onFPSChanged(int)));

        m_pView->getFPSManager()->setDesiredFPS(60);
        //setForceUpdate(true);
        m_pView->setGeometry(QRect(1920, 100, 400, 400));

        m_pView->move(1920, 0.);
    }
}

//-----------------------------------------------------------------------------------------
TestMultiPass::~TestMultiPass()
{
}

//-----------------------------------------------------------------------------------------
void TestMultiPass::onFPSChanged(int iFPS)
{
    qDebug() << iFPS;
}
