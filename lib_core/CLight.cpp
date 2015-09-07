#include "CLight.h"
#include "CGeometryGlobal.h"
#include "ARenderer.h"

static const real	DefaultLightConstantAttenuation = 0.;
static const real	DefaultLightLinearAttenuation = 1.;
static const real	DefaultLightQuadraticAttenuation = 0.;
static const QVector4D	DefaultLightAmbientColor(0.5, 0.5, 0.5, 1.);
static const QVector4D	DefaultLightDiffuseColor(0.5, 0.5, 0.5, 1.);
static const QVector4D	DefaultLightSpecularColor(0., 0., 0., 1.);
static const real	DefaultLightInnerConeAngle = Math::TwoPi;
static const real	DefaultLightOuterConeAngle = Math::TwoPi;
static const real	DefaultLightSize = 0.2f;

//--------------------------------------------------------------------------
// CLIGHT
//--------------------------------------------------------------------------
CLight::CLight(CSceneManager* pSceneManager, const QString& name)
: ASceneNodeItem(pSceneManager, name)
, m_bIsEnabled(true)
, m_eSourceType(eSourcePoint)
, m_dConstantAttenuation(DefaultLightConstantAttenuation)
, m_dLinearAttenuation(DefaultLightLinearAttenuation)
, m_dQuadraticAttenuation(DefaultLightQuadraticAttenuation)
, m_DiffuseColor(DefaultLightDiffuseColor)
, m_SpecularColor(DefaultLightSpecularColor)
, m_AmbientColor(DefaultLightAmbientColor)
, m_dInnerConeAngle(DefaultLightInnerConeAngle)
, m_dOuterConeAngle(DefaultLightOuterConeAngle)
{	
}

//--------------------------------------------------------------------------
CLight::~CLight()
{
}

//--------------------------------------------------------------------------
// Setters
//--------------------------------------------------------------------------
void CLight::setEnabled(bool bEnabled)
{
	m_bIsEnabled = bEnabled;
}

//--------------------------------------------------------------------------
void CLight::setSourceType(EnumLightSourceType eSourceType)
{
	m_eSourceType = eSourceType;
}

//--------------------------------------------------------------------------
void CLight::setDirection(const QVector3D& vDirection)
{
	m_vDirection = vDirection;
}

//--------------------------------------------------------------------------
void CLight::setConstantAttenuation(real dAttenuation)
{
	m_dConstantAttenuation = dAttenuation;
}

//--------------------------------------------------------------------------
void CLight::setLinearAttenuation(real dAttenuation)
{
	m_dLinearAttenuation = dAttenuation;
}

//--------------------------------------------------------------------------
void CLight::setQuadraticAttenuation(real dAttenuation)
{
	m_dQuadraticAttenuation = dAttenuation;
}

//--------------------------------------------------------------------------
void CLight::setAmbientColor(const QVector4D& color)
{
	m_AmbientColor = color;
}

//--------------------------------------------------------------------------
void CLight::setAmbientColor(real r, real g, real b, real a /*= 1.0*/)
{
	m_AmbientColor = QVector4D(r, g, b, a);
}

//--------------------------------------------------------------------------
void CLight::setDiffuseColor(const QVector4D& color)
{
	m_DiffuseColor = color;
}

//--------------------------------------------------------------------------
void CLight::setDiffuseColor(real r, real g, real b, real a /*= 1.0*/)
{
	m_DiffuseColor = QVector4D(r, g, b, a);
}

//--------------------------------------------------------------------------
void CLight::setSpecularColor(const QVector4D& color)
{
	m_SpecularColor = color;
}

//--------------------------------------------------------------------------
void CLight::setSpecularColor(real r, real g, real b, real a /*= 1.0*/)
{
	m_SpecularColor = QVector4D(r, g, b, a);
}

//--------------------------------------------------------------------------
void CLight::setInnerConeAngle(real dAngle)
{
	m_dInnerConeAngle = dAngle;
}

//--------------------------------------------------------------------------
void CLight::setOuterConeAngle(real dAngle)
{
	m_dOuterConeAngle = dAngle;
}

//--------------------------------------------------------------------------
// Getters
//--------------------------------------------------------------------------
bool CLight::isEnabled() const 
{
	return m_bIsEnabled;
}

//--------------------------------------------------------------------------
EnumLightSourceType CLight::getSourceType() const
{
	return m_eSourceType;
}

//--------------------------------------------------------------------------
QVector3D CLight::getDirection() const
{
	return m_vDirection;
}

//--------------------------------------------------------------------------
real CLight::getConstantAttenuation() const
{
	return m_dConstantAttenuation;
}

//--------------------------------------------------------------------------
real CLight::getLinearAttenuation() const
{
	return m_dLinearAttenuation;
}

//--------------------------------------------------------------------------
real CLight::getQuadraticAttenuation() const
{
	return m_dQuadraticAttenuation;
}

//--------------------------------------------------------------------------
QVector4D CLight::getDiffuseColor() const
{
	return m_DiffuseColor;
}

//--------------------------------------------------------------------------
QVector4D CLight::getSpecularColor() const
{
	return m_SpecularColor;
}

//--------------------------------------------------------------------------
QVector4D CLight::getAmbientColor() const
{
	return m_AmbientColor;
}

//--------------------------------------------------------------------------
real CLight::getInnerConeAngle() const
{
	return m_dInnerConeAngle;
}

//--------------------------------------------------------------------------
real CLight::getOuterConeAngle() const
{
	return m_dOuterConeAngle;
}

//--------------------------------------------------------------------------
QMatrix4x4 CLight::getViewMatrix() const
{
	QMatrix4x4 mat;
    mat.lookAt(QVector3D(), m_vDirection, QVector3D(0., 1., 0.));
	return mat;
}

//--------------------------------------------------------------------------
QMatrix4x4 CLight::getProjectionMatrix() const
{
    // TODO
    return QMatrix4x4(); //.perspective(m_dOuterConeAngle, dRatio, 1.0, m_light.LightRaduis);
}
