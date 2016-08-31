#include "Controller.h"

//-----------------------------------------------------------------------------------------
Controller::Controller(CWidget3D* pView)
    : m_pView(pView)
    , m_bPressed(false)
    , m_bTouch(false)
    , m_StartPan(-1, -1)
    , m_LastPan(-1, -1)
{
    m_pView->setAttribute(Qt::WA_AcceptTouchEvents);

    connect(m_pView, SIGNAL(mouseMoved()), this, SLOT(onMouseMoved()));
    connect(m_pView, SIGNAL(mouseReleased()), this, SLOT(onMouseReleased()));
    connect(m_pView, SIGNAL(mousePressed()), this, SLOT(onMousePressed()));
    connect(m_pView, SIGNAL(touchScaleStarted()), this, SLOT(onTouchScaleStarted()));
    //connect(m_pView, SIGNAL(touchScaleChanged(real)), this, SLOT(onTouchScaleChanged(real)));
    //connect(m_pView, SIGNAL(touchScaleEnded()), this, SLOT(onTouchScaleEnded()));
    connect(m_pView, SIGNAL(keyPressed()), this, SLOT(onKeyPressed()));
    connect(m_pView->getFPSManager(), SIGNAL(FPSChanged(int)), this, SLOT(onFPSChanged(int)));

    pView->setGeometry(QRect(100, 100, 800, 600));
}

//-----------------------------------------------------------------------------------------
Controller::~Controller()
{
}

//-----------------------------------------------------------------------------------------
void Controller::onMousePressed()
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
void Controller::onMouseReleased()
{
    m_bPressed = false;
    m_bTouch = false;
}

//-----------------------------------------------------------------------------------------
void Controller::onMouseMoved()
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
                wheel(-delta.y());
            }
            else
            {
                rotate(delta.x(), delta.y());
            }
        }
    }
}

//-----------------------------------------------------------------------------------------
void Controller::wheel(int delta)
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
void Controller::rotate(int deltax, int deltay)
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
void Controller::onTouchScaleStarted()
{
    m_bTouch = true;
}

//-----------------------------------------------------------------------------------------
void Controller::onTouchScaleChanged(real dScaleFactor)
{
    m_bTouch = true;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
    {
        QVector3D nPos = pCamera->getEyePosition() / dScaleFactor;
        pCamera->setEyePosition(nPos);
    }
}

//-----------------------------------------------------------------------------------------
void Controller::onTouchScaleEnded()
{
    m_bTouch = false;
}

//-----------------------------------------------------------------------------------------
void Controller::onKeyPressed()
{
    CKeyboardStates keyboardStates = m_pView->getKeyboardStates();
    float dSpeed = 1.0;

    if (CCamera* pCamera = m_pView->getCurrentCamera())
    {
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

//-----------------------------------------------------------------------------------------
void Controller::onFPSChanged(int iFPS)
{
    qDebug() << "FPS: " << iFPS;
    qDebug() << "Draw call count: " << m_pView->getGLRenderer()->getDrawCallCount();
}
