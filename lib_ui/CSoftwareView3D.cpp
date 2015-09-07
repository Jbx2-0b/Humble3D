#include "CSoftwareView3D.h"
#include "CSoftwareRenderer.h"
#include "CSceneManager.h"

#include <QPainter>
#include <QResizeEvent>


//-----------------------------------------------------------------------------------------
CSoftwareView3D::CSoftwareView3D(CSceneManager* pSceneManager, CCamera* pCamera, QWidget* parent)
    : QWidget(parent)
    , AView(pSceneManager)
{
    m_pSoftwareRenderer = new CSoftwareRenderer(m_pSceneManager);
    m_pRenderer = m_pSoftwareRenderer;
    m_pRenderer->setCurrentCamera(pCamera);
    m_pSoftwareRenderer->setBackgroundColor(CQtHelper::toColor(m_BackgroundColor));

    connect(&m_FPSManager, SIGNAL(FPSTimeout()), this, SLOT(onTimeOut()));
    m_FPSManager.start();

    m_pSoftwareRenderer->run();
}

//-----------------------------------------------------------------------------------------
CSoftwareView3D::~CSoftwareView3D()
{

}

//-----------------------------------------------------------------------------------------
bool CSoftwareView3D::event(QEvent* pEvent)
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

    return QWidget::event(pEvent);
}

//-----------------------------------------------------------------------------------------
void CSoftwareView3D::mouseMoveEvent(QMouseEvent* pEvent)
{
    m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    m_MouseMoveFilter.filter();

    QWidget::mouseMoveEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CSoftwareView3D::onUpdate(CFilterEvent* /*pFilter*/)
{
    emit mouseMoved();
}

//-----------------------------------------------------------------------------------------
void CSoftwareView3D::mousePressEvent(QMouseEvent* pEvent)
{
    m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    emit mousePressed();

    QWidget::mousePressEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CSoftwareView3D::mouseReleaseEvent(QMouseEvent* pEvent)
{
    m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    emit mouseReleased();

    QWidget::mouseReleaseEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CSoftwareView3D::mouseDoubleClickEvent(QMouseEvent* pEvent)
{
    m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    emit mouseDoubleClicked();

    QWidget::mouseDoubleClickEvent(pEvent);
}
//-----------------------------------------------------------------------------------------
void CSoftwareView3D::onTimeOut()
{
    m_FPSManager.incrFPS();

    if (m_pRenderer->isDirty())
    {
        m_pRenderer->render();
        update();
    }
}

//-----------------------------------------------------------------------------------------
void CSoftwareView3D::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawImage(QRect(0, 0, width(), height()), m_pSoftwareRenderer->getImage());
    painter.setPen(Qt::red);
    painter.drawText(5, 10, QString::number(m_FPSManager.getFPSCount()));
}

//-----------------------------------------------------------------------------------------
void CSoftwareView3D::resizeEvent(QResizeEvent* pEvent)
{
    if (CCamera* pCamera = m_pRenderer->getCurrentCamera())
    {
        pCamera->setAspectRatio((real)width() / (real)height());
    }

    m_pSoftwareRenderer->setResolution(pEvent->size().width(), pEvent->size().height());
    m_pRenderer->setDirty();
    m_pRenderer->render();

    emit sizeChanged(pEvent->size());
}
