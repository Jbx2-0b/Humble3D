// Math
#include "Math.h"

// Core
#include "CCamera.h"
#include "CSceneManager.h"
#include "CMaterialManager.h"
#include "CShaderManager.h"
#include "CDebugManager.h"
#include "Shapes/CSphereMesh.h"
#include "Shapes/CPlaneMesh.h"
#include "Shapes/CSolidBox.h"
#include "Shapes/CCoordinateSystem.h"

// Assimp
#include "CAssimpImporter.h"

// Physics
#include "TestPhysics.h"

// Ui
#include "CWidget3D.h"

// Qt
#include <QTimer>


//-----------------------------------------------------------------------------------------
TestPhysics::TestPhysics()
: m_bPressed(false)
, m_bTouch(false)
, m_StartPan(-1, -1)
, m_LastPan(-1, -1)
{
    //CShaderManager::getInstance()->createShader("bump", "://resources/NBT.vertex.glsl", "", "://resources/NBT.fragment.glsl");

    // On crée une camera
    CCamera* pCamera = getSceneManager()->createCamera();

    m_vNewEyePosition = QVector3D(60., 60., 60.);
    pCamera->setEyePosition(m_vNewEyePosition);
    pCamera->setCenter(QVector3D(0., 0., 0.));

    m_pView = createWidget3D(pCamera);
    m_pView->setAttribute(Qt::WA_AcceptTouchEvents);
    m_pView->setGeometry(2000, 100, 800, 800);
    m_pView->setBackgroundColor(QVector4D(0.2, 0.2, 0.6, 1.0));

	// On crée un noeud afin d'y placer une lumiére
    CSceneNode* pRootNode = getSceneManager()->getRootNode();

    CSceneNode* pLightNode = pRootNode->createChild("LightNode", QVector3D(40., 40., 0.));

    // On crée une lumiére diffuse blanche
    CLight* pLight = getSceneManager()->createLight();
    pLight->setDiffuseColor(1.0, 1.0, 1.0);
    pLight->setAmbientColor(1.0, 1.0, 1.0);
	pLight->setDirection(QVector3D(-1, 0, 0));
    pLight->setSpecularColor(1.0f, 1.0f, 1.0f);

	// On l'associe au noeud
    pLightNode->addItem(pLight);

    CSceneNode* pLightNode2 = pRootNode->createChild("LightNode2", QVector3D(0., 40., 30.));

    // On crée une lumiére diffuse bleue
    CLight* pLight2 = getSceneManager()->createLight();
    pLight2->setDiffuseColor(0.3, 0.3, 0.8);
    pLight2->setAmbientColor(0.4, 0.3, 0.8);
    pLight2->setDirection(QVector3D(-1, 0, 0));
    pLight2->setSpecularColor(0.8f, 0.8f, 1.0f);

    // On l'associe au noeud
    pLightNode2->addItem(pLight2);

    //CSceneNode* pModelNode = pRootNode->createChild("ModelNode", QVector3D(0, 0, 0));
//    QList<AEntity*> loadedEntities = CAssimpImporter::loadScene("beer.3ds", m_pSceneManager, true);

//    CMaterial* pModelMat = CMaterialManager::getInstance()->createMaterial("ModelMat");
//    pModelMat->setDiffuseColor(0.2, 1., 0.2, 1.0);
//    pModelMat->setOpacity(0.4);
//    pModelMat->getRenderPass(0)->renderStates().blending().setEnabled(true);
//    //pModelMat->getRenderPass(0)->renderStates().blending().setBlendFunction(Source::eSourceAlpha, Destination::eOneMinusDestinationAlpha);
//    pModelMat->getRenderPass(0)->renderStates().blending().setBlendFunction(Source::eZero, Destination::eSourceColor);
//    //pModelMat->getRenderPass(0)->renderStates().blending().setBlendFunction(Source::eDestinationColor, Destination::eSourceColor);
//    pModelMat->getRenderPass(0)->renderStates().faceCulling().setEnabled(false);
//    pModelMat->getRenderPass(0)->renderStates().setDepthMask(false);
//    pModelMat->getRenderPass(0)->setShaderName("toon");

//    foreach (AEntity* pEntity, loadedEntities)
//    {
//        if (CMeshInstance* pMeshInstance = dynamic_cast<CMeshInstance*>(pEntity))
//        {
//            CSceneNode* pModelNode = pRootNode->createChild("ModelNode", QVector3D(0, 10, 0));
//            pModelNode->scale(5.0);
//            pMeshInstance->setMass(0.5);
//            pMeshInstance->setPhysicShape(eCylinderShapeY);
//            pMeshInstance->setRollingFriction(0.4);
//            pMeshInstance->setMaterialName(pModelMat->getName());
//            pModelNode->addItem(pMeshInstance);
//            pMeshInstance->getNode()->addItem(CDebugManager::getInstance()->createBox(pMeshInstance->getBoundingBox(), Color::eGreen));
//            m_PhysicsManager.registerItem(pMeshInstance);
//        }
//    }

    CSphereMesh* pSphereMesh = CMeshManager::getInstance()->createCustomMesh<CSphereMesh>("CSphereMesh", "Cube");
    pSphereMesh->setRadius(1.);

    ATexture* pBumpTexture = CTextureManager::getInstance()->createTexture2D("Bump", "://resources/3912-diffuse.jpg");
    //ATexture* pBumpNormalTexture = CTextureManager::getInstance()->createTexture2D("BumpNormals", "://resources/3912-normal.jpg");

    CMaterial* pSphereMat = CMaterialManager::getInstance()->createMaterial("CubeMat");
    pSphereMat->addTexture(pBumpTexture, eDiffuse);
    //pSphereMat->addTexture(pBumpNormalTexture, eNormals);
    pSphereMat->getRenderPass(0)->setShaderName("phong");

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                CMeshInstance* pSphere = getSceneManager()->createMeshInstance(pSphereMesh, "Sphere");
                pSphere->setPhysicShape(eSphereShape);
                pSphere->setRestitution(0.4);
                pSphere->setRollingFriction(0.1);

                pSphere->setMaterialName(pSphereMat->getName());
                pSphere->setMass(1.0);

                CSceneNode* pSphereNode = pRootNode->createChild(
                                QVector3D(0, i * pSphereMesh->getRadius() + 30, 0));
                pSphereNode->addItem(pSphere);
                pSphereNode->scale(Math::randInt(1, 6));
                m_PhysicsManager.registerItem(pSphere);
            }
        }
    }

    CSolidBox* pCubeMesh = CMeshManager::getInstance()->createCustomMesh<CSolidBox>("CSolidBox", "Cube");
    pCubeMesh->setSize(QVector3D(1., 1., 1.));

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                CMeshInstance* pCube = getSceneManager()->createMeshInstance(pCubeMesh, "Cube");
                pCube->setPhysicShape(eBoxShape);

                CMaterial* pCubeMat = CMaterialManager::getInstance()->createMaterial("CubeMat");
                pCubeMat->getRenderPass(0)->setShaderName("phong");
                pCubeMat->setAmbientColor(i / 3., j / 3., k / 3.);

                pCube->setMaterialName(pCubeMat->getName());
                pCube->setMass(1.0);

                CSceneNode* pCubeNode = pRootNode->createChild(
                            QVector3D(0,
                                      i * pCubeMesh->getSize().y() + 40,
                                      0));

                pCubeNode->addItem(pCube);
                pCubeNode->scale(Math::randInt(1, 6));
                m_PhysicsManager.registerItem(pCube);
            }
        }
    }

    CPlaneMesh* pGroundMesh = CMeshManager::getInstance()->createCustomMesh<CPlaneMesh>("CPlaneMesh", "GroundMesh");
    pGroundMesh->init(CPlaneMesh::ePlaneXZ, 100., 100., 10, 10, 1.0, 1.0);

    //pGroundMesh->setSize(QVector3D(100, 0.2, 100));
    CMeshInstance* pGround = getSceneManager()->createMeshInstance(pGroundMesh, "Ground");

    CMaterial* pGroundMat = CMaterialManager::getInstance()->createMaterial("GroundMat");
    pGroundMat->setAmbientColor(0.7, 0.7, 0.7);


    pGroundMat->getRenderPass(0)->setShaderName("phong");

    //ATexture* pWoodTexture = CTextureManager::getInstance()->createTexture2D("Wood", "://resources/Wood_Boards.jpg");
    ATexture* pGroundTexture = CTextureManager::getInstance()->createTexture2D("GroundTex", "://resources/ground.jpg");
    pGroundMat->addTexture(pGroundTexture, eDiffuse);

    pGround->setMaterialName(pGroundMat->getName());
    pGround->setMass(0.);
    pGround->setRestitution(0.5);
    pGround->setRollingFriction(0.4);

    CSceneNode* pGroundNode = pRootNode->createChild(); //QVector3D(0., - 2. * i, 2. * i), QQuaternion::fromAxisAndAngle(QVector3D(1., 0., 0.), 14));
    pGroundNode->addItem(pGround);
    m_PhysicsManager.registerItem(pGround);

    connect(m_pView, SIGNAL(mouseMoved()),         this, SLOT(onMouseMoved()));
    connect(m_pView, SIGNAL(mouseReleased()),      this, SLOT(onMouseReleased()));
    connect(m_pView, SIGNAL(mousePressed()),       this, SLOT(onMousePressed()));
    connect(m_pView, SIGNAL(touchScaleStarted()),  this, SLOT(onTouchScaleStarted()));
    //connect(m_pView, SIGNAL(touchScaleChanged(real)),      this, SLOT(onTouchScaleChanged(real)));
    //connect(m_pView, SIGNAL(touchScaleEnded()),                this, SLOT(onTouchScaleEnded()));
	
    m_PhysicsManager.start();

    //m_pView->getRenderer()->setRasterizationMode(eRasterizationLine);
}

//-----------------------------------------------------------------------------------------
TestPhysics::~TestPhysics()
{
}

//-----------------------------------------------------------------------------------------
void TestPhysics::onMousePressed()
{
    if (m_bTouch)
        return;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
	{
        CMouseStates mouseStates = m_pView->getMouseStates();

		m_bPressed = true;
		m_StartPan = m_LastPan = mouseStates.getPosition();
		m_vStartEye = pCamera->getEyePosition();
		m_vStartCenter = pCamera->getCenter();
		m_vStartUpVector = pCamera->getUp();
	}
}

//-----------------------------------------------------------------------------------------
void TestPhysics::onMouseReleased()
{
	m_bPressed = false;
    m_bTouch = false;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
    {
        m_vNewEyePosition = pCamera->getEyePosition();
    }
}

//-----------------------------------------------------------------------------------------
void TestPhysics::onMouseMoved()
{
    if (m_bTouch)
        return;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
	{
		if (m_bPressed)
		{
            CMouseStates mouseStates = m_pView->getMouseStates();

			QPoint delta = mouseStates.getPosition() - m_StartPan;

			if (mouseStates.isRightButtonPressed())
			{
				pCamera->setEyePosition(m_vStartEye);
				pCamera->setCenter(m_vStartCenter);
				pCamera->setUp(m_vStartUpVector);
			}
			else
			{
				m_StartPan = m_LastPan;
				delta = mouseStates.getPosition() - m_StartPan;
				m_vStartEye = pCamera->getEyePosition();
				m_vStartCenter = pCamera->getCenter();
				m_vStartUpVector = pCamera->getUp();
			}

			m_LastPan = mouseStates.getPosition();

			if (mouseStates.isRightButtonPressed())
			{
                wheel(-delta.y() * 2);
			}
			else
			{
				rotate(delta.x(), delta.y());
			}
		}
    }
}

//-----------------------------------------------------------------------------------------
void TestPhysics::wheel(int delta)
{
    if (CCamera* pCamera = m_pView->getCurrentCamera())
	{
		QVector3D viewVector = pCamera->getEyePosition() - pCamera->getCenter();
		real zoomMag = viewVector.length();
		real zoomIncrement = -real(delta) / 100.;
		if (!qFuzzyIsNull(zoomIncrement))
		{
			zoomMag += zoomIncrement;
            if (zoomMag < 2.)
                zoomMag = 2.;

			CRay viewLine(pCamera->getCenter(), viewVector.normalized());
			pCamera->setEyePosition(viewLine.point(zoomMag));
		}
	}
}

//-----------------------------------------------------------------------------------------
void TestPhysics::rotate(int deltax, int deltay)
{
    if (m_bTouch)
        return;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
	{
        real angleX = deltax * 90. / m_pView->width();
        real angleY = deltay * 90. / m_pView->height();

		QQuaternion q = pCamera->pan(-angleX);
		q *= pCamera->tilt(-angleY);
		pCamera->rotateCenter(q);

		pCamera->setUp(QVector3D(0., 1., 0.));
	}
}

//-----------------------------------------------------------------------------------------
void TestPhysics::onTouchScaleStarted()
{
    m_bTouch = true;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
    {
        m_vNewEyePosition = pCamera->getEyePosition();
    }
}

//-----------------------------------------------------------------------------------------
void TestPhysics::onTouchScaleChanged(real dScaleFactor)
{
    m_bTouch = true;

    QVector3D nPos = m_vNewEyePosition / dScaleFactor;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
    {
        pCamera->setEyePosition(nPos);
    }
}

//-----------------------------------------------------------------------------------------
void TestPhysics::onTouchScaleEnded()
{
    m_bTouch = false;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
    {
        m_vNewEyePosition = pCamera->getEyePosition();
    }
}
