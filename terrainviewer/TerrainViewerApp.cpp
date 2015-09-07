#include "TerrainViewerApp.h"

#include <QStringList>
#include <QCoreApplication>

#include "CCamera.h"
#include "CSceneManager.h"
#include "CCoordinateSystem.h"
#include "CLineGrid.h"
#include "CQtHelper.h"
#include "CGLGlobal.h"
#include "CGeometryGlobal.h"
#include "CResourcesPathManager.h"
#include "CWater.h"
#include "CPlaneMesh.h"
#include "CDomeMesh.h"
#include "CSphere.h"
#include "CCloudsMesh.h"
#include "CBillboardMesh.h"
#include "MathRand.h"


static const float dSpeed = 1000.0f;
static const float dMouseSensibility = 0.05f;

//-----------------------------------------------------------------------------------------
TerrainViewerApp::TerrainViewerApp(QWidget* parent /* = 0*/)
: CGraphicsView3D(parent)
, m_pTopToolBar(0)
, m_pBottomToolBar(0)
, m_eCurrentOption(eOptionRotate)
, m_bFullScreen(false)
, m_bRun(false)
, m_bPressed(false)
, m_LastPan(-1, -1)
, m_StartPan(-1, -1)
, m_vStartFOV(DefaultCameraHorizontalFOV)

{
	CResourcesPathManager::getInstance()->addPath(":/textures/", eAbsolute);
	CResourcesPathManager::getInstance()->addPath(":/shaders/", eAbsolute);
	CResourcesPathManager::getInstance()->addPath("/textures/");
	CResourcesPathManager::getInstance()->addPath("/shaders/");
    CResourcesPathManager::getInstance()->addPath("/scenes/");
    CResourcesPathManager::getInstance()->addPath("D:/HelpMe/QtRender/Deploy/textures/", eAbsolute);
    CResourcesPathManager::getInstance()->addPath("D:/HelpMe/QtRender/Deploy/shaders/", eAbsolute);
    CResourcesPathManager::getInstance()->addPath("D:/HelpMe/QtRender/Deploy/scenes/", eAbsolute);
	CResourcesPathManager::getInstance()->addPath("D:/HelpMe/QtRender/Deploy/map/", eAbsolute);
    CResourcesPathManager::getInstance()->addPath("F:/Projects/QtRender/Deploy/textures/", eAbsolute);
    CResourcesPathManager::getInstance()->addPath("F:/Projects/QtRender/Deploy/shaders/", eAbsolute);
    CResourcesPathManager::getInstance()->addPath("F:/Projects/QtRender/Deploy/scenes/", eAbsolute);
	CResourcesPathManager::getInstance()->addPath("F:/Projects/QtRender/Deploy/map/", eAbsolute);
	
    CResourcesPathManager::getInstance()->addPath("/home/openjbx/Projects/QtRender/Deploy/textures/", eAbsolute);
    CResourcesPathManager::getInstance()->addPath("/home/openjbx/Projects/QtRender/Deploy/scenes/", eAbsolute);
    CResourcesPathManager::getInstance()->addPath("/home/openjbx/Projects/QtRender/Deploy/shaders/", eAbsolute);
    CResourcesPathManager::getInstance()->addPath("/home/openjbx/Projects/QtRender/Deploy/map/", eAbsolute);

	// On crée un noeud afin d'y placer une lumiére
	m_pCenterNode = m_pSceneManager->getRootNode()->createChild("CenterNode", QVector3D(0, 0, 0));
	CSceneNode* pLightNode = m_pCenterNode->createChild("LightNode1", QVector3D(0.0, 15000.0, 0));
	pLightNode->setAutoCreated(true);

	// On crée une lumiére diffuse blanche
 	CLight* pLight = m_pSceneManager->createLight("Light 1");
	pLight->setDiffuseColor(0.6f, 0.6f, 0.6f);
	pLight->setAmbientColor(0.8f, 0.7f, 0.6f);
	pLight->setSpecularColor(0.4f, 0.4f, 0.4f);
	pLight->setDirection(QVector3D(-1, -1, 0));
	pLight->setAutoCreated(true);

	// On l'associe au noeud
	pLightNode->addItem(pLight);

	CSceneNode* pLightNode2 = m_pCenterNode->createChild("LightNode2", QVector3D(0.0, 15000.0, 0.0));
	pLightNode2->setAutoCreated(true);

	// On crée une lumiére diffuse bleue
	CLight* pLight2 = m_pSceneManager->createLight("Light 2");
	pLight2->setDiffuseColor(0.4f, 0.4f, 0.8f);
	pLight2->setAmbientColor(0.4f, 0.4f, 0.8f);
	pLight2->setSpecularColor(0.4f, 0.6f, 0.8f);
	pLight2->setDirection(QVector3D(-1, -1, 0));
	pLight2->setAutoCreated(true);
	
	// On l'associe au noeud
	pLightNode2->addItem(pLight2);

	CSceneNode* pRootNode = m_pSceneManager->getRootNode();

	// On crée une camera
	CCamera* pCamera = sceneManager()->createCamera();
	pCamera->setPerspective(0.7853f, (real) width() / (real) height(), 1.0, 1000000.);
	pCamera->setAutoCreated(true);
	pCamera->setEyePosition(QVector3D(70012, 11170, 100703));
	pCamera->setCenter(QVector3D(68382, 9709, 101795));

	// On la définit comme caméra courante
	pCamera->setCurrent();

	m_pTerrainManager = new CTerrainManager(m_pSceneManager, 22360., 256, 4, 250000.);
	//CSRTMLoader::toBinaryFile("srtm_38_12.asc", "srtm_38_12.bin", 7);
    m_pTerrainManager->loadSRTMBinaryFile("srtm_38_12.bin", 8);

	
	connect(this, SIGNAL(mouseMoved()),					this, SLOT(onMouseMoved()));
	connect(this, SIGNAL(mouseReleased()),				this, SLOT(onMouseReleased()));
	connect(this, SIGNAL(mousePressed()),				this, SLOT(onMousePressed()));
	connect(this, SIGNAL(keyPressed()),					this, SLOT(onKeyPressed()));
	connect(this, SIGNAL(sizeChanged(QSize)),			this, SLOT(onViewSizeChanged(QSize)));

	// On crée et on affecte une texture environmentale à l'objet de la premiére scéne
	QStringList fileNames;
	
	m_pTopToolBar = createToolBar();

	m_pRunButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	connect(m_pRunButton, SIGNAL(pressed()), this, SLOT(onRunEditSwitch()));
	m_pRunButton->setIcon(QIcon(":/Resources/Run.png"));
	m_pRunButton->setIconSize(QSize(32, 32));
	
	m_pFullScreenButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetRight);
	m_pFullScreenButton->setIcon(QIcon(":/Resources/FullScreen.png"));
	m_pFullScreenButton->setIconSize(QSize(32, 32));
	connect(m_pFullScreenButton, SIGNAL(pressed()), this, SLOT(onFullScreen()));

	QPushButton* pQuitButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetRight);
	pQuitButton->setIcon(QIcon(":/Resources/Quit.png"));
	pQuitButton->setIconSize(QSize(32, 32));
	connect(pQuitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

	m_pTopToolBar->setStyleSheetFile(":/Resources/ToolBar.css");
	m_pTopToolBar->setOpacity(0.8f);

	m_pRotateButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pRotateButton->setIcon(QIcon(":/Resources/Rotate.png"));
	m_pRotateButton->setIconSize(QSize(32, 32));
	m_pRotateButton->setCheckable(true);
	m_pRotateButton->setChecked(true);

	m_pZoomButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pZoomButton->setIcon(QIcon(":/Resources/Zoom.png"));
	m_pZoomButton->setIconSize(QSize(32, 32));
	m_pZoomButton->setCheckable(true);

	m_pPanButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pPanButton->setIcon(QIcon(":/Resources/Pan.png"));
	m_pPanButton->setIconSize(QSize(32, 32));
	m_pPanButton->setCheckable(true);

	m_pFOVButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pFOVButton->setIcon(QIcon(":/Resources/FOV.png"));
	m_pFOVButton->setIconSize(QSize(32, 32));
	m_pFOVButton->setCheckable(true);

	m_pResetButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pResetButton->setIcon(QIcon(":/Resources/Reset.png"));
	m_pResetButton->setIconSize(QSize(32, 32));
	
	connect(m_pZoomButton,			SIGNAL(clicked()), this, SLOT(onOptionChanged()));
	connect(m_pRotateButton,		SIGNAL(clicked()), this, SLOT(onOptionChanged()));
	connect(m_pPanButton,			SIGNAL(clicked()), this, SLOT(onOptionChanged()));
	connect(m_pFOVButton,			SIGNAL(clicked()), this, SLOT(onOptionChanged()));
	connect(m_pResetButton,			SIGNAL(clicked()), this, SLOT(onOptionChanged()));
		
	m_ExclusiveOptionButton.addButton(m_pZoomButton);
	m_ExclusiveOptionButton.addButton(m_pRotateButton);
	m_ExclusiveOptionButton.addButton(m_pPanButton);
	m_ExclusiveOptionButton.addButton(m_pFOVButton);
	
	m_pBottomToolBar = createToolBar(CToolBar::eToolBarBottom);
	m_pBottomToolBar->setStyleSheetFile(":/Resources/ToolBar.css");
	m_pBottomToolBar->setOpacity(0.5);
	m_pBottomToolBar->setThickness(30);

	m_pAxeXZButton = m_pBottomToolBar->createWidget<QPushButton>(CToolBar::eWidgetRight);
	m_pAxeXZButton->setText("XZ");
	m_pAxeXZButton->setCheckable(true);
	m_pAxeXZButton->setChecked(true);
	
	m_pAxeYButton = m_pBottomToolBar->createWidget<QPushButton>(CToolBar::eWidgetRight);
	m_pAxeYButton->setText("Y");
	m_pAxeYButton->setCheckable(true);
	m_pAxeYButton->setChecked(true);

	m_pFPSLabel = m_pBottomToolBar->createWidget<QLabel>(CToolBar::eWidgetLeft);
	m_pFPSLabel->setFixedWidth(600);
	connect(fpsManager(), SIGNAL(FPSChanged(int)), this, SLOT(onFPSChanged(int)));


	// On affiche la vue Qt
	glScene()->setBackgroundColor(QColor(125, 125, 125));
	resize(iDefaultViewSizeX, iDefaultViewSizeY);
	move(iDefaultViewPosX, iDefaultViewPosY);

	m_pSceneManager->setBoundingBoxPolicy(eVisibleIfSelected);

	setWindowTitle(QString("TerrainViewer - %1 - OpenGL %2 - GLSL %3").arg(GLHelper::getGPUName())
															 .arg(GLHelper::getOpenGLVersion())
															 .arg(GLHelper::getGLSLVersion()));


	renderer()->run();

	fpsManager()->setDesiredFPS(1000);
}

//-----------------------------------------------------------------------------------------
TerrainViewerApp::~TerrainViewerApp()
{
}

//-----------------------------------------------------------------------------------------
// EVENEMENTS
//-----------------------------------------------------------------------------------------
void TerrainViewerApp::onMousePressed()
{
	CMouseStates mouseStates = getMouseStates();

	if (mouseStates.isLeftButtonPressed())
	{
		m_bPressed = true;
	
		if (CCamera* pCamera = sceneManager()->getCurrentCamera())
		{
			m_StartPan = m_LastPan = mouseStates.getPosition();
			m_vStartEye = pCamera->getEyePosition();
			m_vStartCenter = pCamera->getCenter();
			m_vStartUpVector = pCamera->getUp();
			m_vStartFOV = pCamera->getHorizontalFOV();
		}
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::onMouseMoved()
{
	CMouseStates mouseStates = getMouseStates();

	QPoint delta = mouseStates.getPosition() - m_StartPan;

	if (CCamera* pCamera = sceneManager()->getCurrentCamera())
	{
		if (m_bRun)
		{
			QPoint appCenterInScreen(x() + width() / 2, y() + height() / 2);

			QPoint delta = mouseStates.getGlobalPosition() - appCenterInScreen;

			pCamera->tiltPanRollEye(-delta.y() * dMouseSensibility, -delta.x() * dMouseSensibility, 0.);
			pCamera->setUp(QVector3D(0., 1., 0.));

			QCursor::setPos(appCenterInScreen);
		}
		else
		{
			if (m_bPressed)
			{
				if (m_eCurrentOption == eOptionRotate)
				{
					pCamera->setEyePosition(m_vStartEye);
					pCamera->setCenter(m_vStartCenter);
					pCamera->setUp(m_vStartUpVector);
					pCamera->setHorizontalFOV(m_vStartFOV);
				}
				else
				{
					m_StartPan = m_LastPan;
					delta = mouseStates.getPosition() - m_StartPan;
					m_vStartEye = pCamera->getEyePosition();
					m_vStartCenter = pCamera->getCenter();
					m_vStartUpVector = pCamera->getUp();
					m_vStartFOV = pCamera->getHorizontalFOV();
				}

				m_LastPan = mouseStates.getPosition();

				if (m_eCurrentOption == eOptionZoom)
				{
					wheel(-delta.y());
				}
				else if (m_eCurrentOption == eOptionPan)
				{
					pan(delta.x(), delta.y());
				}
				else if (m_eCurrentOption == eOptionRotate)
				{
					rotate(delta.x(), delta.y());
				}
				else if (m_eCurrentOption == eOptionFOV)
				{
					fov(delta.y());
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::onMouseReleased()
{
	m_bPressed = false;
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::onKeyPressed()
{
	CKeyboardStates keyboardStates = getKeyboardStates();

	if (m_bRun)
	{
		if (CCamera* pCamera = sceneManager()->getCurrentCamera())
		{
			if (keyboardStates.getKey() == Qt::Key_Escape)
			{
				onRunEditSwitch();
			}

			if (keyboardStates.getKey() == Qt::Key_Z)
			{
				pCamera->translateEyePosition(QVector3D(0., 0., +dSpeed));
				pCamera->translateCenter(QVector3D(0., 0., +dSpeed));
			}
			else if (keyboardStates.getKey() == Qt::Key_S)
			{
				pCamera->translateEyePosition(QVector3D(0., 0., -dSpeed));
				pCamera->translateCenter(QVector3D(0., 0., -dSpeed));
			}

			if (keyboardStates.getKey() == Qt::Key_D)
			{
				pCamera->translateEyePosition(QVector3D(+dSpeed, 0., 0.));
				pCamera->translateCenter(QVector3D(+dSpeed, 0., 0.));
			}
			else if (keyboardStates.getKey()  == Qt::Key_Q)
			{
				pCamera->translateEyePosition(QVector3D(-dSpeed, 0., 0.));
				pCamera->translateCenter(QVector3D(-dSpeed, 0., 0.));
			}

			QVector3D eyePos = pCamera->getEyePosition();
			
			if (eyePos.y() < m_pTerrainManager->getAbsoluteHeight(eyePos.x(), eyePos.z()) + 100.)
			{
				pCamera->setEyePosition(QVector3D(eyePos.x(), m_pTerrainManager->getAbsoluteHeight(eyePos.x(), eyePos.z()) + 100., eyePos.z()));

				QVector3D centerPos = pCamera->getCenter();

				if (centerPos.y() < m_pTerrainManager->getAbsoluteHeight(centerPos.x(), centerPos.z()) + 1.)
				{
					pCamera->setCenter(QVector3D(centerPos.x(), m_pTerrainManager->getAbsoluteHeight(centerPos.x(), centerPos.z()) + 1., centerPos.z()));
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::wheel(int delta)
{
	if (CCamera* pCamera = sceneManager()->getCurrentCamera())
	{
		QVector3D viewVector = pCamera->getEyePosition() - pCamera->getCenter();
		real zoomMag = viewVector.length();
		real zoomIncrement = -real(delta) / 100.;
		if (!qFuzzyIsNull(zoomIncrement))
		{
			zoomMag += zoomIncrement;
			if (zoomMag < 1.)
				zoomMag = 1.;

			CRay viewLine(pCamera->getCenter(), viewVector.normalized());
			pCamera->setEyePosition(viewLine.point(zoomMag));
		}
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::pan(int deltax, int deltaY)
{
	if (CCamera* pCamera = sceneManager()->getCurrentCamera())
	{
		QVector3D t = pCamera->translation(QVector3D(deltax * 0.001, -deltaY * 0.001, 0.));

		pCamera->setEyePosition(pCamera->getEyePosition() - t);
		pCamera->setCenter(pCamera->getCenter() - t);
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::rotate(int deltax, int deltay)
{
	if (CCamera* pCamera = sceneManager()->getCurrentCamera())
	{
		real angleX = 0.;
		real angleY = 0.;

		if (m_pAxeYButton->isChecked())
		{
			angleX = deltax * 90. / width();
		}
		if (m_pAxeXZButton->isChecked())
		{
			angleY = deltay * 90. / height();
		}

		/*if (m_pSelectedMesh)
		{
			if (CSceneNode* pNode = m_pSelectedMesh->getNode())
			{
				pNode->rotate(QVector3D(1., 0., 0.), -angleX);
				pNode->rotate(QVector3D(0., 1., 0.), -angleY);
			}
		}
		else
		{*/

			QQuaternion q = pCamera->pan(-angleX);
			q *= pCamera->tilt(-angleY);
		
			pCamera->rotateCenter(q);
			pCamera->setUp(QVector3D(0., 1., 0.));
		//}
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::fov(int iDelta)
{
	if (CCamera* pCamera = sceneManager()->getCurrentCamera())
	{
		real dNextFOV = pCamera->getHorizontalFOV() + iDelta * 0.01;

		if (dNextFOV > 0.1 && dNextFOV < (Math::Pi - 0.1))
		{
			pCamera->setHorizontalFOV(dNextFOV);
		}
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::onViewSizeChanged(const QSize& size)
{
	int iViewWidth = size.width();
	int iViewHeight = size.height();

	if (CCamera* pCamera = sceneManager()->getCurrentCamera())
	{
		pCamera->setAspect((real)iViewWidth / (real)iViewHeight);
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::onFullScreen()
{
	m_bFullScreen = !m_bFullScreen;

	if (m_bFullScreen)
	{
		showFullScreen();
		m_pFullScreenButton->setIcon(QIcon(":/Resources/ExitFullScreen.png"));
		setStyleSheet("QGraphicsView { border-style: none; }");
	}
	else
	{
		showNormal();
		m_pFullScreenButton->setIcon(QIcon(":/Resources/FullScreen.png"));
		setStyleSheet("");
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::onRunEditSwitch()
{
	//static bool bWasInFullScreen = false;

	m_bRun = !m_bRun;

	//m_pBottomToolBar->setVisible(!m_bRun);
	m_pTopToolBar->setVisible(!m_bRun);

	if (m_bRun)
	{
		//bWasInFullScreen = m_bFullScreen;
		//showFullScreen();
		QCursor::setPos(x() + width() / 2, y() + height() / 2);
		qApp->setOverrideCursor(QCursor(Qt::BlankCursor));
	}
	else
	{
		//if (!bWasInFullScreen) showNormal();
		qApp->restoreOverrideCursor();
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::onOptionChanged()
{
	if (QPushButton* pSender = dynamic_cast<QPushButton*>(sender()))
	{
		if		(pSender == m_pZoomButton)		m_eCurrentOption = eOptionZoom;
		else if (pSender == m_pRotateButton)	m_eCurrentOption = eOptionRotate;
		else if (pSender == m_pPanButton)		m_eCurrentOption = eOptionPan;
		else if (pSender == m_pFOVButton)		m_eCurrentOption = eOptionFOV;
		else if (pSender == m_pResetButton)
		{
			if (CCamera* pCamera = m_pSceneManager->getCurrentCamera())
			{
				pCamera->setEyePosition(QVector3D(2., 1., 2.));
				pCamera->setCenter(QVector3D(0., 0., 0.));
				pCamera->setHorizontalFOV(DefaultCameraHorizontalFOV);
			}
		}
		
		m_pAxeXZButton->setVisible(m_eCurrentOption == TerrainViewerApp::eOptionRotate || m_eCurrentOption == TerrainViewerApp::eOptionPan);
		m_pAxeYButton->setVisible(m_eCurrentOption == TerrainViewerApp::eOptionRotate || m_eCurrentOption == TerrainViewerApp::eOptionPan);
	}
}

//-----------------------------------------------------------------------------------------
void TerrainViewerApp::onFPSChanged(int iFPS)
{
	m_pFPSLabel->setText(QString("%1 frame/s - Draw calls: %2 - Polygons/s: %3 M")
		.arg(iFPS)
		.arg(m_pGLRenderer->getDrawCallCount())
		.arg((real)(m_pGLRenderer->getPolygonCount() * iFPS) / 1000000.));
}
