#ifndef CCAMERA_H
#define CCAMERA_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMath.h"
#include "CGeometryGlobal.h"
#include "ASceneNodeItem.h"
#include "CRay.h"
#include "CPlane.h"
#include "CFrustum.h"
#include "CStringHelper.h"

// Qt
#include <QPointF>
#include <QVector3D>
#include <QMatrix4x4>


class CSceneManager;

//-----------------------------------------------------------------------------------------
// \class CCamera
// \brief Classe permettant de manipuler des cameras
//-----------------------------------------------------------------------------------------
class LIB_CORE_SHARED_EXPORT CCamera : public ASceneNodeItem
{

public:

    //! Constructeur
    CCamera(CSceneManager* pSceneManager, const QString& name);

    //! Destructeur
    virtual ~CCamera();

    //-------------------------------------------------------------------------------------------------
    // Listeners
    //-------------------------------------------------------------------------------------------------

    class ICameraListener
    {
        friend class CCamera;

    protected:

        virtual void onUpdate(CCamera* pCamera) = 0;
    };

    //! Enregistre un listener
    void registerListener(ICameraListener* pListener);

    //! Libére un listener
    void unregisterListener(ICameraListener* pListener);

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le type de projection
    void setProjectionType(EnumProjectionType eProjectionType);

    //! Définit la position de la caméra
    void setEyePosition(const QVector3D& vEyePosition);

    //! Définit l'axe haut de la caméra
    void setUp(const QVector3D& vUp);

    //! Définit le point regardé par la caméra
    void setCenter(const QVector3D& vCenter);

    //! Définit la position et orientation de la caméra
    void setLookAt(const QVector3D& vEyePosition, const QVector3D& vCenter, const QVector3D& vUp);

    //! Définit le champ de vision horizontal de la caméra
    void setHorizontalFOV(real dHorizontalFOV);

    //! Définit le plan minimum de visibilité
    void setNear(real dClipPlaneNear);

    //! Définit le plan maximum de visibilité
    void setFar(real dClipPlaneFar);

    //! Définit le ratio largeur / hauteur
    void setAspectRatio(real dAspect);

    //! Définit une projection perspective
    void setPerspective(real dHorizontalFOV, real dAspect, real dClipPlaneNear, real dClipPlaneFar);

    //! Définit une projection perspective avec un z infini
    void setInfinitePerspective(real dHorizontalFOV, real dAspect, real dClipPlaneNear);

    //! Définit une projection orthographique
    void setOrtho(real dLeft, real dRight, real dBottom, real dTop, real dClipPlaneNear, real dClipPlaneFar);

    //-------------------------------------------------------------------------------------------------
    // Fonction de transformation
    //-------------------------------------------------------------------------------------------------

    //! Effectue une translation de la position de l'oeil de la caméra
    void translateEyePosition(const QVector3D& vTranslation);

    //! Effectue une translation de la position du centre de la caméra
    void translateCenter(const QVector3D& vTranslation);

    /*!
    Retourne le Quaternion correspondant à la rotation de la vue autour de l'axe X.
    Le quaternion peut être appliqué à la position de l'oeil via la méthode rotateEye()
    */
    QQuaternion pan(real dAngle);

    /*!
    Retourne le Quaternion correspondant à la rotation de la vue autour de l'axe Y.
    Le quaternion peut être appliqué à la position de l'oeil via la méthode rotateEye()
    */
    QQuaternion tilt(real dAngle);

    /*!
    Retourne le Quaternion correspondant à la rotation de la vue autour de l'axe Z.
    Le quaternion peut être appliqué à la position de l'oeil via la méthode rotateEye()
    */
    QQuaternion roll(real dAngle);

    //! Effectue une rotation de l'oeil de la caméra
    void rotateEye(const QQuaternion& q);

    //! Effectue une rotation du centre de la caméra
    void rotateCenter(const QQuaternion& q);

    //! Effectue une translation de la caméra
    QVector3D translation(const QVector3D& vTranslation) const;

    //! Effectue une rotation du centre de la caméra
    void tiltPanRollCenter(real dTilt, real dPan, real dRoll, EnumRotateOrder eOrder = eTiltPanRoll);

    //! Effectue une rotation du l'oeil de la caméra
    void tiltPanRollEye(real dTilt, real dPan, real dRoll, EnumRotateOrder eOrder = eTiltPanRoll);

    /*!
        Transforme un point dans la vue 2D en point dans la repére de l'oeil de la caméra
        x et y sont les coordonnées sur le plan ClipNear, et z = -m_dClipNearPlane
    */
    QVector3D mapPointToEyeCoordinates(const QPoint& point, const QSize& viewportSize) const;

    /*!
        Transforme un point dans la vue 2D en point dans le repére global
        x et y sont les coordonnées sur le plan ClipNear, et z = -m_dClipNearPlane
    */
    QVector3D mapPointToWorldCoordinates(const QPoint& point, const QSize& viewportSize) const;

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "Camera"; }

    //! Retourne le type de projection
    EnumProjectionType getProjectionType() const { return m_eProjectionType; }

    //! Retourne la position de la caméra
    QVector3D getEyePosition() const;

    //! Retourne le point observé par la caméra
    QVector3D getCenter() const;

    //! Retourne la direction de la caméra
    QVector3D getForward() const;

    //! Retourne l'axe supérieur de la caméra
    QVector3D getUp() const;

    //! Retourne l'axe droite de la caméra
    QVector3D getRight() const;

    //! Retourne l'axe gauche de la caméra
    QVector3D getLeft() const;

    //! Retourne le plan minimum de la caméra
    real getNear() const;

    //! Retourne le plan maximum de la caméra
    real getFar() const;

    //! Retourne la pyramide de vision de la caméra
    const CFrustum& getFrustum() const;

    //! Retourne le champ horizontal
    real getHorizontalFOV() const;

    //! Retourne le champ vertical
    real getVerticalFOV() const;

    //! Retourne le rapport largeur / hauteur de la caméra
    real getAspectRatio() const;

    //! Construit et retourne la matrice concatenant la matrice de vue et de projection
    const QMatrix4x4& getViewProjectionMatrix() const;

    //! Retourne la transformation globale
    QMatrix4x4 getTransformation() const;

    //! Construit et retourne la matrice de vue
    const QMatrix4x4& getViewMatrix() const;

    //! Retourne vrai si le point est visible
    bool isVisible(const QVector3D& point) const;

    //! Retourne vrai si la boite est visible
    bool isVisible(const CBox3D& box) const;

    //! Construit et retourne la matrice de projection
    const QMatrix4x4& getProjectionMatrix() const;

    //! Retourne la boite englobante
    virtual const CBox3D& getBoundingBox(void) const { /*TODO*/ return CBox3D::NullBox; }

    //! Retourne le nombre de polygones
    virtual unsigned int getPolygonCount() const { return 0; }

    //! Crée une string contenant les propriétés de l'objet
    virtual QString toString() const
    {
        QString result;
        result += "[Camera \n";
        result += ASceneNodeItem::toString();
        result += QString("EyePosition: %1\n").arg(StringHelper::toQString(m_vEyePosition));
        result += QString("Up: %1\n").arg(StringHelper::toQString(m_vUp));
        result += QString("View: %1\n").arg(StringHelper::toQString(m_vForward));
        result += QString("Center: %1\n").arg(StringHelper::toQString(m_vCenter));
        result += QString("HFOV: %1\n").arg(m_dHorizontalFOV);
        result += QString("ClipPlaneNear: %1\n").arg(m_dClipPlaneNear);
        result += QString("ClipPlaneFar: %1\n").arg(m_dClipPlaneFar);
        result += QString("Aspect: %1\n").arg(m_dAspect);
        result += QString("Left: %1\n").arg(m_dLeft);
        result += QString("Right: %1\n").arg(m_dRight);
        result += QString("Bottom: %1\n").arg(m_dBottom);
        result += QString("Top: %1\n").arg(m_dTop);
        result += QString("ViewMatrix: %1\n").arg(StringHelper::toQString(getViewMatrix()));
        result += QString("ProjectionMatrix: %1\n").arg(StringHelper::toQString(getProjectionMatrix()));
        result += QString("ViewProjectionMatrix: %1\n").arg(StringHelper::toQString(getViewProjectionMatrix()));
        result += "]\n";

        return result;
    }


protected:

    //! Notifie les listeners qu'une les propriétés de la caméra ont changées
    virtual void notifyUpdate();

    //! Liste des listeners
    QSet<ICameraListener*> m_CameraListeners;

    //! Type de projection
    EnumProjectionType m_eProjectionType;

    //! Position de l'oeil
    QVector3D m_vEyePosition;

    //! Vecteur haut
    QVector3D m_vUp;

    //! Direction de la vue
    QVector3D m_vForward;

    //! Point observé par la caméra
    QVector3D m_vCenter;

    // Paramétres caméra perspective
    real m_dHorizontalFOV;

    //! Le plan minimum de la caméra
    real m_dClipPlaneNear;

    //! Le plan maximum de la caméra
    real m_dClipPlaneFar;

    //! Rapport largeur / hauteur de la caméra
    real m_dAspect;

    //! Paramétre gauche caméra orthographique
    real m_dLeft;

    //! Paramétre droite caméra orthographique
    real m_dRight;

    //! Paramétre bas caméra orthographique
    real m_dBottom;

    //! Paramétre haut caméra orthographique
    real m_dTop;

    CBox3D m_ViewingCube;

    mutable bool m_bNeedUpdateFrustum;

    mutable CFrustum m_Frustum;

    mutable QMatrix4x4 m_ViewMatrix;

    mutable bool m_bNeedUpdateViewMatrix;

    mutable QMatrix4x4 m_ProjectionMatrix;

    mutable bool m_bNeedUpdateProjectionMatrix;

    mutable QMatrix4x4 m_ViewProjectionMatrix;

    mutable bool m_bNeedUpdateViewProjectionMatrix;

};

#endif // CCAMERA_H
