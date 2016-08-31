#include "CCamera.h"
#include "CLogManager.h"
#include "CGeometryGlobal.h"
#include "CSceneNode.h"

static const QVector3D DefaultCameraUp(0., 1., 0.);
static const QVector3D DefaultCameraCenter(1., 0., 0.);
static const QVector3D DefaultCameraEyePosition(0., 0., 0.);
static const real DefaultCameraHorizontalFOV(0.25f * Math::Pi);
static const real DefaultCameraClipPlaneNear(0.1f);
static const real DefaultCameraClipPlaneFar(500.f);
static const real DefaultCameraAspect(4.f/3.f);

//-----------------------------------------------------------------------------------------
CCamera::CCamera(CSceneManager* pSceneManager, const QString& name)
    : ASceneNodeItem(pSceneManager, name)
    , m_eProjectionType(eProjectionPerspective)
    , m_vEyePosition(DefaultCameraEyePosition)
    , m_vUp(DefaultCameraUp)
    , m_vCenter(DefaultCameraCenter)
    , m_dHorizontalFOV(DefaultCameraHorizontalFOV)
    , m_dClipPlaneNear(DefaultCameraClipPlaneNear)
    , m_dClipPlaneFar(DefaultCameraClipPlaneFar)
    , m_dAspect(DefaultCameraAspect)
    , m_dLeft(-1.)
    , m_dRight(1.)
    , m_dBottom(-1.)
    , m_dTop(1.)
    , m_ViewingCube(QVector3D(-1, -1, -1), QVector3D(1, 1, 1))
    , m_bNeedUpdateFrustum(true)
    , m_bNeedUpdateViewMatrix(true)
    , m_bNeedUpdateProjectionMatrix(true)
    , m_bNeedUpdateViewProjectionMatrix(true)
{
    m_vForward = m_vCenter - m_vEyePosition;
}

//-----------------------------------------------------------------------------------------
CCamera::~CCamera()
{
}

//-------------------------------------------------------------------------------------------------
// Listeners
//-----------------------------------------------------------------------------------------
void CCamera::registerListener(ICameraListener* pListener)
{
    m_CameraListeners.insert(pListener);
}

//-----------------------------------------------------------------------------------------
void CCamera::unregisterListener(ICameraListener* pListener)
{
    m_CameraListeners.remove(pListener);
}

//-----------------------------------------------------------------------------------------
void CCamera::notifyUpdate()
{
    if (m_bNotificationsEnabled)
    {
        foreach (ICameraListener* pListener, m_CameraListeners)
        {
            pListener->onUpdate(this);
        }

        ASceneNodeItem::notifyUpdate();
    }
}

//-------------------------------------------------------------------------------------------------
// Setters
//-------------------------------------------------------------------------------------------------
void CCamera::setProjectionType(EnumProjectionType eProjectionType)
{ 
    m_eProjectionType = eProjectionType;
    m_bNeedUpdateProjectionMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setEyePosition(const QVector3D& vEyePosition)
{
    m_vEyePosition = vEyePosition;
    m_vForward = m_vCenter - m_vEyePosition;
    m_bNeedUpdateViewMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setUp(const QVector3D& vUp)
{
    m_vUp = vUp;
    m_bNeedUpdateViewMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setCenter(const QVector3D& vCenter)
{
    m_vCenter = vCenter;
    m_vForward = m_vCenter - m_vEyePosition;
    m_bNeedUpdateViewMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setLookAt(const QVector3D& vEyePosition, const QVector3D& vCenter, const QVector3D& vUp)
{
    setEyePosition(vEyePosition);
    setCenter(vCenter);
    setUp(vUp);
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setHorizontalFOV(real dHorizontalFOV)
{
    m_dHorizontalFOV = dHorizontalFOV;
    m_bNeedUpdateProjectionMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setNear(real dClipPlaneNear)
{
    m_dClipPlaneNear = dClipPlaneNear;
    m_bNeedUpdateProjectionMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setFar(real dClipPlaneFar)
{
    m_dClipPlaneFar = dClipPlaneFar;
    m_bNeedUpdateProjectionMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setAspectRatio(real dAspect)
{
    m_dAspect = dAspect;
    m_bNeedUpdateProjectionMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setPerspective(real dHorizontalFOV, real dAspect, real dClipPlaneNear, real dClipPlaneFar)
{
    m_eProjectionType = eProjectionPerspective;

    m_dHorizontalFOV = dHorizontalFOV;
    m_dAspect = dAspect;
    m_dClipPlaneNear = dClipPlaneNear;
    m_dClipPlaneFar = dClipPlaneFar;
    m_bNeedUpdateProjectionMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setInfinitePerspective(real dHorizontalFOV, real dAspect, real dClipPlaneNear)
{
    m_eProjectionType = eProjectionInfinitePerspective;

    m_dHorizontalFOV = dHorizontalFOV;
    m_dAspect = dAspect;
    m_dClipPlaneNear = dClipPlaneNear;
    m_dClipPlaneFar = std::numeric_limits<real>::max();
    m_bNeedUpdateProjectionMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::setOrtho(real dLeft, real dRight, real dBottom, real dTop, real dClipPlaneNear, real dClipPlaneFar)
{
    m_eProjectionType = eProjectionOrtho;

    m_dLeft = dLeft;
    m_dRight = dRight;
    m_dBottom = dBottom;
    m_dTop = dTop;

    m_dClipPlaneNear = dClipPlaneNear;
    m_dClipPlaneFar = dClipPlaneFar;
    m_bNeedUpdateProjectionMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-------------------------------------------------------------------------------------------------
void CCamera::translateEyePosition(const QVector3D& vTranslation)
{
    m_vEyePosition += translation(vTranslation);
    m_vForward = m_vCenter - m_vEyePosition;
    m_bNeedUpdateViewMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::translateCenter(const QVector3D& vTranslation)
{
    m_vCenter += translation(vTranslation);
    m_vForward = m_vCenter - m_vEyePosition;
    m_bNeedUpdateViewMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
QQuaternion CCamera::pan(real dAngle)
{
    return QQuaternion::fromAxisAndAngle(m_vUp, dAngle);
}

//-----------------------------------------------------------------------------------------
QQuaternion CCamera::tilt(real dAngle)
{
    QVector3D side = QVector3D::crossProduct(m_vForward, m_vUp);
    return QQuaternion::fromAxisAndAngle(side, dAngle);
}

//-----------------------------------------------------------------------------------------
QQuaternion CCamera::roll(real dAngle)
{
    return QQuaternion::fromAxisAndAngle(m_vForward, dAngle);
}

//-----------------------------------------------------------------------------------------
void CCamera::rotateEye(const QQuaternion& q)
{
    m_vUp = q.rotatedVector(m_vUp);
    m_vForward = q.rotatedVector(m_vForward);
    m_vCenter = m_vEyePosition + m_vForward;
    m_bNeedUpdateViewMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-----------------------------------------------------------------------------------------
void CCamera::rotateCenter(const QQuaternion& q)
{
    m_vUp = q.rotatedVector(m_vUp);
    m_vForward = q.rotatedVector(m_vForward);
    m_vEyePosition = m_vCenter - m_vForward;
    m_bNeedUpdateViewMatrix = true;
    m_bNeedUpdateFrustum = true;
    notifyUpdate();
}

//-------------------------------------------------------------------------------------------------
QVector3D CCamera::translation(const QVector3D& vTranslation) const
{
    QVector3D vector(0., 0., 0.);
    if (vTranslation.x() != 0.)
        vector += QVector3D::normal(m_vForward, m_vUp) * vTranslation.x();
    if (vTranslation.y() != 0.)
        vector += m_vUp.normalized() * vTranslation.y();
    if (vTranslation.z() != 0.)
        vector += m_vForward.normalized() * vTranslation.z();
    return vector;
}

//-----------------------------------------------------------------------------------------
void CCamera::tiltPanRollCenter(real dTilt, real dPan, real dRoll, EnumRotateOrder eOrder /*= eTiltPanRoll*/)
{
    switch (eOrder)
    {
    case eTiltPanRoll:
        rotateCenter(roll(dRoll) * pan(dPan) * tilt(dTilt));
        break;
    case eTiltRollPan:
        rotateCenter(pan(dPan) * roll(dRoll) * tilt(dTilt));
        break;
    case ePanTiltRoll:
        rotateCenter(roll(dRoll) * tilt(dTilt) * pan(dPan));
        break;
    case ePanRollTilt:
        rotateCenter(tilt(dTilt) * roll(dRoll) * pan(dPan));
        break;
    case eRollTiltPan:
        rotateCenter(pan(dPan) * tilt(dTilt) * roll(dRoll));
        break;
    case eRollPanTilt:
        rotateCenter(tilt(dTilt) * pan(dPan) * roll(dRoll));
        break;
    }
}


//-----------------------------------------------------------------------------------------
void CCamera::tiltPanRollEye(real dTilt, real dPan, real dRoll, EnumRotateOrder eOrder /*= eTiltPanRoll*/)
{
    switch (eOrder)
    {
    case eTiltPanRoll:
        rotateEye(roll(dRoll) * pan(dPan) * tilt(dTilt));
        break;
    case eTiltRollPan:
        rotateEye(pan(dPan) * roll(dRoll) * tilt(dTilt));
        break;
    case ePanTiltRoll:
        rotateEye(roll(dRoll) * tilt(dTilt) * pan(dPan));
        break;
    case ePanRollTilt:
        rotateEye(tilt(dTilt) * roll(dRoll) * pan(dPan));
        break;
    case eRollTiltPan:
        rotateEye(pan(dPan) * tilt(dTilt) * roll(dRoll));
        break;
    case eRollPanTilt:
        rotateEye(tilt(dTilt) * pan(dPan) * roll(dRoll));
        break;
    }
}

//-------------------------------------------------------------------------------------------------
// Getters
//-------------------------------------------------------------------------------------------------
QVector3D CCamera::getEyePosition() const
{
    return m_vEyePosition;
}

//-----------------------------------------------------------------------------------------
QVector3D CCamera::getCenter() const
{
    return m_vCenter;
}

//-----------------------------------------------------------------------------------------
QVector3D CCamera::getForward() const
{
    return m_vForward;
}

//-----------------------------------------------------------------------------------------
QVector3D CCamera::getUp() const
{
    return m_vUp;
}

//-----------------------------------------------------------------------------------------
QVector3D CCamera::getRight() const
{
    return QVector3D::crossProduct(m_vUp, m_vForward);
}

//-----------------------------------------------------------------------------------------
QVector3D CCamera::getLeft() const
{
    return - getRight();
}

//-----------------------------------------------------------------------------------------
real CCamera::getHorizontalFOV() const
{
    return m_dHorizontalFOV;
}

//-----------------------------------------------------------------------------------------
real CCamera::getVerticalFOV() const
{
    return m_dHorizontalFOV * m_dAspect;
}

//-----------------------------------------------------------------------------------------
real CCamera::getNear() const
{
    return m_dClipPlaneNear;
}

//-----------------------------------------------------------------------------------------
real CCamera::getFar() const
{
    return m_dClipPlaneFar;
}

//-----------------------------------------------------------------------------------------
const CFrustum& CCamera::getFrustum() const
{
    if (m_bNeedUpdateFrustum)
    {
        m_Frustum.update(getViewProjectionMatrix());
        m_bNeedUpdateFrustum = false;
    }

    return m_Frustum;
}

//-----------------------------------------------------------------------------------------
real CCamera::getAspectRatio() const
{
    return m_dAspect;
}

//-----------------------------------------------------------------------------------------
const QMatrix4x4& CCamera::getViewProjectionMatrix() const
{
    if (m_bNeedUpdateViewMatrix || m_bNeedUpdateProjectionMatrix || m_bNeedUpdateViewProjectionMatrix)
    {
        m_ViewProjectionMatrix = getProjectionMatrix() * getViewMatrix();
        m_bNeedUpdateViewProjectionMatrix = false;
    }

    return m_ViewProjectionMatrix;
}

//-----------------------------------------------------------------------------------------
QMatrix4x4 CCamera::getTransformation() const
{
    if (CSceneNode* pNode = getNode())
    {
        return getViewProjectionMatrix() * pNode->getGlobalTransformation();
    }

    return getViewProjectionMatrix();
}

//-----------------------------------------------------------------------------------------
const QMatrix4x4& CCamera::getViewMatrix() const
{
    if (m_bNeedUpdateViewMatrix)
    {
        m_ViewMatrix.setToIdentity();
        m_ViewMatrix.lookAt(m_vEyePosition, m_vCenter, m_vUp);

        m_bNeedUpdateViewProjectionMatrix = true;
        m_bNeedUpdateViewMatrix = false;
    }

    return m_ViewMatrix;
}

//-----------------------------------------------------------------------------------------
const QMatrix4x4& CCamera::getProjectionMatrix() const
{
    if (m_bNeedUpdateProjectionMatrix)
    {
        m_ProjectionMatrix.setToIdentity();

        switch (m_eProjectionType)
        {
        case eProjectionOrtho:
            m_ProjectionMatrix.ortho(m_dLeft, m_dRight, m_dBottom, m_dTop, m_dClipPlaneNear, m_dClipPlaneFar);
            break;
        case eProjectionPerspective:
            m_ProjectionMatrix.perspective(Math::radToDeg(m_dHorizontalFOV), m_dAspect, m_dClipPlaneNear, m_dClipPlaneFar);
            break;
        case eProjectionInfinitePerspective:
            m_ProjectionMatrix = Math::infinitePerspective(m_dHorizontalFOV, m_dAspect, m_dClipPlaneNear);
            break;
        }

        m_bNeedUpdateViewProjectionMatrix = true;
        m_bNeedUpdateProjectionMatrix = false;
    }

    return m_ProjectionMatrix;
}

//-----------------------------------------------------------------------------------------
bool CCamera::isVisible(const QVector3D& point) const
{
    QVector3D projected = getViewProjectionMatrix() * point;
    return m_ViewingCube.contains(projected);
}

// -----------------------------------------------------------------------------------------
bool CCamera::isVisible(const CBox3D& box) const
{
    CBox3D projected = box.transformed(getViewMatrix());

    if (projected.getMinimum().z() >= 0.0f
            || projected.getMaximum().z() >= 0.0f) {
        // The box crosses the eye line in the view. Don't do the
        // projection or the math will go all strange with a
        // perspective projection. Just assume that it is cullable
        // if it passes the eye line, and hence is definitely outside
        // the viewing volume. Note that it is possible that the box is
        // half in front of the eye and half behind, which we handle now
        // by truncating the box at the eye plane.
        // If the projection is orthographic, we don't need to do this.
        // Orthographic projections have the last row set to (0, 0, 0, 1).

        QMatrix4x4 proj = getProjectionMatrix();

        if (proj(3, 0) != 0.0f || proj(3, 1) != 0.0f
                || proj(3, 2) != 0.0f || proj(3, 3) != 1.0f) {
            if (projected.getMinimum().z() >= 0.0f)
                return true;

            projected.setExtents(projected.getMinimum(),
                                 QVector3D(
                                     projected.getMaximum().x(),
                                     projected.getMaximum().y(),
                                     0.0f));
        }
    }
    projected.transform(getProjectionMatrix());
    return m_ViewingCube.intersects(projected);
}

//-----------------------------------------------------------------------------------------
QVector3D CCamera::mapPointToEyeCoordinates(const QPoint& point, const QSize& viewportSize) const
{
    int x = point.x();
    int y = point.y();
    int width = viewportSize.width();
    int height = viewportSize.height();

    real xrel = ((real(x * 2)) - real(width)) / real(width);
    real yrel = -((real(y * 2)) - real(height)) / real(height);

    QMatrix4x4 m = getProjectionMatrix();
    QMatrix4x4 invm = m.inverted();

    return invm.map(QVector3D(xrel, yrel, -1.0f));
}

//-----------------------------------------------------------------------------------------
QVector3D CCamera::mapPointToWorldCoordinates(const QPoint& point, const QSize& viewportSize) const
{
    QVector3D pick = mapPointToEyeCoordinates(point, viewportSize);

    QMatrix4x4 m = getViewMatrix();
    QMatrix4x4 invm = m.inverted();

    return invm.map(pick);
}

