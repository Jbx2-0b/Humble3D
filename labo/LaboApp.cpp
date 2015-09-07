#include "LaboApp.h"

#include <QStringList>
#include <QFileDialog>
#include <QCoreApplication>

#include "CCamera.h"
#include "CLight.h"
#include "CAssimpImporter.h"
#include "CSceneManager.h"
#include "Shapes/CCoordinateSystem.h"
#include "CLineGrid.h"
#include "CQtHelper.h"
#include "CGLHelper.h"
#include "CNodeTreeWidget.h"
#include "CAnimatedPanelDecorator.h"
#include "CLoadDialog.h"
#include "CSaveDialog.h"
#include "CGLGlobal.h"
#include "CGeometryGlobal.h"
#include "CAnimationManager.h"
#include "Shapes/CPlaneMesh.h"
#include "CScriptManager.h"
#include "CGraphicsView3D.h"


static const float dSpeed = 4.0f;
static const float dMouseSensibility = 0.05f;
static const bool bActiveTreeWidget = true;

//-----------------------------------------------------------------------------------------
LaboApp::LaboApp()
: m_Settings(configFileName, QSettings::IniFormat)
, m_pEditShadersTextItem(0)
, m_pTreeWidget(0)
, m_pTreeWidgetDecorator(0)
, m_pTopToolBar(0)
, m_pBottomToolBar(0)
, m_eCurrentOption(eOptionRotate)
, m_bFullScreen(false)
, m_bRun(false)
, m_bPressed(false)
, m_LastPan(-1, -1)
, m_StartPan(-1, -1)
, m_vStartFOV(Math::Pi / 4.)
, m_pSelectedMeshInstance(0)
, m_pSelectedBoundingBox(0)
, m_bMergeMaterials(true)
, m_bRescaleMeshs(true)
, m_bInvertXZ(false)
, m_bIsEditingShaders(false)
, m_pSDEdited(0)
{
	// Enregistre quelques shaders par défaut...
    CShaderManager::getInstance()->createShader("fresnel",		"://Resources/fresnel.vertex.glsl",	"", "://Resources/fresnel.fragment.glsl");
    CShaderManager::getInstance()->createShader("bump",			"://Resources/bump.vertex.glsl",	"", "://Resources/bump.fragment.glsl");

    CCamera* pCamera = getSceneManager()->createCamera();
    pCamera->setPerspective(0.7853f, (real) iDefaultViewSizeX / (real) iDefaultViewSizeY, 0.01f, 1000.f);
    m_AutoCreatedEntities.insert(pCamera);

    pCamera->setEyePosition(QVector3D(1., 2., 1.0));
    pCamera->setCenter(QVector3D(0., 0., 0.));

    m_pView = createGraphicsView3D(pCamera);

    // On crée un noeud afin d'y placer une lumiére
	m_pCenterNode = m_pSceneManager->getRootNode()->createChild("CenterNode", QVector3D(0, 0, 0));

    CSceneNode* pLightNode = m_pCenterNode->createChild("LightNode_1", QVector3D(2., 0., 0.));
    m_AutoCreatedEntities.insert(pLightNode);

	// On crée une lumiére diffuse blanche
    CLight* pLight = m_pSceneManager->createLight("Light_1");
    pLight->setDiffuseColor(0.8f, 0.8f, 0.8f);
    pLight->setAmbientColor(0.4f, 0.4f, 0.4f);
    pLight->setSpecularColor(0.4f, 0.4f, 0.4f);
    m_AutoCreatedEntities.insert(pLight);

	// On l'associe au noeud
    pLightNode->addItem(pLight);

    CSceneNode* pLightNode2 = m_pCenterNode->createChild("LightNode_2", QVector3D(-2., 0., 0.));
    m_AutoCreatedEntities.insert(pLightNode2);

    // On crée une lumiére diffuse blanche
    CLight* pLight2 = m_pSceneManager->createLight("Light_2");
    pLight2->setDiffuseColor(0.8f, 0.8f, 0.8f);
    pLight2->setAmbientColor(0.4f, 0.4f, 0.4f);
    pLight2->setSpecularColor(0.4f, 0.4f, 0.4f);
    m_AutoCreatedEntities.insert(pLight2);

    // On l'associe au noeud
    pLightNode2->addItem(pLight2);

    CSceneNode* pLightNode3 = m_pCenterNode->createChild("LightNode_3", QVector3D(0., 2., 0.));
    m_AutoCreatedEntities.insert(pLightNode3);

    // On crée une lumiére diffuse blanche
    CLight* pLight3 = m_pSceneManager->createLight("Light_3");
    pLight3->setDiffuseColor(0.8f, 0.8f, 0.8f);
    pLight3->setAmbientColor(0.4f, 0.4f, 0.4f);
    pLight3->setSpecularColor(0.4f, 0.4f, 0.4f);
    m_AutoCreatedEntities.insert(pLight3);

    // On l'associe au noeud
    pLightNode3->addItem(pLight3);

	// On crée un timer pour l'animer
	QTimer* pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	pTimer->start(25);

	m_pSceneManager->getRootNode()->dumpNodeTree();
	
	CSceneNode* pRootNode = m_pSceneManager->getRootNode();

    CCoordinateSystem* pCoordinateSystemMesh = CMeshManager::getInstance()->createCustomMesh<CCoordinateSystem>("CoordinateSystemMesh", "CoordinateSystemMesh");
    m_AutoCreatedEntities.insert(pCoordinateSystemMesh);

    CMeshInstance* pCoordinateSystem = m_pSceneManager->createMeshInstance(pCoordinateSystemMesh, "CoordinateSystem");
	pRootNode->addItem(pCoordinateSystem);
    m_AutoCreatedEntities.insert(pCoordinateSystem);


    /*CHeightMap* pHeightMap = m_pSceneManager->createCustomMesh<CHeightMap>("CHeightMap");
    pRootNode->addItem(pHeightMap);*/

	//CMaterial* pHeightMat = CMaterialManager::getInstance()->createMaterial("HeightMat");
	//pMercureMat->setAmbientColor(0.8, 0.8, 0.8);
	//pHeightMat->getRenderPass(0)->setShader(eShadingModelToon);
    //pHeightMap->setMaterial(pHeightMat->getName());

    /*CGrass* pGrass = m_pSceneManager->createCustomMesh<CGrass>("CGrass");
	pGrass->setHeightMap(pHeightMap);
    pRootNode->addItem(pGrass);*/

	/*

    CMaterial* pMat = CMaterialManager::getInstance()->createMaterial("Grass");
	pMat->setAmbientColor(0.f, 0.f, 0.f);
	pMat->setDiffuseColor(0.6f, 0.7f, 0.5f);
	pMat->setSpecularColor(0.1f, 0.1f, 0.1f);
	pMat->setShininessFactor(100);

    CShader* pShader = CShaderManager::getInstance()->createShader("NBT",
		"NBT.vertex.glsl", "", "NBT.fragment.glsl");

	pMat->getRenderingPass(0)->setUserShadingModelName(pShader->getName());

	//pMat->getRenderingPass(0)->setShadingModel(eShadingModelColor);

	ATexture* pDiffuseTexture = CTextureManager::getInstance()->createTexture2D("DiffuseTex", "3912-diffuse.jpg");
	pMat->addTexture(pDiffuseTexture, eDiffuse);

	ATexture* pNormalTexture = CTextureManager::getInstance()->createTexture2D("NormalTex", "3912-normal.jpg");
	pMat->addTexture(pNormalTexture, eNormals);

    pPlaneMesh->setMaterial(pMat->getName());*/


    connect(m_pView, SIGNAL(mouseMoved()),					this, SLOT(onMouseMoved()));
    connect(m_pView, SIGNAL(mouseReleased()),				this, SLOT(onMouseReleased()));
    connect(m_pView, SIGNAL(mousePressed()),				this, SLOT(onMousePressed()));
    connect(m_pView, SIGNAL(keyPressed()),					this, SLOT(onKeyPressed()));
    connect(m_pView, SIGNAL(sizeChanged(QSize)),			this, SLOT(onViewSizeChanged(QSize)));

    m_pTopToolBar = m_pView->createToolBar();

	m_pRunButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	connect(m_pRunButton, SIGNAL(pressed()), this, SLOT(onRunEditSwitch()));
	m_pRunButton->setIcon(QIcon(":/Resources/Run.png"));
	m_pRunButton->setIconSize(QSize(32, 32));
	//m_pRunButton->hide();

	m_pTreeButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pTreeButton->setIcon(QIcon(":/Resources/Tree.png"));
	m_pTreeButton->setIconSize(QSize(32, 32));

	m_pEditShadersButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pEditShadersButton->setIcon(QIcon(":/Resources/EditText.png"));
	m_pEditShadersButton->setIconSize(QSize(32, 32));
	m_pEditShadersButton->setCheckable(true);

	m_pEditVSButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pEditVSButton->setIcon(QIcon(":/Resources/VertexShader.png"));
	m_pEditVSButton->setIconSize(QSize(32, 32));
	m_pEditVSButton->setCheckable(true);
	m_pEditVSButton->setChecked(true);
	m_pEditVSButton->hide();

	m_pEditGSButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pEditGSButton->setIcon(QIcon(":/Resources/GeometryShader.png"));
	m_pEditGSButton->setIconSize(QSize(32, 32));
	m_pEditGSButton->setCheckable(true);
	m_pEditGSButton->hide();

	m_pEditFSButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pEditFSButton->setIcon(QIcon(":/Resources/FragmentShader.png"));
	m_pEditFSButton->setIconSize(QSize(32, 32));
	m_pEditFSButton->setCheckable(true);
	m_pEditFSButton->hide();

	m_ExclusiveEditShadersButton.addButton(m_pEditVSButton);
	m_ExclusiveEditShadersButton.addButton(m_pEditGSButton);
	m_ExclusiveEditShadersButton.addButton(m_pEditFSButton);

	connect (m_pEditVSButton, SIGNAL(clicked()), this, SLOT(onVSButtonClicked()));
	connect (m_pEditGSButton, SIGNAL(clicked()), this, SLOT(onGSButtonClicked()));
	connect (m_pEditFSButton, SIGNAL(clicked()), this, SLOT(onFSButtonClicked()));

	m_pEditItemButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pEditItemButton->setIcon(QIcon(":/Resources/Edit.png"));
	m_pEditItemButton->setIconSize(QSize(32, 32));
	m_pEditItemButton->setCheckable(true);

	m_pLoadButton = m_pTopToolBar->createWidget<QPushButton>();
	m_pLoadButton->setIcon(QIcon(":/Resources/Open.png"));
	m_pLoadButton->setIconSize(QSize(32, 32));
    connect(m_pLoadButton, SIGNAL(pressed()), this, SLOT(onLoad()));

    m_pSaveButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
    m_pSaveButton->setIcon(QIcon(":/Resources/Save.png"));
    m_pSaveButton->setIconSize(QSize(32, 32));

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
	
    connect(m_pSaveButton,			SIGNAL(clicked()), this, SLOT(onOptionChanged()));
    connect(m_pZoomButton,			SIGNAL(clicked()), this, SLOT(onOptionChanged()));
	connect(m_pRotateButton,		SIGNAL(clicked()), this, SLOT(onOptionChanged()));
	connect(m_pPanButton,			SIGNAL(clicked()), this, SLOT(onOptionChanged()));
	connect(m_pFOVButton,			SIGNAL(clicked()), this, SLOT(onOptionChanged()));
	connect(m_pResetButton,			SIGNAL(clicked()), this, SLOT(onOptionChanged()));
	connect(m_pEditShadersButton,	SIGNAL(clicked()), this, SLOT(onEditShadersClicked()));
		
	m_ExclusiveOptionButton.addButton(m_pZoomButton);
	m_ExclusiveOptionButton.addButton(m_pRotateButton);
	m_ExclusiveOptionButton.addButton(m_pPanButton);
	m_ExclusiveOptionButton.addButton(m_pFOVButton);
	
    m_pBottomToolBar = m_pView->createToolBar(CToolBar::eToolBarBottom);
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

	m_pLoadProgressBar = m_pBottomToolBar->createWidget<QProgressBar>(CToolBar::eWidgetLeft, 80);
	m_pLoadProgressBar->setTextVisible(false);
	m_pLoadProgressBar->setVisible(false);
    m_pLoadProgressBar->setStyleSheet(CQtHelper::loadStyleSheet(":/Resources/ProgressBar.css"));

	m_pFPSLabel = m_pBottomToolBar->createWidget<QLabel>(CToolBar::eWidgetLeft);
	m_pFPSLabel->setFixedWidth(600);
    connect(m_pView->getFPSManager(), SIGNAL(FPSChanged(int)), this, SLOT(onFPSChanged(int)));


	// On affiche la vue Qt
    m_pView->getRenderer()->setBackgroundColor(QVector4D(0.5, 0.5, 0.5, 1.0));
    m_pView->resize(iDefaultViewSizeX, iDefaultViewSizeY);
    m_pView->move(iDefaultViewPosX, iDefaultViewPosY);

    m_pEditShadersTextItem = new CEditShaderTextItem(m_pView->scene());
    m_pEditShadersTextItem->resize(m_pView->width() - iBorderEditItem, m_pView->height() - m_pTopToolBar->height() - m_pBottomToolBar->height() - iBorderEditItem);
	m_pEditShadersTextItem->setPos(0, m_pTopToolBar->height());
    m_pEditShadersTextItem->setStyleSheet(CQtHelper::loadStyleSheet(":/Resources/FileEdit.css"));
	m_pEditShadersTextItem->hide();

    //connect(getRenderer(), SIGNAL(sigNewCompileShadersMessage(QGLGlobal::EnumTypeMessage, const QString&)), m_pEditShadersTextItem, SLOT(onNewCompileShadersMessage(QGLGlobal::EnumTypeMessage, const QString&)));

	m_pSaveShaderButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
	m_pSaveShaderButton->setIcon(QIcon(":/Resources/Save.png"));
	m_pSaveShaderButton->setIconSize(QSize(32, 32));
    m_pSaveShaderButton->hide();

	connect(m_pSaveShaderButton, SIGNAL(clicked()), m_pEditShadersTextItem, SLOT(onSaveShader()));

    if (bActiveTreeWidget)
    {
        m_pTreeWidget = new CNodeTreeWidget(m_pSceneManager, m_pView, m_pEditShadersTextItem);
        m_pTreeWidget->resize(iTreeWidgetWidth, m_pView->height() - m_pTopToolBar->height() - m_pBottomToolBar->height());
        m_pTreeWidget->move(0, 0);
        //m_pTreeWidget->setStyleSheet(QtHelper::loadStyleSheet(":/Resources/TreeWidget.css"));

        m_pTreeWidgetDecorator = new CAnimatedPanelDecorator(m_pTreeWidget, m_pView->scene(), eLeft, m_pTopToolBar->height(), m_pTreeButton);
        m_pTreeWidgetDecorator->setStyleSheet(CQtHelper::loadStyleSheet(":/Resources/TreeWidget.css"));
        m_pTreeWidgetDecorator->setOpacity(0.8f);

        connect(m_pTreeWidgetDecorator, SIGNAL(opening()),	this, SLOT(onTreeWidgetOpen()));
        connect(m_pTreeWidgetDecorator, SIGNAL(closed()),	this, SLOT(onTreeWidgetClosing()));

        connect(m_pTreeWidget, SIGNAL(editShader(const QString&)), this, SLOT(onEditShader(const QString&)));

        connect(m_pTreeWidget, SIGNAL(playAnimation(const QString&, bool)), this, SLOT(onPlayAnimation(const QString&, bool)));
    }

#ifdef MULTITHREAD
	connect(&m_FutureWatcher, SIGNAL(finished()),						this,				SLOT(onLoadFinished()));
	connect(&m_FutureWatcher, SIGNAL(progressRangeChanged(int, int)),	m_pLoadProgressBar,	SLOT(setRange(int, int)));
	connect(&m_FutureWatcher, SIGNAL(progressValueChanged(int)),		m_pLoadProgressBar,	SLOT(setValue(int)));
#endif // MULTITHREAD

//    m_pView->setWindowTitle(QString("Labo - %1 - OpenGL %2 - GLSL %3")
//                            .arg(CGLHelper::getInstance()->getGPUName())
//                            .arg(CGLHelper::getInstance()->getOpenGLVersion())
//                            .arg(CGLHelper::getInstance()->getGLSLVersion()));

	getAnimationManager()->run();
    m_pView->getFPSManager()->setDesiredFPS(1000);
}

//-----------------------------------------------------------------------------------------
LaboApp::~LaboApp()
{
#ifdef MULTITHREAD
	m_FutureWatcher.waitForFinished();
#endif // MULTITHREAD
	delete m_pTreeWidgetDecorator;
	delete m_pTreeWidget;
}

//-----------------------------------------------------------------------------------------
// EVENEMENTS
//-----------------------------------------------------------------------------------------
void LaboApp::onMousePressed()
{
    CMouseStates mouseStates = m_pView->getMouseStates();

	if (mouseStates.isLeftButtonPressed())
	{
		m_bPressed = true;

        if (CCamera* pCamera = m_pView->getCurrentCamera())
        {
            // Picking
            if (m_pEditItemButton->isChecked() && !m_pView->isOnWidget(mouseStates.getPosition()))
            {
                QSize viewSize = m_pView->size();

                if (CMeshInstance* pMeshInstance = dynamic_cast<CMeshInstance*>(m_pSceneManager->pick(pCamera, mouseStates.getPosition(), viewSize)))
                {
                    if (m_pSelectedBoundingBox)
                    {
                        CDebugManager::getInstance()->remove(m_pSelectedBoundingBox);
                    }
                    m_pSelectedMeshInstance = pMeshInstance;
                    m_pSelectedBoundingBox = CDebugManager::getInstance()->createBox(pMeshInstance->getNode()->getLocalAxisAlignedBoundingBox(), Color::eGreen);
                    if (m_pTreeWidget) m_pTreeWidget->selectEntity(pMeshInstance);
                }
                else
                {
                    m_pSelectedMeshInstance = 0;
                }
            }

            m_StartPan = m_LastPan = mouseStates.getPosition();
			m_vStartEye = pCamera->getEyePosition();
			m_vStartCenter = pCamera->getCenter();
			m_vStartUpVector = pCamera->getUp();
			m_vStartFOV = pCamera->getHorizontalFOV();
		}
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onMouseMoved()
{
    CMouseStates mouseStates = m_pView->getMouseStates();

	QPoint delta = mouseStates.getPosition() - m_StartPan;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
	{
		if (m_bRun)
		{
            QPoint appCenterInScreen(m_pView->x() + m_pView->width() / 2, m_pView->y() + m_pView->height() / 2);

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
void LaboApp::onMouseReleased()
{
	m_bPressed = false;
}

//-----------------------------------------------------------------------------------------
void LaboApp::onKeyPressed()
{
    CKeyboardStates keyboardStates = m_pView->getKeyboardStates();

	if (m_bRun)
	{
        if (CCamera* pCamera = m_pView->getCurrentCamera())
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
		}
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::wheel(int delta)
{
    if (CCamera* pCamera = m_pView->getCurrentCamera())
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
void LaboApp::pan(int deltax, int deltaY)
{
    if (CCamera* pCamera = m_pView->getCurrentCamera())
	{
		QVector3D t = pCamera->translation(QVector3D(deltax * 0.001, -deltaY * 0.001, 0.));

		if (m_pEditItemButton->isChecked() && m_pSelectedMeshInstance)
		{
			if (CSceneNode* pNode = m_pSelectedMeshInstance->getNode())
			{
				bool bXZ = m_pAxeXZButton->isChecked();
				bool bY = m_pAxeYButton->isChecked();
				pNode->translate(bXZ ? -t.x() * 10. : 0., bY ? -t.y() * 10. : 0., bXZ ? -t.y() * 10. : 0.);
			}
		}
		else
		{
			pCamera->setEyePosition(pCamera->getEyePosition() - t);
			pCamera->setCenter(pCamera->getCenter() - t);
		}
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::rotate(int deltax, int deltay)
{
    if (CCamera* pCamera = m_pView->getCurrentCamera())
	{
		real angleX = 0.;
		real angleY = 0.;

		if (m_pAxeYButton->isChecked())
		{
            angleX = deltax * 90. / m_pView->width();
		}
		if (m_pAxeXZButton->isChecked())
		{
            angleY = deltay * 90. / m_pView->height();
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
void LaboApp::fov(int iDelta)
{
    if (CCamera* pCamera = m_pView->getCurrentCamera())
	{
		real dNextFOV = pCamera->getHorizontalFOV() + iDelta * 0.01;

		if (dNextFOV > 0.1 && dNextFOV < (Math::Pi - 0.1))
		{
			pCamera->setHorizontalFOV(dNextFOV);
		}
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onViewSizeChanged(const QSize& size)
{
	int iViewWidth = size.width();
	int iViewHeight = size.height();

    if (CCamera* pCamera = m_pView->getCurrentCamera())
	{
		pCamera->setAspectRatio((real)iViewWidth / (real)iViewHeight);
	}

	if (m_pTreeWidget && m_pTopToolBar && m_pBottomToolBar)
	{
		m_pTreeWidget->resize(iTreeWidgetWidth, iViewHeight - m_pTopToolBar->height() - m_pBottomToolBar->height());
	}

	if (m_pEditShadersTextItem && m_pTreeWidget && m_pTopToolBar && m_pBottomToolBar)
	{
		int iTreeWidgetWidth = m_pTreeWidgetDecorator->isOpen() ? m_pTreeWidget->width() : 0;
        m_pEditShadersTextItem->resize(m_pView->width() - iTreeWidgetWidth - iBorderEditItem, m_pView->height() - m_pTopToolBar->height() - m_pBottomToolBar->height() - iBorderEditItem);
		m_pEditShadersTextItem->setPos(iTreeWidgetWidth, m_pTopToolBar->height());
	}
}

//-----------------------------------------------------------------------------------------
// CHARGEMENT D'UNE SCENE
//-----------------------------------------------------------------------------------------
void LaboApp::onLoad()
{
	m_pLoadButton->setEnabled(false);
	m_pLoadProgressBar->setVisible(true);

    QString inDirectory = m_Settings.value("InDirectory", QCoreApplication::applicationDirPath()).toString();
    QString fileName = QFileDialog::getOpenFileName(m_pView,	tr("Open 3D Model"), inDirectory, tr("*.*"));

	if (!fileName.isEmpty())
	{
        m_Settings.setValue("InDirectory", QFileInfo(fileName).canonicalPath());
        CLoadDialog loadDialog(m_pView);
        loadDialog.move(m_pView->x() + m_pView->width() / 2 - loadDialog.width() / 2, m_pView->y() + m_pView->height() / 2 - loadDialog.height() / 2);

		if (loadDialog.exec() == QDialog::Accepted)
		{
            if (loadDialog.clearMeshs())		{ m_pSceneManager->clearMeshInstances(); CMeshManager::getInstance()->clearMeshs(); }
			if (loadDialog.clearEmptyNodes())	{ m_pSceneManager->clearEmptyNodes(); }
			if (loadDialog.clearLights())		{ m_pSceneManager->clearLights(); }
			if (loadDialog.clearCameras())		{ m_pSceneManager->clearCameras(); }
			if (loadDialog.clearAnimations())	{ m_pSceneManager->clearAnimations(); }
            if (loadDialog.clearMaterials())	{ CMaterialManager::getInstance()->clearMaterials(); }
            if (loadDialog.clearShaders())		{ CShaderManager::getInstance()->clearShaders(); }
			m_bMergeMaterials = loadDialog.mergeMaterials();
			m_bRescaleMeshs = loadDialog.rescaleMeshs();
			m_bInvertXZ = loadDialog.invertXZ();

			m_pSceneManager->getRootNode()->dumpNodeTree();

#ifdef MULTITHREAD
			m_FutureWatcher.setFuture(QtConcurrent::run(this, &ShadingMachineApp::loadScene, fileName));
#else
			loadScene(fileName);
			onLoadFinished();
#endif // MULTITHREAD
		}
		else
		{
			m_pLoadButton->setEnabled(true);
			m_pLoadProgressBar->setVisible(false);
		}
	}
	else
	{
		m_pLoadButton->setEnabled(true);
		m_pLoadProgressBar->setVisible(false);
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::loadScene(const QString& fileName)
{
	QFileInfo fileInfo(fileName);

    if (fileInfo.completeSuffix() == "mesh")
	{
		QTime time;
		time.start();
        CMeshManager::getInstance()->loadMesh(fileName);
        qDebug() << "Mesh load time : " << time.elapsed();
    }
    else if (fileInfo.completeSuffix() == "meshgroup")
    {
        QTime time;
        time.start();
        CMeshManager::getInstance()->loadMeshGroup(fileName);
        qDebug() << "MeshGroup load time : " << time.elapsed();
    }
    else if (fileInfo.completeSuffix() == "xml")
    {
        CSceneNode* pNode = m_pSceneManager->getRootNode()->createChild(fileInfo.baseName());

        QTime time;
        time.start();
        CScriptManager sManager(m_pSceneManager);
        sManager.loadScript(fileName, pNode);
        qDebug() << "Script : " << time.elapsed();
    }
	else
	{
		CSceneNode* pNode = m_pSceneManager->getRootNode()->createChild(fileInfo.baseName());

		QTime time;
		time.start();
		
        if (!CAssimpImporter::mergeScene(fileName, m_pSceneManager, m_bMergeMaterials, pNode).isEmpty())
		{

			qDebug() << "Loaded node tree: " << fileInfo.baseName();
			pNode->dumpNodeTree();

			qDebug() << "Loading time :" << time.elapsed();

			if (m_bRescaleMeshs)
			{
				// On récupére la bounding box
                CBox3D bbox = pNode->getGlobalAxisAlignedBoundingBox();

				real sizeX = bbox.getMaximum().x() - bbox.getMinimum().x();
				real sizeY = bbox.getMaximum().y() - bbox.getMinimum().y();
				real sizeZ = bbox.getMaximum().z() - bbox.getMinimum().z();

				real maxSize = sizeX;
				if (sizeY > maxSize) maxSize = sizeY;
				if (sizeZ > maxSize) maxSize = sizeZ;

				// On redimensionne la scene
				pNode->scale(1. / maxSize);

                CBox3D scaledBbox = pNode->getGlobalAxisAlignedBoundingBox();

				pNode->translate(-scaledBbox.getCenter());
			}

			if (m_bInvertXZ)
			{
				// On effectue une rotation du modèle
				pNode->rotate(QVector3D(1.0, 0.0, 0.0), -90.);
			}
		}
        else
        {
            qDebug() << "Error loading : " << fileName;
        }
	}
}

//-----------------------------------------------------------------------------------------
// GESTION DES ACTIONS BOUTONS
//-----------------------------------------------------------------------------------------
void LaboApp::onLoadFinished()
{
	m_pLoadProgressBar->reset();
	m_pLoadProgressBar->setVisible(false);

    if (m_pTreeWidget) m_pTreeWidget->updateData();
	//m_pSceneManager->getRootNode()->dumpNodeTree();
	m_pLoadButton->setEnabled(true);
    m_pSaveButton->setEnabled(true);

	// On crée une camera
    if (!m_pView->getCurrentCamera())
    {
        CCamera* pCamera = getSceneManager()->createCamera();
        pCamera->setEyePosition(QVector3D(2., 1., 2.));
        pCamera->setCenter(QVector3D(0., 0., 0.));
        pCamera->setAspectRatio((real)m_pView->width() / (real)m_pView->height());

        // On la définie comme caméra courante
        m_pView->setCurrentCamera(pCamera);
    }
}

//-----------------------------------------------------------------------------------------
void LaboApp::onFullScreen()
{
	m_bFullScreen = !m_bFullScreen;

	if (m_bFullScreen)
	{
        m_pView->showFullScreen();
		m_pFullScreenButton->setIcon(QIcon(":/Resources/ExitFullScreen.png"));
        m_pView->setStyleSheet("QGraphicsView { border-style: none; }");
	}
	else
	{
        m_pView->showNormal();
		m_pFullScreenButton->setIcon(QIcon(":/Resources/FullScreen.png"));
        m_pView->setStyleSheet("");
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onRunEditSwitch()
{
	//static bool bWasInFullScreen = false;

	m_bRun = !m_bRun;

    if (m_pTreeWidget) m_pTreeWidget->setVisible(!m_bRun);
	m_pEditShadersTextItem->setVisible(!m_bRun && m_bIsEditingShaders);
	//m_pBottomToolBar->setVisible(!m_bRun);
	m_pTopToolBar->setVisible(!m_bRun);

	if (m_bRun)
	{
		//bWasInFullScreen = m_bFullScreen;
		//showFullScreen();
        QCursor::setPos(m_pView->x() + m_pView->width() / 2, m_pView->y() + m_pView->height() / 2);
		qApp->setOverrideCursor(QCursor(Qt::BlankCursor));
	}
	else
	{
		//if (!bWasInFullScreen) showNormal();
		qApp->restoreOverrideCursor();
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onOptionChanged()
{
	if (QPushButton* pSender = dynamic_cast<QPushButton*>(sender()))
	{
		if		(pSender == m_pZoomButton)		m_eCurrentOption = eOptionZoom;
		else if (pSender == m_pRotateButton)	m_eCurrentOption = eOptionRotate;
		else if (pSender == m_pPanButton)		m_eCurrentOption = eOptionPan;
		else if (pSender == m_pFOVButton)		m_eCurrentOption = eOptionFOV;
		else if (pSender == m_pResetButton)
		{
            if (CCamera* pCamera = m_pView->getCurrentCamera())
			{
				pCamera->setEyePosition(QVector3D(2., 1., 2.));
				pCamera->setCenter(QVector3D(0., 0., 0.));
                pCamera->setHorizontalFOV(Math::Pi / 4.);
			}
		}
        else if (pSender == m_pSaveButton)
        {
            QString outDirectory = m_Settings.value("OutDirectory", QCoreApplication::applicationDirPath()).toString();
            QString fileName = QFileDialog::getSaveFileName(m_pView,	tr("Save 3D Scene"), outDirectory, tr("*.xml"));

            if (!fileName.isEmpty())
            {
                outDirectory = QFileInfo(fileName).canonicalPath();
                m_Settings.setValue("OutDirectory", outDirectory);

                CSaveDialog saveDialog(m_pView);
                saveDialog.move(m_pView->x() + m_pView->width() / 2 - saveDialog.width() / 2, m_pView->y() + m_pView->height() / 2 - saveDialog.height() / 2);

                if (saveDialog.exec() == QDialog::Accepted)
                {
                    m_pLoadButton->setEnabled(false);
                    m_pSaveButton->setEnabled(false);
                    m_pLoadProgressBar->setVisible(true);

                    CScriptManager sManager(m_pSceneManager);
                    sManager.saveScript(fileName);

                    m_pLoadProgressBar->reset();
                    m_pLoadProgressBar->setVisible(false);

                    m_pLoadButton->setEnabled(true);
                    m_pSaveButton->setEnabled(true);

                }
            }
        }

		m_pAxeXZButton->setVisible(m_eCurrentOption == LaboApp::eOptionRotate || m_eCurrentOption == LaboApp::eOptionPan);
		m_pAxeYButton->setVisible(m_eCurrentOption == LaboApp::eOptionRotate || m_eCurrentOption == LaboApp::eOptionPan);
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onEditShadersClicked()
{
	m_bIsEditingShaders = m_pEditShadersButton->isChecked();

	m_pEditShadersTextItem->setVisible(m_bIsEditingShaders);

	if (!m_bIsEditingShaders) m_pTopToolBar->setTitle("");


	m_pEditVSButton->setVisible(m_bIsEditingShaders);
	m_pEditGSButton->setVisible(m_bIsEditingShaders);
	m_pEditFSButton->setVisible(m_bIsEditingShaders);
	m_pSaveShaderButton->setVisible(m_bIsEditingShaders);

	m_pAxeXZButton->setVisible(!m_bIsEditingShaders);
	m_pAxeYButton->setVisible(!m_bIsEditingShaders);
	m_pEditItemButton->setVisible(!m_bIsEditingShaders);
	m_pLoadButton->setVisible(!m_bIsEditingShaders);
	m_pZoomButton->setVisible(!m_bIsEditingShaders);
	m_pRotateButton->setVisible(!m_bIsEditingShaders);
	m_pPanButton->setVisible(!m_bIsEditingShaders);
	m_pFOVButton->setVisible(!m_bIsEditingShaders);
	m_pResetButton->setVisible(!m_bIsEditingShaders);
}

//-----------------------------------------------------------------------------------------
void LaboApp::onEditShader(const QString& name)
{
	m_pEditShadersButton->setChecked(true);
	m_pEditVSButton->setChecked(true);
	m_pEditGSButton->setChecked(false);
	m_pEditFSButton->setChecked(false);

	onEditShadersClicked();

    if(CShaderManager::getInstance()->isShaderExist(name))
	{
        m_pSDEdited = CShaderManager::getInstance()->getShaderByName(name);
		m_pTopToolBar->setTitle(m_pSDEdited->getName() + " Vertex Shader (" + m_pSDEdited->getVertexShaderFileName() + ")");
		m_pEditShadersTextItem->onEditShader(m_pSDEdited);
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onPlayAnimation(const QString& name, bool bPlay)
{
	if (CAnimation* pAnimation = getSceneManager()->getAnimationByName(name))
	{
		bPlay ? pAnimation->start() : pAnimation->stop();
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onTreeWidgetOpen()
{
	if (m_pEditShadersTextItem && m_pTreeWidget && m_pTopToolBar && m_pBottomToolBar)
	{
        m_pEditShadersTextItem->resize(m_pView->width() - m_pTreeWidget->width() - iBorderEditItem, m_pView->height() - m_pTopToolBar->height() - m_pBottomToolBar->height() - iBorderEditItem);
		m_pEditShadersTextItem->setPos(m_pTreeWidget->width(), m_pTopToolBar->height());
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onTreeWidgetClosing()
{
	if (m_pEditShadersTextItem && m_pTreeWidget && m_pTopToolBar && m_pBottomToolBar)
	{
        m_pEditShadersTextItem->resize(m_pView->width() - iBorderEditItem, m_pView->height() - m_pTopToolBar->height() - m_pBottomToolBar->height() - iBorderEditItem);
		m_pEditShadersTextItem->setPos(0, m_pTopToolBar->height());
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onTimeout()
{
	//m_pCenterNode->rotate(QVector3D(0, 1, 0), 0.5f);
}

//-----------------------------------------------------------------------------------------
void LaboApp::onVSButtonClicked()
{
	if (m_pSDEdited)
	{
		if (m_pSDEdited->hasVertexShader())
		{
			m_pTopToolBar->setTitle(m_pSDEdited->getName() + "  - Vertex Shader (" + m_pSDEdited->getVertexShaderFileName() + ")");
		}
		else
		{
			m_pTopToolBar->setTitle(m_pSDEdited->getName() + " - Vertex Shader");
		}

		m_pEditShadersTextItem->setShaderType(eVertexShader);
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onGSButtonClicked()
{
	if (m_pSDEdited)
	{
		if (m_pSDEdited->hasGeometryShader())
		{
			m_pTopToolBar->setTitle(m_pSDEdited->getName() + " - Geometry Shader (" + m_pSDEdited->getGeometryShaderFileName() + ")");
		}
		else
		{
			m_pTopToolBar->setTitle(m_pSDEdited->getName() + " - Geometry Shader");
		}

		m_pEditShadersTextItem->setShaderType(eGeometryShader);	
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onFSButtonClicked()
{
	if (m_pSDEdited)
	{
		if (m_pSDEdited->hasFragmentShader())
		{
			m_pTopToolBar->setTitle(m_pSDEdited->getName() + " - Fragment Shader (" + m_pSDEdited->getFragmentShaderFileName() + ")");
		}
		else
		{
			m_pTopToolBar->setTitle(m_pSDEdited->getName() + " - Fragment Shader");
		}

		m_pEditShadersTextItem->setShaderType(eFragmentShader);
	}
}

//-----------------------------------------------------------------------------------------
void LaboApp::onFPSChanged(int iFPS)
{
    m_pFPSLabel->setText(QString("%1 frame/s - Draw calls: %2 - Polygons: %3")
		.arg(iFPS)
        .arg(m_pView->getGLRenderer()->getDrawCallCount())
        .arg(m_pView->getGLRenderer()->getPolygonCount()));
}

//-----------------------------------------------------------------------------------------
/*void LaboApp::keyPressEvent(QKeyEvent* pEvent)
{
	if (pEvent->key() == Qt::Key_R)
	{
		if (CGLRenderer* pRender = dynamic_cast<CGLRenderer*>(m_pRenderer))
		{
			pRender->rebuildShaders();
		}
	}

    CGraphicsView3D::keyPressed(pEvent);
}*/
