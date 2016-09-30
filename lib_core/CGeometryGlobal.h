#ifndef CGEOMETRYGLOBAL_H
#define CGEOMETRYGLOBAL_H

// Lib
#include "lib_core_global.h"

// STL
#include <algorithm>

// Foundations
#include "CMath.h"
#include "CBox3D.h"

// Qt
#include <QString>
#include <QVector3D>
#include <QVector4D>
#include <QColor>
#include <QMap>
#include <QFile>
#include <QDebug>

//--------------------------------------------------------------------------------------------
// Renderer
//--------------------------------------------------------------------------------------------
static const int DefaultDesiredFPS = 60;
static const int DefaultResolutionWidth = 512;
static const int DefaultResolutionHeight = 512;

//--------------------------------------------------------------------------------------------
// Axes
//--------------------------------------------------------------------------------------------

enum EnumAxis
{
    eAxisX,
    eAxisY,
    eAxisZ
};

//--------------------------------------------------------------------------------------------
// Bounding Box
//--------------------------------------------------------------------------------------------

//! Différents modes d'affichage des boîtes englobantes
enum EnumBBoxPolicy
{
    eNeverVisible,
    eAlwaysVisible,
    eVisibleIfSelected
};

//--------------------------------------------------------------------------------------------
// Intersections
//--------------------------------------------------------------------------------------------
enum EnumIntersectionType 
{ 
    eOutside,
    eIntersect,
    eInside
};

//--------------------------------------------------------------------------------------------
// Render States
//--------------------------------------------------------------------------------------------
enum EnumStencilOperation
{
    eZero,
    eInvert,
    eKeep,
    eReplace,
    eIncrement,
    eDecrement,
    eIncrementWrap,
    eDecrementWrap
};

enum EnumStencilTestFunction
{
    eNever,
    eLess,
    eEqual,
    eLessThanOrEqual,
    eGreater,
    eNotEqual,
    eGreaterThanOrEqual,
    eAlways
};

//--------------------------------------------------------------------------------------------
// Culling
//--------------------------------------------------------------------------------------------
enum EnumCullFace
{
    eFront,
    eBack,
    eFrontAndBack
};

enum EnumWindingOrder
{
    eClockwise,
    eCounterclockwise
};

//--------------------------------------------------------------------------------------------
// Buffer
//--------------------------------------------------------------------------------------------

static const unsigned int NoBuffer = 0;
static const unsigned int ColorBuffer = 1;
static const unsigned int DepthBuffer = 2;
static const unsigned int StencilBuffer = 4;

//--------------------------------------------------------------------------------------------
// Vertex buffer
//--------------------------------------------------------------------------------------------

//! Définit les différents types de primitive
enum EnumPrimitiveType
{
    ePrimitivePoint,
    ePrimitiveLine,
    ePrimitiveLineLoop,
    ePrimitiveLineStrip,
    ePrimitiveTriangles,
    ePrimitiveTriangleStrip,
    ePrimitiveTriangleFan,
    ePrimitiveLinesAdjacency,
    ePrimitiveLineStripAdjacency,
    ePrimitiveTrianglesAdjacency,
    ePrimitiveTrianglesStripAdjacency
};

//! Définit le type de pattern à utiliser
enum EnumUsagePattern
{
    eStreamDraw,
    eStreamRead,
    eStreamCopy,
    eStaticDraw,
    eStaticRead,
    eStaticCopy,
    eDynamicDraw,
    eDynamicRead,
    eDynamicCopy
};


//--------------------------------------------------------------------------------------------
// Couleurs
//--------------------------------------------------------------------------------------------

class Color
{
public:

    enum EnumColor
    {
        eWhite,
        eRed,
        eGreen,
        eBlue,
        eBlack,
        eYellow,
        eCyan,
        eMagenta,
        eGray,
        eEndColor
    };

    Color(EnumColor eColor) : m_eColor(eColor) {}

    QVector4D toVector4D() const
    {
        static const QVector4D vWhite	(1., 1., 1., 1.);
        static const QVector4D vRed		(1., 0., 0., 1.);
        static const QVector4D vGreen	(0., 1., 0., 1.);
        static const QVector4D vBlue	(0., 0., 1., 1.);
        static const QVector4D vBlack	(0., 0., 0., 1.);
        static const QVector4D vYellow	(1., 1., 0., 1.);
        static const QVector4D vCyan	(0., 1., 1., 1.);
        static const QVector4D vMagenta	(1., 0., 1., 1.);
        static const QVector4D vGray	(.5, .5, .5, 1.);

        switch (m_eColor)
        {
        case eWhite:	return vWhite;
        case eRed:		return vRed;
        case eGreen:	return vGreen;
        case eBlue:		return vBlue;
        case eBlack:	return vBlack;
        case eYellow:	return vYellow;
        case eCyan:		return vCyan;
        case eMagenta:	return vMagenta;
        case eGray:		return vGray;
        case eEndColor:
        default:		return vWhite; // avoid warning
        }
    }

    QString toString() const
    {
        switch (m_eColor)
        {
        case eWhite:	return "white";
        case eRed:		return "red";
        case eGreen:	return "green";
        case eBlue:		return "blue";
        case eBlack:	return "black";
        case eYellow:	return "yellow";
        case eCyan:		return "cyan";
        case eMagenta:	return "magenta";
        case eGray:		return "gray";
        case eEndColor: return "invalid";
        default:		return "white"; // avoid warning
        }
    }

    unsigned int toOrder() const
    {
        return (unsigned int) m_eColor;
    }

    static QList<Color> getColorList() { return s_ColorList; }

private:

    EnumColor m_eColor;
    static QList<Color> s_ColorList;
};

//--------------------------------------------------------------------------------------------
// Renderer
//--------------------------------------------------------------------------------------------

//! Différents modes de rasterization
enum EnumRasterizationMode
{
    eRasterizationPoint,
    eRasterizationLine,
    eRasterizationFill
};

//--------------------------------------------------------------------------------------------
// Renderer
//--------------------------------------------------------------------------------------------

//! Différents modes de filtrage
enum EnumFilterType
{
    eNearest,
    eLinear
};


//--------------------------------------------------------------------------------------------
// Blending
//--------------------------------------------------------------------------------------------

struct Source
{
    enum EnumBlendingFactor
    {
        eZero,
        eOne,
        eSourceAlpha,
        eOneMinusSourceAlpha,
        eDestinationAlpha,
        eOneMinusDestinationAlpha,
        eDestinationColor,
        eOneMinusDestinationColor,
        eSourceAlphaSaturate,
        eConstantColor,
        eOneMinusConstantColor,
        eConstantAlpha,
        eOneMinusConstantAlpha
    };
};

struct Destination
{
    enum EnumBlendingFactor
    {
        eZero,
        eOne,
        eSourceColor,
        eOneMinusSourceColor,
        eSourceAlpha,
        eOneMinusSourceAlpha,
        eDestinationAlpha,
        eOneMinusDestinationAlpha,
        eDestinationColor,
        eOneMinusDestinationColor,
        eConstantColor,
        eOneMinusConstantColor,
        eConstantAlpha,
        eOneMinusConstantAlpha
    };
};

enum EnumBlendEquation
{
    eAdd,
    eMinimum,
    eMaximum,
    eSubtract,
    eReverseSubtract
};

//--------------------------------------------------------------------------------------------
// Shaders
//--------------------------------------------------------------------------------------------

//! Types de shaders
enum EnumShaderType
{
    eVertexShader,
    eGeometryShader,
    eFragmentShader,
    eTessellationControlShader,
    eTessellationEvaluationShader,
    eComputeShader
};

//! Nombre de canaux de texture maximum
static const int DefaultMaxTextureChannelCount	= 4;

//! Active ou desactive la définition de la taille d'un point dans le shader
enum EnumProgramPointSize
{
    eEnabled,
    eDisabled
};


static const QString DefaultShaderName = "phong";

//--------------------------------------------------------------------------------------------
// Multi-Pass
//--------------------------------------------------------------------------------------------
enum EnumTarget
{
    eTargetScreen,
    eTargetTexture,
    eTargetQuadScreen
};

enum EnumBufferType
{
    eBufferTypeTexture,
    eBufferTypeRenderBuffer
};

enum EnumInternalFormat
{
    eInternalFormatRGB,
    eInternalFormatDepthComponent,
    eInternalFormatStencilIndex
};

enum EnumAttachment
{
    eAttachmentDepth,
    eAttachmentStencil,
    eAttachmentColor0,
    eAttachmentColor1,
    eAttachmentColor2,
    eAttachmentColor3,
    eAttachmentColor4,
    eAttachmentColor5,
    eAttachmentColor6,
    eAttachmentColor7,
    eAttachmentColor8,
    eAttachmentColor9,
    eAttachmentColor10,
    eAttachmentColor11,
    eAttachmentColor12,
    eAttachmentColor13,
    eAttachmentColor14,
    eAttachmentColor15
};

enum EnumDrawBuffer
{
    eDrawBufferNone,
    eDrawBufferFrontLeft,
    eDrawBufferFrontRight,
    eDrawBufferBackLeft,
    eDrawBufferBackRight,
    eDrawBufferAttachmentColor0,
    eDrawBufferAttachmentColor1,
    eDrawBufferAttachmentColor2,
    eDrawBufferAttachmentColor3,
    eDrawBufferAttachmentColor4,
    eDrawBufferAttachmentColor5,
    eDrawBufferAttachmentColor6,
    eDrawBufferAttachmentColor7,
    eDrawBufferAttachmentColor8,
    eDrawBufferAttachmentColor9,
    eDrawBufferAttachmentColor10,
    eDrawBufferAttachmentColor11,
    eDrawBufferAttachmentColor12,
    eDrawBufferAttachmentColor13,
    eDrawBufferAttachmentColor14,
    eDrawBufferAttachmentColor15
};

//--------------------------------------------------------------------------------------------
// Camera
//--------------------------------------------------------------------------------------------

//! Type de caméra
enum EnumProjectionType
{
    eProjectionOrtho,
    eProjectionPerspective,
    eProjectionInfinitePerspective
};

//! Ordre des rotations
enum EnumRotateOrder
{
    eTiltPanRoll,
    eTiltRollPan,
    ePanTiltRoll,
    ePanRollTilt,
    eRollTiltPan,
    eRollPanTilt
};

//--------------------------------------------------------------------------------------------
// Animations
//--------------------------------------------------------------------------------------------

//! Comportement à adopter entre deux clefs d'animation
enum EnumAnimationBehaviour
{
    eAnimationBehaviourDefault,		// The value from the default node transformation is taken
    eAnimationBehaviourConstant,	// The nearest key value is used without interpolation
    eAnimationBehaviourLinear,		// The value of the nearest two keys is linearly extrapolated for the current time value
    eAnimationBehaviourRepeat		// The animation is repeated.If the animation key go from n to m and the current time is t,
    // use the value at (t-n) % (|m-n|).
};

//--------------------------------------------------------------------------------------------
// Textures
//--------------------------------------------------------------------------------------------

//! Types de cible (2D, 3D, Cube...)
enum EnumTextureType
{
    eTextureUndefined,
    eTexture2D,
    eTexture3D,
    eTextureCube,
    eTextureTarget
};

//--------------------------------------------------------------------------------------------
// Lights
//--------------------------------------------------------------------------------------------

static const unsigned int MaxLightCount = 8;

//! Type de source lumineuse
enum EnumLightSourceType
{
    eSourceDirectionnal,
    eSourcePoint,
    eSourceSpot
};

//--------------------------------------------------------------------------------------------
// Meshs
//--------------------------------------------------------------------------------------------
enum EnumMeshType
{
    eMeshDynamic,
    eMeshStatic
};


//--------------------------------------------------------------------------------------------
// Materials
//--------------------------------------------------------------------------------------------
static const QVector4D	DefaultMaterialAmbientColor(0.8f, 0.5f, 0.3f, 1.0f);
static const QVector4D	DefaultMaterialDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
static const QVector4D	DefaultMaterialSpecularColor(0.8f, 0.5f, 0.4f, 1.0f);
static const QVector4D	DefaultMaterialAlphaMaskColor(1.0f, 1.0f, 1.0f, 1.0f);
static const real       DefaultMaterialShininessFactor = 50.f;

//! Types de paramétre de matériau
enum EnumMaterialParameter
{
    eUndefined,
    eDiffuse,
    eSpecular,
    eAmbient,
    eEmissive,
    eHeight,
    eNormals,
    eShininess,
    eOpacity,
    eDisplacement,
    eLightmap,
    eReflection
};


enum EnumAlphaMaskFunc
{
    eAlphaMaskNever,
    eAlphaMaskLess,
    eAlphaMaskEqual,
    eAlphaMaskLessOrEqual,
    eAlphaMaskGreater,
    eAlphaMaskNotEqual,
    eAlphaMaskGreaterOrEqual,
    eAlphaMaskAlways
};


//--------------------------------------------------------------------------------------------
// Lights
//--------------------------------------------------------------------------------------------
enum EnumLightParameter
{
    eIsEnabled,
    ePosition,
    eDirection,
    eAmbientColor,
    eDiffuseColor,
    eSpecularColor,
    eConstantAttenuation,
    eLinearAttenuation,
    eQuadraticAttenuation,
    eEndLightParameter
};


//--------------------------------------------------------------------------------------------
// Squelette
//--------------------------------------------------------------------------------------------

static const unsigned int DefaultMaxBonesPerVertex = 1;
static const unsigned int MaxBoneTransformations = 100;

//--------------------------------------------------------------------------------------------
// Physique
//--------------------------------------------------------------------------------------------

enum EnumPhysicShape
{
    eBoxShape,
    eSphereShape,
    eCapsuleShape,
    eTetrahedron,
    eCylinderShapeX,
    eCylinderShapeY,
    eCylinderShapeZ,
    eConeShape,
    eMultiSphereShape,
    eCompoundShape,
    eConvexHullShape,
    eBvhTriangleMeshShape,
    eHeightfieldTerrainShape,
    eStaticPlaneShape
};

//--------------------------------------------------------------------------------------------
// Sort by Z Value utilitary class
//--------------------------------------------------------------------------------------------
class CSortByZValue
{
public:
    CSortByZValue(const QMatrix4x4& MVMatrix)
        : m_dMVMatrix20(MVMatrix(2, 0))
        , m_dMVMatrix21(MVMatrix(2, 1))
        , m_dMVMatrix22(MVMatrix(2, 2))
    {}

    inline bool operator()(const QVector3D& left, const QVector3D& right) const
    {
        return left.x() * m_dMVMatrix20 + left.y() * m_dMVMatrix21 + left.z() * m_dMVMatrix22 <
                right.x() * m_dMVMatrix20 + right.y() * m_dMVMatrix21 + right.z() * m_dMVMatrix22;
    }

private:
    real m_dMVMatrix20;
    real m_dMVMatrix21;
    real m_dMVMatrix22;
};

//--------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------

class LIB_CORE_SHARED_EXPORT CGeometryGlobal
{
public:

    //-----------------------------------------------------------------------------------------
    // COULEURS
    //-----------------------------------------------------------------------------------------

    static QVector4D QVector4DFromQColor(const QColor& color)
    {
        return QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    }

    static QColor QColorFromQVector4D(const QVector4D& vector)
    {
        return QColor((int) (255 * vector.x()), (int) (255 * vector.y()), (int) (255 * vector.z()), (int) (255 * vector.w()));
    }

    //-----------------------------------------------------------------------------------------
    // VECTEURS
    //-----------------------------------------------------------------------------------------

    static QString stringFromQVector3D(const QVector3D& vector)
    {
        return QString::number(vector.x()) + ", " +
                QString::number(vector.y()) + ", " +
                QString::number(vector.z());
    }

    static QString stringFromQVector4D(const QVector4D& vector)
    {
        return QString::number(vector.x()) + ", " +
                QString::number(vector.y()) + ", " +
                QString::number(vector.z()) + ", " +
                QString::number(vector.w());
    }

    static QVector3D QVector3DFromQString(const QString& string)
    {
        QStringList sList = string.split(",", QString::SkipEmptyParts);

        return QVector3D(sList.size() > 0 ? sList[0].toDouble() : 0.,
                         sList.size() > 1 ? sList[1].toDouble() : 0.,
                         sList.size() > 2 ? sList[2].toDouble() : 0.);

    }

    static QVector4D QVector4DFromQString(const QString& string)
    {
        QStringList sList = string.split(",", QString::SkipEmptyParts);

        return QVector4D(sList.size() > 0 ? sList[0].toDouble() : 0.,
                         sList.size() > 1 ? sList[1].toDouble() : 0.,
                         sList.size() > 2 ? sList[2].toDouble() : 0.,
                         sList.size() > 3 ? sList[3].toDouble() : 0.);
    }

    //-----------------------------------------------------------------------------------------
    // PRIMITIVES
    //-----------------------------------------------------------------------------------------

    static EnumPrimitiveType fromRasterizationMode(EnumRasterizationMode eMode, EnumPrimitiveType eType)
    {
        switch (eMode)
        {
        case eRasterizationFill:
            return eType;
        case eRasterizationLine:
            if (eType == ePrimitiveTriangles)
                return ePrimitiveLine;
            if (eType == ePrimitiveTriangleStrip || eType == ePrimitiveTriangleFan)
                return ePrimitiveLineStrip;
            if (eType == ePrimitiveTrianglesAdjacency)
                return ePrimitiveLinesAdjacency;
            if (eType == ePrimitiveTrianglesStripAdjacency)
                return ePrimitiveLineStripAdjacency;
            else
                return eType;
        default:
        case eRasterizationPoint:
            return ePrimitivePoint;
        }
    }

    //-----------------------------------------------------------------------------------------
    // TYPE DE BASE
    //-----------------------------------------------------------------------------------------

    static real scalaireFromQString(const QString& string)
    {
        return string.trimmed().toDouble();
    }

    static bool boolFromQString(const QString& string)
    {
        return (bool) string.trimmed().toInt();
    }

    //-----------------------------------------------------------------------------------------
    // MODE DE REMPLISSAGE
    //-----------------------------------------------------------------------------------------

    static EnumRasterizationMode rasterizationModeFromString(const QString& string)
    {
        return rasterizationModeMap.key(string, eRasterizationFill);
    }

    static QString stringFromRasterizationMode(EnumRasterizationMode eRasterizationMode)
    {
        return rasterizationModeMap.value(eRasterizationMode, "PolygonModeFill");
    }

    static QList<QString> rasterizationModeEntries()
    {
        return rasterizationModeMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // POLITIQUE D'AFFICHAGE DES BOITES ENGLOBANTES
    //-----------------------------------------------------------------------------------------

    static EnumBBoxPolicy bboxPolicyFromString(const QString& string)
    {
        return bboxPolicyMap.key(string, eNeverVisible);
    }

    static QString stringFromBBoxPolicy(EnumBBoxPolicy eBBoxPolicy)
    {
        return bboxPolicyMap.value(eBBoxPolicy, "NeverVisible");
    }

    static QList<QString> bboxPolicyEntries()
    {
        return bboxPolicyMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // TYPE DE SOURCE LUMINEUSE
    //-----------------------------------------------------------------------------------------

    static EnumLightSourceType lightSourceTypeFromString(const QString& string)
    {
        return lightSourceTypeMap.key(string, eSourceDirectionnal);
    }

    static QString stringFromLightSourceType(EnumLightSourceType eLightSourceType)
    {
        return lightSourceTypeMap.value(eLightSourceType, "Directionnal");
    }

    static QList<QString> lightSourceTypeEntries()
    {
        return lightSourceTypeMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // PARAMETRE DE MATERIAU
    //-----------------------------------------------------------------------------------------

    static EnumMaterialParameter materialParameterFromString(const QString& string)
    {
        return materialParameterMap.key(string, eUndefined);
    }

    static QString stringFromMaterialParameter(EnumMaterialParameter eMaterialParameter)
    {
        return materialParameterMap.value(eMaterialParameter, "Undefined");
    }

    static QList<QString> materialParameterEntries()
    {
        return materialParameterMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // FONCTION ALPHA MASK
    //-----------------------------------------------------------------------------------------

    static EnumAlphaMaskFunc alphaMaskFuncFromString(const QString& string)
    {
        return materialAlphaMaskMap.key(string, eAlphaMaskAlways);
    }

    static QString stringFromAlphaMaskFunc(EnumAlphaMaskFunc eAlphaMaskFunc)
    {
        return materialAlphaMaskMap.value(eAlphaMaskFunc, "Always");
    }

    static QList<QString> alphaMaskFuncEntries()
    {
        return materialAlphaMaskMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // TEXTURES
    //-----------------------------------------------------------------------------------------

    static EnumTextureType textureTypeFromString(const QString& string)
    {
        return textureTypeMap.key(string, eTextureUndefined);
    }

    static QString stringFromTextureType(EnumTextureType eTextureType)
    {
        return textureTypeMap.value(eTextureType, "Undefined");
    }

    static QList<QString> textureTypeEntries()
    {
        return textureTypeMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // RENDERING TARGET
    //-----------------------------------------------------------------------------------------

    static EnumTarget targetTypeFromString(const QString& string)
    {
        return targetTypeMap.key(string, eTargetScreen);
    }

    static QString stringFromTargetType(EnumTarget eTarget)
    {
        return targetTypeMap.value(eTarget, "Screen");
    }

    static QList<QString> targetTypeEntries()
    {
        return targetTypeMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // INTERNAL FORMAT
    //-----------------------------------------------------------------------------------------

    static EnumInternalFormat internalFormatFromString(const QString& string)
    {
        return internalFormatMap.key(string, eInternalFormatRGB);
    }

    static QString stringFromInternalFormat(EnumInternalFormat eTarget)
    {
        return internalFormatMap.value(eTarget, "RGB");
    }

    static QList<QString> internalFormatEntries()
    {
        return internalFormatMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // TYPE DE PROJECTION
    //-----------------------------------------------------------------------------------------

    static EnumProjectionType projectionTypeFromString(const QString& string)
    {
        return projectionTypeMap.key(string, eProjectionOrtho);
    }

    static QString stringFromProjectionType(EnumProjectionType eProjectionType)
    {
        return projectionTypeMap.value(eProjectionType, "Ortho");
    }

    static QList<QString> projectionTypeEntries()
    {
        return projectionTypeMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // ANIMATIONS
    //-----------------------------------------------------------------------------------------

    static EnumAnimationBehaviour animationBehaviorTypeFromString(const QString& string)
    {
        return animationBehaviorTypeMap.key(string, eAnimationBehaviourConstant);
    }

    static QString stringFromAnimationBehaviorType(EnumAnimationBehaviour eAnimationBehaviorType)
    {
        return animationBehaviorTypeMap.value(eAnimationBehaviorType, "Default");
    }

    static QList<QString> animationBehaviorTypeEntries()
    {
        return animationBehaviorTypeMap.values();
    }

    //-----------------------------------------------------------------------------------------
    // PRIMITIVES
    //-----------------------------------------------------------------------------------------

    static EnumPrimitiveType primitiveTypeFromString(const QString& string)
    {
        return primitiveTypeMap.key(string, ePrimitivePoint);
    }

    static QString stringFromPrimitiveType(EnumPrimitiveType ePrimitiveType)
    {
        return primitiveTypeMap.value(ePrimitiveType, "Point");
    }

    static QList<QString> primitiveTypeEntries()
    {
        return primitiveTypeMap.values();
    }


    static EnumPrimitiveType fromRasterizationMode(EnumPrimitiveType eIn, EnumRasterizationMode eMode)
    {
        switch (eMode)
        {
        case eRasterizationFill:
            return eIn;
        case eRasterizationLine:
            if (eIn == ePrimitiveTriangles)
                return ePrimitiveLine;
            if (eIn == ePrimitiveTriangleStrip || eIn == ePrimitiveTriangleFan)
                return ePrimitiveLineStrip;
            if (eIn == ePrimitiveTrianglesAdjacency)
                return ePrimitiveLinesAdjacency;
            if (eIn == ePrimitiveTrianglesStripAdjacency)
                return ePrimitiveLineStripAdjacency;
            else
                return eIn;
        default:
        case eRasterizationPoint:
            return ePrimitivePoint;
        }
    }

    //-----------------------------------------------------------------------------------------
    // PHYSICS SHAPES
    //-----------------------------------------------------------------------------------------

    static EnumPhysicShape physicShapeFromString(const QString& string)
    {
        return physicShapeMap.key(string, eBoxShape);
    }

    static QString stringFromPhysicShape(EnumPhysicShape ePhysicShape)
    {
        return physicShapeMap.value(ePhysicShape, "BoxShape");
    }

    static QList<QString> physicShapeEntries()
    {
        return physicShapeMap.values();
    }

private:

    static QMap<EnumRasterizationMode, QString> rasterizationModeMap;
    static QMap<EnumRasterizationMode, QString> initRasterizationModeMap();

    static QMap<EnumBBoxPolicy, QString> bboxPolicyMap;
    static QMap<EnumBBoxPolicy, QString> initBBoxPolicyMap();

    static QMap<EnumLightSourceType, QString> lightSourceTypeMap;
    static QMap<EnumLightSourceType, QString> initLightSourceTypeMap();

    static QMap<EnumMaterialParameter, QString> materialParameterMap;
    static QMap<EnumMaterialParameter, QString> initMaterialParameterMap();

    static QMap<EnumAlphaMaskFunc, QString> materialAlphaMaskMap;
    static QMap<EnumAlphaMaskFunc, QString> initAlphaMaskFuncMap();

    static QMap<EnumTextureType, QString> textureTypeMap;
    static QMap<EnumTextureType, QString> initTextureTypeMap();

    static QMap<EnumTarget, QString> targetTypeMap;
    static QMap<EnumTarget, QString> initTargetTypeMap();

    static QMap<EnumInternalFormat, QString> internalFormatMap;
    static QMap<EnumInternalFormat, QString> initInternalFormatMap();

    static QMap<EnumProjectionType, QString> projectionTypeMap;
    static QMap<EnumProjectionType, QString> initProjectionTypeMap();

    static QMap<EnumAnimationBehaviour, QString> animationBehaviorTypeMap;
    static QMap<EnumAnimationBehaviour, QString> initAnimationBehaviorTypeMap();

    static QMap<EnumPrimitiveType, QString> primitiveTypeMap;
    static QMap<EnumPrimitiveType, QString> initPrimitiveTypeMap();

    static QMap<EnumPhysicShape, QString> physicShapeMap;
    static QMap<EnumPhysicShape, QString> initPhysicShapeMap();
};

#endif // CGEOMETRYGLOBAL_H
