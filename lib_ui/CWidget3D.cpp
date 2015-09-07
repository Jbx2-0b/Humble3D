#include "CWidget3D.h"
#include <QMouseEvent>
#include "CQtHelper.h"
#include "CSceneManager.h"


//-----------------------------------------------------------------------------------------
CWidget3D::CWidget3D(CSceneManager* pSceneManager, CCamera* pCamera, QWidget *parent)
: CGLWidget(parent)
, AView(pSceneManager)
, m_bForceUpdate(false)
{
    m_pGLRenderer = new CGLRenderer(m_pSceneManager);
	m_pRenderer = m_pGLRenderer;

    m_pRenderer->setCurrentCamera(pCamera);
	
	setMouseTracking(true);
    connect(&m_FPSManager, SIGNAL(FPSTimeout()), this, SLOT(onTimeOut()));

    m_pGLRenderer->run();
}

//-----------------------------------------------------------------------------------------
CWidget3D::~CWidget3D()
{
}

//-----------------------------------------------------------------------------------------
void CWidget3D::setForceUpdate(bool bForceUpdate)
{
    m_bForceUpdate = bForceUpdate;
}

//-----------------------------------------------------------------------------------------
bool CWidget3D::event(QEvent* pEvent)
{
    switch (pEvent->type())
    {
        case QEvent::TouchBegin:
        {
            QTouchEvent* pTouchEvent = static_cast<QTouchEvent*>(pEvent);
            QList<QTouchEvent::TouchPoint> points = pTouchEvent->touchPoints();
            if (points.count() == 2)
            {
                emit touchScaleStarted();
            }

            return true;
        }
        break;

        case QEvent::TouchUpdate:
        {
            QTouchEvent* pTouchEvent = static_cast<QTouchEvent*>(pEvent);
            QList<QTouchEvent::TouchPoint> points = pTouchEvent->touchPoints();
            if (points.count() == 2)
            {
                QTouchEvent::TouchPoint p0 = points.first();
                QTouchEvent::TouchPoint p1 = points.last();
                QLineF line1(p0.startPos(), p1.startPos());
                QLineF line2(p0.pos(), p1.pos());
                real dScaleFactor = line2.length() / line1.length();
                emit touchScaleChanged(dScaleFactor);

                return true;
            }
        }
        break;

        case QEvent::TouchEnd:
        {
            QTouchEvent* pTouchEvent = static_cast<QTouchEvent*>(pEvent);
            QList<QTouchEvent::TouchPoint> points = pTouchEvent->touchPoints();
            if (points.count() == 2)
            {
                emit touchScaleEnded();
            }

            return true;
        }
        break;

        default:
        break;
    }

    return CGLWidget::event(pEvent);
}

//-----------------------------------------------------------------------------------------
void CWidget3D::mouseMoveEvent(QMouseEvent* pEvent)
{
	m_MouseStates = CQtHelper::fromMouseEvent(pEvent);
    m_MouseMoveFilter.filter();

    CGLWidget::mouseMoveEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CWidget3D::onUpdate(CFilterEvent* /*pFilter*/)
{
    emit mouseMoved();
}
//-----------------------------------------------------------------------------------------
void CWidget3D::mousePressEvent(QMouseEvent* pEvent)
{
	m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    emit mousePressed();

    CGLWidget::mousePressEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CWidget3D::mouseReleaseEvent(QMouseEvent* pEvent)
{
	m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    emit mouseReleased();

    CGLWidget::mouseReleaseEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CWidget3D::mouseDoubleClickEvent(QMouseEvent* pEvent)
{
	m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    emit mouseDoubleClicked();

    CGLWidget::mouseDoubleClickEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CWidget3D::initializeGL()
{
    //qglClearColor(Qt::black);
    m_pRenderer->setBackgroundColor(QVector4D(m_BackgroundColor.redF(),
                                              m_BackgroundColor.greenF(),
                                              m_BackgroundColor.blueF(),
                                              m_BackgroundColor.alphaF()));

    m_pRenderer->init();
	m_pRenderer->setDirty();

    //    // Enable depth buffer
    //    glEnable(GL_DEPTH_TEST);

    //    // Enable back face culling
    //    glEnable(GL_CULL_FACE);

    CGLWidget::initializeGL();

    m_FPSManager.start();
}

//-----------------------------------------------------------------------------------------
void CWidget3D::onTimeOut()
{
    if (m_pRenderer->isDirty() || m_bForceUpdate)
	{
		update();
	}
}

//-----------------------------------------------------------------------------------------
void CWidget3D::resizeGL(int iWidth, int iHeight)
{
    m_pRenderer->setResolution(iWidth, iHeight);

    if (CCamera* pCamera = m_pRenderer->getCurrentCamera())
	{
		pCamera->setAspectRatio((real)iWidth / (real)iHeight);
	}

	emit sizeChanged(QSize(iWidth, iHeight));

    CGLWidget::resizeGL(iWidth, iHeight);
}

//-----------------------------------------------------------------------------------------
void CWidget3D::paintGL()
{
	m_pRenderer->setDirty();
    m_pRenderer->render();
    m_FPSManager.incrFPS();
}

