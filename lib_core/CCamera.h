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

    //! Lib�re un listener
    void unregisterListener(ICameraListener* pListener);

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le type de projection
    void setProjectionType(EnumProjectionType eProjectionType);

    //! D�finit la position de la cam�ra
    void setEyePosition(const QVector3D& vEyePosition);

    //! D�finit l'axe haut de la cam�ra
    void setUp(const QVector3D& vUp);

    //! D�finit le point regard� par la cam�ra
    void setCenter(const QVector3D& vCenter);

    //! D�finit la position et orientation de la cam�ra
    void setLookAt(const QVector3D& vEyePosition, const QVector3D& vCenter, const QVector3D& vUp);

    //! D�finit le champ de vision horizontal de la cam�ra
    void setHorizontalFOV(real dHorizontalFOV);

    //! D�finit le plan minimum de visibilit�
    void setNear(real dClipPlaneNear);

    //! D�finit le plan maximum de visibilit�
    void setFar(real dClipPlaneFar);

    //! D�finit le ratio largeur / hauteur
    void setAspectRatio(real dAspect);

    //! D�finit une projection perspective
    void setPerspective(real dHorizontalFOV, real dAspect, real dClipPlaneNear, real dClipPlaneFar);

    //! D�finit une projection perspective avec un z infini
    void setInfinitePerspective(real dHorizontalFOV, real dAspect, real dClipPlaneNear);

    //! D�finit une projection orthographique
    void setOrtho(real dLeft, real dRight, real dBottom, real dTop, real dClipPlaneNear, real dClipPlaneFar);

    //-------------------------------------------------------------------------------------------------
    // Fonction de transformation
    //-------------------------------------------------------------------------------------------------

    //! Effectue une translation de la position de l'oeil de la cam�ra
    void translateEyePosition(const QVector3D& vTranslation);

    //! Effectue une translation de la position du centre de la cam�ra
    void translateCenter(const QVector3D& vTranslation);

    /*!
    Retourne le Quaternion correspondant � la rotation de la vue autour de l'axe X.
    Le quaternion peut �tre appliqu� � la position de l'oeil via la m�thode rotateEye()
    */
    QQuaternion pan(real dAngle);

    /*!
    Retourne le Quaternion correspondant � la rotation de la vue autour de l'axe Y.
    Le quaternion peut �tre appliqu� � la position de l'oeil via la m�thode rotateEye()
    */
    QQuaternion tilt(real dAngle);

    /*!
    Retourne le Quaternion correspondant � la rotation de la vue autour de l'axe Z.
    Le quaternion peut �tre appliqu� � la position de l'oeil via la m�thode rotateEye()
    */
    QQuaternion roll(real dAngle);

    //! Effectue une rotation de l'oeil de la cam�ra
    void rotateEye(const QQuaternion& q);

    //! Effectue une rotation du centre de la cam�ra
    void rotateCenter(const QQuaternion& q);

    //! Effectue une translation de la cam�ra
    QVector3D translation(const QVector3D& vTranslation) const;

    //! Effectue une rotation du centre de la cam�ra
    void tiltPanRollCenter(real dTilt, real dPan, real dRoll, EnumRotateOrder eOrder = eTiltPanRoll);

    //! Effectue une rotation du l'oeil de la cam�ra
    void tiltPanRollEye(real dTilt, real dPan, real dRoll, EnumRotateOrder eOrder = eTiltPanRoll);

    /*!
        Transforme un point dans la vue 2D en point dans la rep�re de l'oeil de la cam�ra
        x et y sont les coordonn�es sur le plan ClipNear, et z = -m_dClipNearPlane
    */
    QVector3D mapPointToEyeCoordinates(const QPoint& point, const QSize& viewportSize) const;

    /*!
        Transforme un point dans la vue 2D en point dans le rep�re global
        x et y sont les coordonn�es sur le plan ClipNear, et z = -m_dClipNearPlane
    */
    QVector3D mapPointToWorldCoordinates(const QPoint& point, const QSize& viewportSize) const;

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "Camera"; }

    //! Retourne le type de projection
    EnumProjectionType getProjectionType() const { return m_eProjectionType; }

    //! Retourne la position de la cam�ra
    QVector3D getEyePosition() const;

    //! Retourne le point observ� par la cam�ra
    QVector3D getCenter() const;

    //! Retourne la direction de la cam�ra
    QVector3D getForward() const;

    //! Retourne l'axe sup�rieur de la cam�ra
    QVector3D getUp() const;

    //! Retourne l'axe droite de la cam�ra
    QVector3D getRight() const;

    //! Retourne l'axe gauche de la cam�ra
    QVector3D getLeft() const;

    //! Retourne le plan minimum de la cam�ra
    real getNear() const;

    //! Retourne le plan maximum de la cam�ra
    real getFar() const;

    //! Retourne la pyramide de vision de la cam�ra
    const CFrustum& getFrustum() const;

    //! Retourne le champ horizontal
    real getHorizontalFOV() const;

    //! Retourne le champ vertical
    real getVerticalFOV() const;

    //! Retourne le rapport largeur / hauteur de la cam�ra
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

    //! Cr�e une string contenant les propri�t�s de l'objet
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

    //! Notifie les listeners qu'une les propri�t�s de la cam�ra ont chang�es
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

    //! Point observ� par la cam�ra
    QVector3D m_vCenter;

    // Param�tres cam�ra perspective
    real m_dHorizontalFOV;

    //! Le plan minimum de la cam�ra
    real m_dClipPlaneNear;

    //! Le plan maximum de la cam�ra
    real m_dClipPlaneFar;

    //! Rapport largeur / hauteur de la cam�ra
    real m_dAspect;

    //! Param�tre gauche cam�ra orthographique
    real m_dLeft;

    //! Param�tre droite cam�ra orthographique
    real m_dRight;

    //! Param�tre bas cam�ra orthographique
    real m_dBottom;

    //! Param�tre haut cam�ra orthographique
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
