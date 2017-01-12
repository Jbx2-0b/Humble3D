#include "CQmlWidget3D.h"
#include "Humble3DApp.h"
#include "CGLRenderer.h"
#include "CCamera.h"

// Qt
#include <QOpenGLFramebufferObject>

class CQmlGLRenderer : public QQuickFramebufferObject::Renderer
{
public:
    CQmlGLRenderer()
        : m_GLRenderer(Humble3DApp::instance()->getSceneManager())
        , m_Camera(Humble3DApp::instance()->getSceneManager(), "QmlCamera")
    {
        m_GLRenderer.init();
        m_GLRenderer.setCurrentCamera(&m_Camera);
        m_GLRenderer.run();
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
    : m_EyePosition(DefaultCameraEyePosition)
    , m_Center(DefaultCameraCenter)
{

}

QQuickFramebufferObject::Renderer *CQmlWidget3D::createRenderer() const
{
    return new CQmlGLRenderer();
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
    }
}

