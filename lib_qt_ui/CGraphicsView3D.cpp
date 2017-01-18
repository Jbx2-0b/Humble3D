#include "CGraphicsView3D.h"

// Foundations
#include "CCamera.h"
#include "CGLRenderer.h"
#include "CSceneManager.h"
#include "CLogManager.h"
#include "CGraphicsWidgetItem.h"
#include "CQtHelper.h"

// Qt
#include <QDesktopWidget>
#include <QGLWidget>
#include <QPolygonF>
#include <QtAlgorithms>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QDebug>


//-----------------------------------------------------------------------------------------
CGraphicsView3D::CGraphicsView3D(CSceneManager* pSceneManager, CCamera* pCamera, QWidget* pParent /*= 0*/)
    : QGraphicsView(pParent)
    , AView(pSceneManager)
{
    m_pGLRenderer = new CGLRenderer(m_pSceneManager);
    m_pRenderer = m_pGLRenderer;
    m_pRenderer->setCurrentCamera(pCamera);
    m_pGLWidget = new CGLWidget();

    setFrameShape(QFrame::NoFrame);
    setViewport(m_pGLWidget);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    m_pGraphicsScene = new CGraphicsScene3D(&m_FPSManager);
    setScene(m_pGraphicsScene);

    m_pGraphicsScene->setRenderer(m_pGLRenderer);

#ifndef USE_QT_OPENGL
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    resize(DefaultResolutionWidth, DefaultResolutionHeight);
#else
    resize(QApplication::desktop()->screenGeometry().size());
#endif

    m_pGLRenderer->run();
}

//-----------------------------------------------------------------------------------------
CGraphicsView3D::~CGraphicsView3D()
{
    for (QWidget* pWidget : m_GraphicsWidgetItems.keys())
    {
        removeWidget(pWidget);
    }   
}

//-----------------------------------------------------------------------------------------
// EVENEMENTS
//-----------------------------------------------------------------------------------------
void CGraphicsView3D::mouseMoveEvent(QMouseEvent* pEvent)
{
    m_MouseStates = CQtHelper::fromMouseEvent(pEvent);
    m_MouseMoveFilter.filter();

    // on remonte les evenements
    QGraphicsView::mouseMoveEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CGraphicsView3D::onUpdate(CFilterEvent* /*pFilter*/)
{
    emit mouseMoved();
}

//-----------------------------------------------------------------------------------------
void CGraphicsView3D::mousePressEvent(QMouseEvent* pEvent)
{
    m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    if (!isOnWidget(pEvent->pos()))
    {
        emit mousePressed();
    }

    // on remonte les evenements
    QGraphicsView::mousePressEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CGraphicsView3D::mouseReleaseEvent(QMouseEvent* pEvent)
{
    m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    emit mouseReleased();

    // on remonte les evenements
    QGraphicsView::mouseReleaseEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CGraphicsView3D::mouseDoubleClickEvent(QMouseEvent* pEvent)
{
    m_MouseStates = CQtHelper::fromMouseEvent(pEvent);

    if (!isOnWidget(pEvent->pos()))
    {
        emit mouseDoubleClicked();
    }

    // on remonte les evenements
    QGraphicsView::mousePressEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
//bool CGraphicsView3D::event(QEvent* pEvent)
//{
//    switch (pEvent->type())
//    {
//        case QEvent::TouchBegin:
//        {
//            QTouchEvent* pTouchEvent = static_cast<QTouchEvent*>(pEvent);
//            QList<QTouchEvent::TouchPoint> points = pTouchEvent->touchPoints();
//            if (points.count() == 2)
//            {
//                emit touchScaleStarted();
//            }

//            return true;
//        }
//        break;

//        case QEvent::TouchUpdate:
//        {
//            QTouchEvent* pTouchEvent = static_cast<QTouchEvent*>(pEvent);
//            QList<QTouchEvent::TouchPoint> points = pTouchEvent->touchPoints();
//            if (points.count() == 2)
//            {
//                QTouchEvent::TouchPoint p0 = points.first();
//                QTouchEvent::TouchPoint p1 = points.last();
//                QLineF line1(p0.startPos(), p1.startPos());
//                QLineF line2(p0.pos(), p1.pos());
//                real dScaleFactor = line2.length() / line1.length();
//                emit touchScaleChanged(dScaleFactor);

//                return true;
//            }
//        }
//        break;

//        case QEvent::TouchEnd:
//        {
//            QTouchEvent* pTouchEvent = static_cast<QTouchEvent*>(pEvent);
//            QList<QTouchEvent::TouchPoint> points = pTouchEvent->touchPoints();
//            if (points.count() == 2)
//            {
//                emit touchScaleEnded();
//            }

//            return true;
//        }
//        break;

//        default:
//        break;
//    }

//    return CGLView::event(pEvent);
//}

//-----------------------------------------------------------------------------------------
void CGraphicsView3D::wheelEvent(QWheelEvent* pEvent)
{
    if (!isOnWidget(pEvent->pos()))
    {
        emit mouseWheelMoved();
    }
}

//-----------------------------------------------------------------------------------------
void CGraphicsView3D::resizeEvent(QResizeEvent* pEvent)
{
    scene()->setSceneRect(QRect(QPoint(0, 0), pEvent->size()));

    if (CCamera* pCamera = m_pRenderer->getCurrentCamera())
    {
        pCamera->setAspectRatio((real)pEvent->size().width() / (real)pEvent->size().height());
    }

    m_pRenderer->setResolution(pEvent->size().width(), pEvent->size().height());

    emit sizeChanged(pEvent->size());

    // on remonte les evenements
    QGraphicsView::resizeEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CGraphicsView3D::keyPressEvent(QKeyEvent* pEvent)
{
    m_KeyboardStates = CQtHelper::fromQKeyEvent(pEvent);

    emit keyPressed();

    // on remonte les evenements
    QGraphicsView::keyPressEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
void CGraphicsView3D::keyReleaseEvent(QKeyEvent* pEvent)
{
    m_KeyboardStates = CQtHelper::fromQKeyEvent(pEvent);

    emit keyReleased();

    // on remonte les evenements
    QGraphicsView::keyReleaseEvent(pEvent);
}

//-----------------------------------------------------------------------------------------
bool CGraphicsView3D::isOnWidget(const QPoint& point)
{
    // on recupere l'ensemble des objets sous le pointeur souris
    QList<QGraphicsItem *> itemsList = scene()->items(point);

    // on prend seulement le premier element de la liste , c'est celui qui a la plus grande valeur Z
    QList<QGraphicsItem *>::const_iterator i = itemsList.constBegin();

    //! Selection d'un objet existant
    if (i != itemsList.constEnd())
    {
        if (dynamic_cast<QGraphicsProxyWidget*>(*i))
        {
            return true;
        }
    }

    return false;
}

//-----------------------------------------------------------------------------------------
CToolBar* CGraphicsView3D::createToolBar(CToolBar::EnumToolBarPosition eToolBarPosition)
{
    QSharedPointer<CGraphicsWidgetItem> pItem = QSharedPointer<CGraphicsWidgetItem>::create();
    CToolBar* pToolBar = new CToolBar(eToolBarPosition, size(), pItem);
    pItem->setWidget(pToolBar);
    pToolBar->setAttribute(Qt::WA_TranslucentBackground);
    m_GraphicsWidgetItems.insert(pToolBar, pItem);
    m_pGraphicsScene->addItem(pItem.data());
    connect(this, SIGNAL(sizeChanged(QSize)), pToolBar, SLOT(onViewSizeChanged(QSize)));
    return pToolBar;
}

//-----------------------------------------------------------------------------------------
void CGraphicsView3D::removeWidget(QWidget* pWidget)
{
    if (m_GraphicsWidgetItems.contains(pWidget))
    {
        QSharedPointer<CGraphicsWidgetItem> pItem = m_GraphicsWidgetItems.value(pWidget);
        m_GraphicsWidgetItems.remove(pWidget);
        m_pGraphicsScene->removeItem(pItem.data());
    }
}

