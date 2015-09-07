// Foundations
#include "CGraphicsScene3D.h"
#include "CMath.h"
#include "CLogManager.h"

// Qt
#include <QTimer>
#include <QPainter>
#include <QPaintEngine>

//-----------------------------------------------------------------------------------------
CGraphicsScene3D::CGraphicsScene3D(CFPSManager* pFPSManager, QObject* pParent /*= 0*/)
    : QGraphicsScene(pParent)
    , m_pRenderer(0)
    , m_pFPSManager(pFPSManager)
{   
#ifndef USE_QT_OPENGL
    setSceneRect(QRect(QPoint(0, 0), QSize(DefaultResolutionWidth, DefaultResolutionHeight)));
#endif

    connect(m_pFPSManager, SIGNAL(FPSTimeout()),	this, SLOT(onTimeout()));

    m_pFPSManager->start();
}


//-----------------------------------------------------------------------------------------
CGraphicsScene3D::~CGraphicsScene3D()
{
}

//-----------------------------------------------------------------------------------------
void CGraphicsScene3D::onTimeout()
{
    if (m_pRenderer->isEnabled())
    {
        update();
    }
}

//-----------------------------------------------------------------------------------------
void CGraphicsScene3D::drawBackground(QPainter* painter, const QRectF& rect)
{
    bool bInit = true;

    if (!m_pRenderer->isInit())
    {
        bInit = m_pRenderer->init();
    }

    if (bInit)
    {
        if (m_pRenderer->isEnabled())
        {
            painter->beginNativePainting();

            CRenderStates& renderStates = m_pRenderer->renderStates();

            renderStates.depthTest().setEnabled(true);
            renderStates.faceCulling().setEnabled(true);

            m_pRenderer->onUpdateRenderStates();
            m_pRenderer->setDirty();
            m_pRenderer->render();

            renderStates.depthTest().setEnabled(false);
            renderStates.faceCulling().setEnabled(false);
            m_pRenderer->onUpdateRenderStates();

            m_pFPSManager->incrFPS();

            painter->endNativePainting();
        }
    }
    else
    {
        pLog->addMessage(eFATAL, "Unable to initialize renderer, OpenGL functions missing...");
        qApp->exit();
    }

    QGraphicsScene::drawBackground(painter, rect);
}

//-----------------------------------------------------------------------------------------
void CGraphicsScene3D::setRenderer(CGLRenderer* pRenderer)
{
    m_pRenderer = pRenderer;
}
