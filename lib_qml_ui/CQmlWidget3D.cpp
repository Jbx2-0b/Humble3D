#include "CQmlWidget3D.h"
#include "Humble3DQmlApp.h"
#include "CGLRenderer.h"
#include "CCamera.h"

// Qt
#include <QOpenGLFramebufferObject>

class CQmlGLRenderer : public QQuickFramebufferObject::Renderer
{
public:
    CQmlGLRenderer()
        : m_GLRenderer(Humble3DQmlApp::instance()->getSceneManager())
        , m_Camera(Humble3DQmlApp::instance()->getSceneManager(), "QmlCamera")
    {
        m_GLRenderer.init();
        m_GLRenderer.setCurrentCamera(&m_Camera);
    }

    virtual ~CQmlGLRenderer()
    {
    }

    void render()
    {
        int width = framebufferObject()->width();
        int height = framebufferObject()->height();
        m_Camera.setAspectRatio((real) width / (real) height);

        m_GLRenderer.setResolution(width, height);
        m_GLRenderer.render();

        update();
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size)
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    void synchronize(QQuickFramebufferObject *item)
    {
        CQmlWidget3D *qmlWidget3D = static_cast<CQmlWidget3D*>(item);
        m_GLRenderer.setEnabled(qmlWidget3D->enabled());
        m_GLRenderer.setBackgroundColor(qmlWidget3D->backgroundColor());

        m_Camera.setEyePosition(qmlWidget3D->eyePosition());
        m_Camera.setCenter(qmlWidget3D->center());
    }

private:
    CGLRenderer m_GLRenderer;
    CCamera m_Camera;
};

static const QVector3D DefaultCameraEyePosition(8., 8., 4.);
static const QVector3D DefaultCameraCenter(0., 0., 0.);

CQmlWidget3D::CQmlWidget3D()
    : m_Enabled(false)
    , m_BackgroundColor(0.0, 0.0, 0.0, 1.0)
    , m_EyePosition(DefaultCameraEyePosition)
    , m_Center(DefaultCameraCenter)
{

}

QQuickFramebufferObject::Renderer *CQmlWidget3D::createRenderer() const
{
    return new CQmlGLRenderer();
}

bool CQmlWidget3D::enabled() const
{
    return m_Enabled;
}

void CQmlWidget3D::setEnabled(bool enabled)
{
    if (m_Enabled != enabled)
    {
        m_Enabled = enabled;
        emit enabledChanged();
        update();
    }
}

QVector4D CQmlWidget3D::backgroundColor() const
{
    return m_BackgroundColor;
}

void CQmlWidget3D::setBackgroundColor(const QVector4D& color)
{
    if (m_BackgroundColor != color)
    {
        m_BackgroundColor = color;
        emit backgroundColorChanged();
        update();
    }
}

QVector3D CQmlWidget3D::eyePosition() const
{
    return m_EyePosition;
}

void CQmlWidget3D::setEyePosition(const QVector3D& position)
{
    if (m_EyePosition != position)
    {
        m_EyePosition = position;
        emit eyePositionChanged();
        update();
    }
}

QVector3D CQmlWidget3D::center() const
{
    return m_Center;
}

void CQmlWidget3D::setCenter(const QVector3D& center)
{
    if (m_Center != center)
    {
        m_Center = center;
        emit centerChanged();
        update();
    }
}

