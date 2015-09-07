#include "CGeometryGlobal.h"

//-----------------------------------------------------------------------------------------
QList<Color> Color::s_ColorList = QList<Color>()
    << Color(Color::eWhite)
    << Color(Color::eRed)
    << Color(Color::eGreen)
    << Color(Color::eBlue)
    << Color(Color::eBlack)
    << Color(Color::eYellow)
    << Color(Color::eCyan)
    << Color(Color::eMagenta)
    << Color(Color::eGray)
    << Color(Color::eEndColor);

//-----------------------------------------------------------------------------------------
QMap<EnumRasterizationMode, QString> CGeometryGlobal::rasterizationModeMap = CGeometryGlobal::initRasterizationModeMap();

QMap<EnumRasterizationMode, QString> CGeometryGlobal::initRasterizationModeMap()
{
	QMap<EnumRasterizationMode, QString> map;
    map[eRasterizationFill]     = "RasterizationModeFill";
    map[eRasterizationLine]     = "RasterizationModeLine";
    map[eRasterizationPoint]	= "RasterizationModePoint";
	return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumBBoxPolicy, QString> CGeometryGlobal::bboxPolicyMap = CGeometryGlobal::initBBoxPolicyMap();

QMap<EnumBBoxPolicy, QString> CGeometryGlobal::initBBoxPolicyMap()
{
	QMap<EnumBBoxPolicy, QString> map;
	map[eNeverVisible]		= "NeverVisible";
	map[eAlwaysVisible]		= "AlwaysVisible";
	map[eVisibleIfSelected]	= "VisibleIfSelected";
	return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumLightSourceType, QString> CGeometryGlobal::lightSourceTypeMap = CGeometryGlobal::initLightSourceTypeMap();

QMap<EnumLightSourceType, QString> CGeometryGlobal::initLightSourceTypeMap()
{
	QMap<EnumLightSourceType, QString> map;
	map[eSourceDirectionnal]	= "Directionnal";
	map[eSourcePoint]			= "Point";
	map[eSourceSpot]			= "Spot";
	return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumMaterialParameter, QString> CGeometryGlobal::materialParameterMap = CGeometryGlobal::initMaterialParameterMap();

QMap<EnumMaterialParameter, QString> CGeometryGlobal::initMaterialParameterMap()
{
	QMap<EnumMaterialParameter, QString> map;
	map[eUndefined]		= "Undefined";
	map[eDiffuse]		= "Diffuse";
	map[eSpecular]		= "Specular";
	map[eAmbient]		= "Ambient";
	map[eEmissive]		= "Emissive";
	map[eHeight]		= "Height";
	map[eNormals]		= "Normals";
	map[eShininess]		= "Shininess";
	map[eOpacity]		= "Opacity";
	map[eDisplacement]	= "Displacement";
	map[eLightmap]		= "Lightmap";
	map[eReflection]	= "Reflection";
	return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumAlphaMaskFunc, QString> CGeometryGlobal::materialAlphaMaskMap = CGeometryGlobal::initAlphaMaskFuncMap();

QMap<EnumAlphaMaskFunc, QString> CGeometryGlobal::initAlphaMaskFuncMap()
{
	QMap<EnumAlphaMaskFunc, QString> map;
    map[eAlphaMaskNever]			= "Never";
    map[eAlphaMaskLess]				= "Less";
    map[eAlphaMaskEqual]			= "Equal";
    map[eAlphaMaskLessOrEqual]		= "LessOrEqual";
    map[eAlphaMaskGreater]			= "Greater";
    map[eAlphaMaskNotEqual]			= "NotEqual";
    map[eAlphaMaskGreaterOrEqual]	= "GreaterOrEqual";
    map[eAlphaMaskAlways]			= "Always";
	return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumTextureType, QString> CGeometryGlobal::textureTypeMap = CGeometryGlobal::initTextureTypeMap();

QMap<EnumTextureType, QString> CGeometryGlobal::initTextureTypeMap()
{
	QMap<EnumTextureType, QString> map;
	map[eTextureUndefined]	= "Undefined";
	map[eTexture2D]			= "2D";
	map[eTexture3D]			= "3D";
	map[eTextureCube]		= "Cube";
    map[eTextureTarget]     = "Target";
	return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumTarget, QString> CGeometryGlobal::targetTypeMap = CGeometryGlobal::initTargetTypeMap();

QMap<EnumTarget, QString> CGeometryGlobal::initTargetTypeMap()
{
	QMap<EnumTarget, QString> map;
	map[eTargetScreen]		= "Screen";
	map[eTargetTexture]		= "Texture";
	map[eTargetQuadScreen]	= "Quad";
	return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumInternalFormat, QString> CGeometryGlobal::internalFormatMap = CGeometryGlobal::initInternalFormatMap();

QMap<EnumInternalFormat, QString> CGeometryGlobal::initInternalFormatMap()
{
    QMap<EnumInternalFormat, QString> map;
    map[eInternalFormatRGB]             = "RGB";
    map[eInternalFormatDepthComponent]	= "DepthComponent";
    map[eInternalFormatStencilIndex]	= "StencilIndex";
    return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumProjectionType, QString> CGeometryGlobal::projectionTypeMap = CGeometryGlobal::initProjectionTypeMap();

QMap<EnumProjectionType, QString> CGeometryGlobal::initProjectionTypeMap()
{
	QMap<EnumProjectionType, QString> map;
	map[eProjectionOrtho]			= "Ortho";
	map[eProjectionPerspective]		= "Perspective";
	return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumAnimationBehaviour, QString> CGeometryGlobal::animationBehaviorTypeMap = CGeometryGlobal::initAnimationBehaviorTypeMap();

QMap<EnumAnimationBehaviour, QString> CGeometryGlobal::initAnimationBehaviorTypeMap()
{
	QMap<EnumAnimationBehaviour, QString> map;
	map[eAnimationBehaviourDefault]		= "Default";
	map[eAnimationBehaviourConstant]	= "Constant";
	map[eAnimationBehaviourLinear]		= "Linear";
	map[eAnimationBehaviourRepeat]		= "Repeat";
	return map;
}

//-----------------------------------------------------------------------------------------
QMap<EnumPrimitiveType, QString> CGeometryGlobal::primitiveTypeMap = CGeometryGlobal::initPrimitiveTypeMap();

QMap<EnumPrimitiveType, QString> CGeometryGlobal::initPrimitiveTypeMap()
{
	QMap<EnumPrimitiveType, QString> map;
	map[ePrimitivePoint]					= "Point";
	map[ePrimitiveLine]						= "Line";
	map[ePrimitiveLineLoop]					= "LineLoop";
	map[ePrimitiveLineStrip]				= "LineStrip";
	map[ePrimitiveTriangles]				= "Triangles";
	map[ePrimitiveTriangleStrip]			= "TriangleStrip";
	map[ePrimitiveTriangleFan]				= "TriangleFan";
	map[ePrimitiveLinesAdjacency]			= "LinesAjacency";
	map[ePrimitiveLineStripAdjacency]		= "LineStripAdjacency";
	map[ePrimitiveTrianglesAdjacency]		= "TrianglesAdjacency";
	map[ePrimitiveTrianglesStripAdjacency]	= "TrianglesStripAdjacency";
	return map;
}
//-----------------------------------------------------------------------------------------
QMap<EnumPhysicShape, QString> CGeometryGlobal::physicShapeMap = CGeometryGlobal::initPhysicShapeMap();

QMap<EnumPhysicShape, QString> CGeometryGlobal::initPhysicShapeMap()
{
	QMap<EnumPhysicShape, QString> map;
	map[eBoxShape]					= "BoxShape";
	map[eSphereShape]				= "SphereShape";
	map[eCapsuleShape]				= "CapsuleShape";
    map[eCylinderShapeX]            = "CylinderShapeX";
    map[eCylinderShapeY]            = "CylinderShapeY";
    map[eCylinderShapeZ]            = "CylinderShapeZ";
	map[eConeShape]					= "ConeShape";
	map[eMultiSphereShape]			= "MultiSphereShape";
	map[eCompoundShape]				= "CompoundShape";
	map[eConvexHullShape]			= "ConvexHullShape";
	map[eBvhTriangleMeshShape]		= "BvhTriangleMeshShape";
	map[eHeightfieldTerrainShape]	= "HeightfieldTerrainShape";
	map[eStaticPlaneShape]			= "StaticPlaneShape";
	return map;
}

