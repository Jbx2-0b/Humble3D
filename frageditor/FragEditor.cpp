#include "FragEditor.h"

#include <QStringList>
#include <QFileDialog>
#include <QCoreApplication>

#include "CCamera.h"
#include "CSceneManager.h"
#include "CQtHelper.h"
#include "CGLGlobal.h"
#include "CGeometryGlobal.h"
#include "Shapes/CPlaneMesh.h"
#include "Math.h"
#include "CGraphicsView3D.h"



//-----------------------------------------------------------------------------------------
FragEditor::FragEditor()
    : m_pEditShadersTextItem(0)
    , m_pTopToolBar(0)
    , m_pBottomToolBar(0)
    , m_bFullScreen(false)
    , m_iMouseX(0)
    , m_iMouseY(0)
    , m_bMouseLeftPressed(false)
    , m_bMouseRightPressed(false)
    , m_LastPan(-1, -1)
    , m_StartPan(-1, -1)
    , m_vStartFOV(Math::Pi / 4.)
    , m_pSDEdited(0)
{
    // On crée une camera
    CCamera* pCamera = getSceneManager()->createCamera();
    pCamera->setProjectionType(eProjectionOrtho);
    pCamera->setOrtho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 2.f);

    pCamera->setEyePosition(QVector3D(0., 0., -1.));
    pCamera->setCenter(QVector3D(0., 0., 0.));
    pCamera->setAspectRatio((real) iDefaultViewSizeX / (real) iDefaultViewSizeY);

    m_pView = createGraphicsView3D(pCamera);

    m_pView->getFPSManager()->setDesiredFPS(60);
    //m_pView->getGLRenderer()->registerListener(this);


    CPlaneMesh* pPlaneMesh = CMeshManager::getInstance().createCustomMesh<CPlaneMesh>("CPlaneMesh", "PlaneMesh");
    pPlaneMesh->init(CPlaneMesh::ePlaneXY, 1, 1, 1, 1);

    CMeshInstance* pPlane = m_pSceneManager->createMeshInstance(pPlaneMesh, "PlaneMesh");

#ifdef EMBEDDED_TARGET
    m_pSDEdited = CShaderManager::getInstance().createShader("Edited", ":/Resources/defaultES.vertex.glsl", "", ":/Resources/defaultES.fragment.glsl");
#else
    m_pSDEdited = CShaderManager::getInstance().createShader("Edited", ":/Resources/default.vertex.glsl", "", ":/Resources/default.fragment.glsl");
#endif

    m_pSDEdited->setUniformValue("iResolution", QVector3D(iDefaultViewSizeX, iDefaultViewSizeY, 0));
    m_pSDEdited->setUniformValue("iMouse", QVector4D(m_iMouseX, m_iMouseY, m_bMouseLeftPressed, m_bMouseRightPressed));

    CMaterial* pMat = CMaterialManager::getInstance().createMaterial("GridMat");
    pMat->getRenderPass(0)->setShaderName(m_pSDEdited->getName());
    pPlane->setMaterialName(pMat->getName());

    for (int i = 0; i < 4; ++i)
    {
        CTexture2D* pTexture = CTextureManager::getInstance().createTexture2D("iChannel" + QString::number(i));
        pMat->addTexture(pTexture, eDiffuse);
        m_pSDEdited->setUniformValue("iChannel" + QString::number(i), pTexture->getTextureUnit());
        m_pSDEdited->setUniformValue(QString("iChannelResolution[%1]").arg(i), QVector3D(0., 0., 0.));
    }

    CSceneNode* pRootNode = getSceneManager()->getRootNode();
    pRootNode->addItem(pPlane);

    m_pTopToolBar = m_pView->createToolBar();

    m_pLoadButton = m_pTopToolBar->createWidget<QPushButton>();
    m_pLoadButton->setIcon(QIcon(":/Resources/Open.png"));
    m_pLoadButton->setIconSize(QSize(32, 32));
    connect(m_pLoadButton, SIGNAL(pressed()), this, SLOT(onLoad()));

    m_pSaveButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
    m_pSaveButton->setIcon(QIcon(":/Resources/Save.png"));
    m_pSaveButton->setIconSize(QSize(32, 32));
    connect(m_pSaveButton, SIGNAL(pressed()), this, SLOT(onSave()));

    m_pEditShadersButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
    m_pEditShadersButton->setIcon(QIcon(":/Resources/EditText.png"));
    m_pEditShadersButton->setIconSize(QSize(32, 32));
    m_pEditShadersButton->setCheckable(true);
    connect(m_pEditShadersButton,	SIGNAL(clicked()), this, SLOT(onEditShaderClicked()));

    m_pSelectTextureButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
    m_pSelectTextureButton->setIcon(QIcon(":/Resources/Edit2.png"));
    m_pSelectTextureButton->setIconSize(QSize(32, 32));
    m_pSelectTextureButton->setCheckable(true);
    connect(m_pSelectTextureButton,	SIGNAL(clicked()), this, SLOT(onSelectTextureClicked()));

    m_pStopRenderingButton = m_pTopToolBar->createWidget<QPushButton>(CToolBar::eWidgetLeft);
    m_pStopRenderingButton->setIcon(QIcon(":/Resources/Play.png"));
    m_pStopRenderingButton->setIconSize(QSize(32, 32));
    m_pStopRenderingButton->setCheckable(true);
    m_pStopRenderingButton->setChecked(true);
    connect(m_pStopRenderingButton,	SIGNAL(clicked()), this, SLOT(onStopRenderingClicked()));

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

    m_pBottomToolBar = m_pView->createToolBar(CToolBar::eToolBarBottom);
    m_pBottomToolBar->setStyleSheetFile(":/Resources/ToolBar.css");
    m_pBottomToolBar->setOpacity(0.5);
    m_pBottomToolBar->setThickness(30);

    m_pFPSLabel = m_pBottomToolBar->createWidget<QLabel>(CToolBar::eWidgetLeft);
    m_pFPSLabel->setFixedWidth(100);
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
    m_pEditShadersTextItem->onEditShader(m_pSDEdited);
    m_pEditShadersTextItem->setShaderType(eFragmentShader);

    m_pImgSelectorItem = new CImageSelectorItem(m_pView->scene(), pMat, m_pSDEdited);
    m_pImgSelectorItem->resize(m_pView->width() - iBorderEditItem, m_pView->height() - m_pTopToolBar->height() - m_pBottomToolBar->height() - iBorderEditItem);
    m_pImgSelectorItem->setPos(0, m_pTopToolBar->height());
    m_pImgSelectorItem->setStyleSheet(CQtHelper::loadStyleSheet(":/Resources/FileEdit.css"));
    m_pImgSelectorItem->hide();

    connect(&m_SleepToolBarTimer, SIGNAL(timeout()), this, SLOT(onSleepToolBarTimeout()));

    //    m_pView->setWindowTitle(QString("FragEditor - %1 - OpenGL %2 - GLSL %3")
    //                            .arg(CGLHelper::getInstance().getGPUName())
    //                            .arg(CGLHelper::getInstance().getOpenGLVersion())
    //                            .arg(CGLHelper::getInstance().getGLSLVersion()));

    connect(m_pView, SIGNAL(mouseMoved()),			this, SLOT(onMouseMoved()));
    connect(m_pView, SIGNAL(mouseReleased()),		this, SLOT(onMouseReleased()));
    connect(m_pView, SIGNAL(mousePressed()),		this, SLOT(onMousePressed()));
    connect(m_pView, SIGNAL(sizeChanged(QSize)),	this, SLOT(onViewSizeChanged(QSize)));

}

//-----------------------------------------------------------------------------------------
FragEditor::~FragEditor()
{
    //m_pView->getGLRenderer()->unregisterListener(this);
}

//-----------------------------------------------------------------------------------------
void FragEditor::onNewMessage(bool bIsError, const QString& message)
{
    m_pEditShadersTextItem->setMessage(bIsError, message);
}

//-----------------------------------------------------------------------------------------
// EVENEMENTS
//-----------------------------------------------------------------------------------------
void FragEditor::onMousePressed()
{
    CMouseStates mouseStates = m_pView->getMouseStates();

    if (mouseStates.isLeftButtonPressed())
    {
        m_bMouseLeftPressed = true;
    }
    else if (mouseStates.isRightButtonPressed())
    {
        m_bMouseRightPressed = true;
    }

    if (!m_pImgSelectorItem->isVisible() && !m_pEditShadersTextItem->isVisible())
    {
        m_pSDEdited->setUniformValue("iMouse", QVector4D(m_iMouseX, m_iMouseY, m_bMouseLeftPressed, m_bMouseRightPressed));
        m_SleepToolBarTimer.start(3000);
    }

    if (m_pTopToolBar->getState() == CToolBar::eClosed) m_pTopToolBar->open();
    if (m_pBottomToolBar->getState() == CToolBar::eClosed) m_pBottomToolBar->open();
}

//-----------------------------------------------------------------------------------------
void FragEditor::onMouseMoved()
{
    CMouseStates mouseStates = m_pView->getMouseStates();

    m_iMouseX = mouseStates.getPosition().x();
    m_iMouseY = mouseStates.getPosition().y();

    if (!m_pImgSelectorItem->isVisible() && !m_pEditShadersTextItem->isVisible())
    {
        m_pSDEdited->setUniformValue("iMouse", QVector4D(m_iMouseX, m_iMouseY, m_bMouseLeftPressed, m_bMouseRightPressed));
        m_SleepToolBarTimer.start(3000);
    }

    if (m_pTopToolBar->getState() == CToolBar::eClosed) m_pTopToolBar->open();
    if (m_pBottomToolBar->getState() == CToolBar::eClosed) m_pBottomToolBar->open();


}

//-----------------------------------------------------------------------------------------
void FragEditor::onMouseReleased()
{
    CMouseStates mouseStates = m_pView->getMouseStates();

    if (mouseStates.isLeftButtonPressed())
    {
        m_bMouseLeftPressed = false;
    }
    else if (mouseStates.isRightButtonPressed())
    {
        m_bMouseRightPressed = false;
    }

    if (!m_pImgSelectorItem->isVisible() && !m_pEditShadersTextItem->isVisible())
    {
        m_pSDEdited->setUniformValue("iMouse", QVector4D(m_iMouseX, m_iMouseY, m_bMouseLeftPressed, m_bMouseRightPressed));
        m_SleepToolBarTimer.start(3000);
    }

    if (m_pTopToolBar->getState() == CToolBar::eClosed) m_pTopToolBar->open();
    if (m_pBottomToolBar->getState() == CToolBar::eClosed) m_pBottomToolBar->open();
}

//-----------------------------------------------------------------------------------------
void FragEditor::onViewSizeChanged(const QSize& /*size*/)
{
    m_pSDEdited->setUniformValue("iResolution", QVector3D(m_pView->width(), m_pView->height(), 0));

    m_pImgSelectorItem->resize(m_pView->width() - iBorderEditItem, m_pView->height() - m_pTopToolBar->height() - m_pBottomToolBar->height() - iBorderEditItem);
    m_pEditShadersTextItem->resize(m_pView->width() - iBorderEditItem, m_pView->height() - m_pTopToolBar->height() - m_pBottomToolBar->height() - iBorderEditItem);
    m_pEditShadersTextItem->setPos(0, m_pTopToolBar->height());
}

//-----------------------------------------------------------------------------------------
// CHARGEMENT D'UN SHADER
//-----------------------------------------------------------------------------------------
void FragEditor::onLoad()
{
    m_pLoadButton->setEnabled(false);

    QString fileName = QFileDialog::getOpenFileName(m_pView,	tr("Open Fragment Shader"), QCoreApplication::applicationDirPath(), tr("*.*"));

    if (!fileName.isEmpty())
    {
        m_pSDEdited->setFragmentShaderFileName(fileName);
        m_pEditShadersTextItem->updateText();
    }

    m_pLoadButton->setEnabled(true);
    m_pSaveButton->setEnabled(true);
}

//-----------------------------------------------------------------------------------------
void FragEditor::onSave()
{
    QString fileName = QFileDialog::getSaveFileName(m_pView,	tr("Save Fragment Shader"), QCoreApplication::applicationDirPath(), tr("*.*"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << m_pSDEdited->getFragmentShaderCode();

            file.close();
        }
    }
}

//-----------------------------------------------------------------------------------------
// GESTION DES ACTIONS BOUTONS
//-----------------------------------------------------------------------------------------
void FragEditor::onFullScreen()
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
void FragEditor::onEditShaderClicked()
{
    bool bIsEditingShaders = m_pEditShadersButton->isChecked();

    m_pEditShadersTextItem->setVisible(bIsEditingShaders);

    if (!bIsEditingShaders)
    {
        m_pTopToolBar->setTitle("");
    }
    else
    {
        m_pSelectTextureButton->setChecked(false);
        m_pImgSelectorItem->setVisible(false);
    }
}

//-----------------------------------------------------------------------------------------
void FragEditor::onSelectTextureClicked()
{
    bool bIsEditingImage = m_pSelectTextureButton->isChecked();

    m_pImgSelectorItem->setVisible(bIsEditingImage);

    if (bIsEditingImage)
    {
        m_pEditShadersButton->setChecked(false);
        m_pEditShadersTextItem->setVisible(false);
    }
}

//-----------------------------------------------------------------------------------------
void FragEditor::onStopRenderingClicked()
{
    m_pView->getRenderer()->setEnabled(m_pStopRenderingButton->isChecked());
}

//-----------------------------------------------------------------------------------------
void FragEditor::onFPSChanged(int iFPS)
{
    m_pFPSLabel->setText(QString("%1 frame/s").arg(iFPS));
}

//-----------------------------------------------------------------------------------------
void FragEditor::onSleepToolBarTimeout()
{
    m_SleepToolBarTimer.stop();
    m_pTopToolBar->close();
    m_pBottomToolBar->close();
}
