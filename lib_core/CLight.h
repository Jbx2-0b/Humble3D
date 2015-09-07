#ifndef CLIGHT_H
#define CLIGHT_H

// Lib
#include "lib_core_global.h"

// Foundations
#include "CMath.h"
#include "CGeometryGlobal.h"
#include "ASceneNodeItem.h"
#include "CMeshBuffer.h"

// Qt
#include <QVector4D>
#include <QVector3D>
#include <QMatrix4x4>



//--------------------------------------------------------------------------
// \class CLight
// \brief Classe permettant de manipuler un modele de lumiere
//--------------------------------------------------------------------------
class LIB_CORE_SHARED_EXPORT CLight : public ASceneNodeItem
{

public:

    //! Constructeur
    CLight(CSceneManager* pSceneManager, const QString& name);

    //! Destructeur
    virtual ~CLight();

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Définit si la lumiere est activee
    void setEnabled(bool bEnabled);

    //! Définit le type de source lumineuse
    void setSourceType(EnumLightSourceType eSourceType);

    //! Définit la direction de la lumiere (par rapport a son noeud)
    void setDirection(const QVector3D& vDirection);

    //! Définit l'attenuation constante
    void setConstantAttenuation(real dAttenuation);

    //! Définit l'attenuation lineaire
    void setLinearAttenuation(real dAttenuation);

    //! Définit l'attenuation quadratique
    void setQuadraticAttenuation(real dAttenuation);

    //! Définit la couleur ambiante
    void setAmbientColor(const QVector4D& color);

    //! Définit la couleur ambiante
    void setAmbientColor(real r, real g, real b, real a = 1.0);

    //! Définit la couleur diffuse
    void setDiffuseColor(const QVector4D& color);

    //! Définit la couleur diffuse
    void setDiffuseColor(real r, real g, real b, real a = 1.0);

    //! Définit la couleur speculaire
    void setSpecularColor(const QVector4D& color);

    //! Définit la couleur speculaire
    void setSpecularColor(real r, real g, real b, real a = 1.0);

    //! Définit le cone interieur
    void setInnerConeAngle(real dAngle);

    //! Définit le cone exterieur
    void setOuterConeAngle(real dAngle);
    
    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Retourne vrai si peut être rendu
    virtual bool isRenderable() const { return false; }

    //! Retourne le nom du type
    virtual QString getTypeName() const { return "Light"; }

    //! Retourne true si la lumiere est activee
    bool isEnabled() const;

    //! Retourne le type de source lumineuse
    EnumLightSourceType getSourceType() const;

    //! Retourne la direction de la source lumineuse (par rapport a son noeud)
    QVector3D getDirection() const;

    //! Retourne l'attenuation constante
    real getConstantAttenuation() const;

    //! Retourne l'attenuation lineaire
    real getLinearAttenuation() const;

    //! Retourne l'attenuation quadratique
    real getQuadraticAttenuation() const;

    //! Retourne la couleur diffuse
    QVector4D getDiffuseColor() const;

    //! Retourne la couleur speculaire
    QVector4D getSpecularColor() const;

    //! Retourne la couleur ambiante
    QVector4D getAmbientColor() const;

    //! Retourne le cone interieur
    real getInnerConeAngle() const;

    //! Retourne le cone exterieur
    real getOuterConeAngle() const;

    //! Retourne la matrice de vue de la lumiere
    QMatrix4x4 getViewMatrix() const;

    //! Retourne la matrice de projection de la lumiere
    QMatrix4x4 getProjectionMatrix() const;

    //! Retourne la boite englobante
    virtual const CBox3D& getBoundingBox(void) const { return CBox3D::NullBox; }

    //! Retourne le nombre de polygones
    virtual unsigned int getPolygonCount() const { return 0; }

    //! Crée une string contenant les propriétés de l'objet
    virtual QString toString() const
    {
        QString result;
        result += "[Light \n";
        result += ASceneNodeItem::toString();
        result += QString("Enabled: %1\n").arg(m_bIsEnabled);
        result += QString("Source Type: %1\n").arg(CGeometryGlobal::stringFromLightSourceType(m_eSourceType));
        result += QString("Direction: %1\n").arg(StringHelper::toQString(m_vDirection));
        result += QString("Constant Attenuation: %1\n").arg(m_dConstantAttenuation);
        result += QString("Linear Attenuation: %1\n").arg(m_dLinearAttenuation);
        result += QString("Quadric Attenuation: %1\n").arg(m_dQuadraticAttenuation);
        result += QString("Diffuse color: %1\n").arg(StringHelper::toQString(m_DiffuseColor));
        result += QString("Specular color: %1\n").arg(StringHelper::toQString(m_SpecularColor));
        result += QString("Ambient color: %1\n").arg(StringHelper::toQString(m_AmbientColor));
        result += QString("Inner cone angle: %1\n").arg(m_dInnerConeAngle);
        result += QString("Outer cone angle: %1\n").arg(m_dOuterConeAngle);

        return result;
    }

private:

    //! Vrai si la lumiere est allumee
    bool m_bIsEnabled;

    //! Type de source lumineuse
    EnumLightSourceType m_eSourceType;

    //! Direction de la source lumineuse (par rapport a son noeud)
    QVector3D m_vDirection;

    //! Attenuation constante
    real m_dConstantAttenuation;

    //! Attenuation lineaire
    real m_dLinearAttenuation;

    //! Attenuation quadratique
    real m_dQuadraticAttenuation;

    //! Couleur diffuse
    QVector4D m_DiffuseColor;

    //! Couleur speculaire
    QVector4D m_SpecularColor;

    //! Couleur ambiante
    QVector4D m_AmbientColor;

    //! Cone interieur
    real m_dInnerConeAngle;

    //! Cone exterieur
    real m_dOuterConeAngle;
};


#endif // CLIGHT_H
