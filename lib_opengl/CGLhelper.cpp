#include "CGLHelper.h"


//-------------------------------------------------------------------------------------------------
CGLHelper::CGLHelper()
{
    for (unsigned int i = 0; i < MaxLightCount; ++i)
    {
        for (int e = 0; e < eEndLightParameter; ++e)
        {
            m_LightParamStr.insert(TLightParam(i, (EnumLightParameter)e), fromLightParam(i, (EnumLightParameter)e));
        }
    }

    m_MaterialParameterName
            << "texture"
            << "textureDiffuse"
            << "textureSpecular"
            << "textureAmbient"
            << "textureEmissive"
            << "textureHeight"
            << "textureNormals"
            << "textureShininess"
            << "textureOpacity"
            << "textureDisplacement"
            << "textureLightmap"
            << "textureReflection";

    m_MaterialParameterCountName
            << "textureCount"
            << "textureDiffuseCount"
            << "textureSpecularCount"
            << "textureAmbientCount"
            << "textureEmissiveCount"
            << "textureHeightCount"
            << "textureNormalsCount"
            << "textureShininessCount"
            << "textureOpacityCount"
            << "textureDisplacementCount"
            << "textureLightmapCount"
            << "textureReflectionCount";
}

//-------------------------------------------------------------------------------------------------
CGLHelper& CGLHelper::getInstance()
{
    static CGLHelper instance;
    return instance;
}

//-------------------------------------------------------------------------------------------------
QByteArray CGLHelper::getLightParamStr(const TLightParam& param) const
{
    return m_LightParamStr[param];
}

//-------------------------------------------------------------------------------------------------
const QString& CGLHelper::fromType(EnumMaterialParameter eType)
{
    return m_MaterialParameterName[eType];
}

//-------------------------------------------------------------------------------------------------
const QString& CGLHelper::fromTypeCount(EnumMaterialParameter eType)
{
    return m_MaterialParameterCountName[eType];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumPrimitiveType eType)
{
    static QMap<EnumPrimitiveType, GLenum> primitiveTypes {
        { ePrimitivePoint, GL_POINTS },
        { ePrimitiveLine, GL_LINES },
        { ePrimitiveLineLoop, GL_LINE_LOOP },
        { ePrimitiveLineStrip, GL_LINE_STRIP },
        { ePrimitiveTriangles, GL_TRIANGLES },
        { ePrimitiveTriangleStrip, GL_TRIANGLE_STRIP },
        { ePrimitiveTriangleFan, GL_TRIANGLE_FAN },
#ifdef DESKTOP_TARGET
        { ePrimitiveLinesAdjacency, GL_LINES_ADJACENCY },
        { ePrimitiveLineStripAdjacency, GL_LINE_STRIP_ADJACENCY },
        { ePrimitiveTrianglesAdjacency, GL_TRIANGLES_ADJACENCY },
        { ePrimitiveTrianglesStripAdjacency, GL_TRIANGLE_STRIP_ADJACENCY }
#endif
    };
    return primitiveTypes[eType];
}

//-------------------------------------------------------------------------------------------------
CGLBuffer::UsagePattern CGLHelper::toQtType(EnumUsagePattern ePattern)
{
    static QMap<EnumUsagePattern, CGLBuffer::UsagePattern> usagePatterns {
        { eStreamDraw, CGLBuffer::StreamDraw },
        { eStreamRead, CGLBuffer::StreamRead },
        { eStreamCopy, CGLBuffer::StreamCopy },
        { eStaticDraw, CGLBuffer::StaticDraw },
        { eStaticRead, CGLBuffer::StaticRead },
        { eStaticCopy, CGLBuffer::StaticCopy },
        { eDynamicDraw, CGLBuffer::DynamicDraw },
        { eDynamicRead, CGLBuffer::DynamicRead },
        { eDynamicCopy, CGLBuffer::DynamicCopy }
    };
    return usagePatterns[ePattern];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(CDepthTest::EnumDepthTestFunction eFunction)
{
    static QMap<CDepthTest::EnumDepthTestFunction, GLenum> functions {
        { CDepthTest::eNever, GL_NEVER },
        { CDepthTest::eLess, GL_LESS },
        { CDepthTest::eEqual, GL_EQUAL },
        { CDepthTest::eLessThanOrEqual, GL_LEQUAL },
        { CDepthTest::eGreater, GL_GREATER },
        { CDepthTest::eNotEqual, GL_NOTEQUAL },
        { CDepthTest::eGreaterThanOrEqual, GL_GEQUAL },
        { CDepthTest::eAlways, GL_ALWAYS }
    };
    return functions[eFunction];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(Source::EnumBlendingFactor eFactor)
{
    static QMap<Source::EnumBlendingFactor, GLenum> blendingFactors {
        { Source::eZero, GL_ZERO },
        { Source::eOne, GL_ONE },
        { Source::eSourceAlpha, GL_SRC_ALPHA },
        { Source::eOneMinusSourceAlpha, GL_ONE_MINUS_SRC_ALPHA },
        { Source::eDestinationAlpha, GL_DST_ALPHA },
        { Source::eOneMinusDestinationAlpha, GL_ONE_MINUS_DST_ALPHA },
        { Source::eDestinationColor, GL_DST_COLOR },
        { Source::eOneMinusDestinationColor, GL_ONE_MINUS_DST_COLOR },
        { Source::eSourceAlphaSaturate, GL_SRC_ALPHA_SATURATE },
        { Source::eConstantColor, GL_CONSTANT_COLOR },
        { Source::eOneMinusConstantColor, GL_ONE_MINUS_CONSTANT_COLOR },
        { Source::eConstantAlpha, GL_CONSTANT_ALPHA },
        { Source::eOneMinusConstantAlpha, GL_ONE_MINUS_CONSTANT_ALPHA }
    };
    return blendingFactors[eFactor];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(Destination::EnumBlendingFactor eFactor)
{
    static QMap<Destination::EnumBlendingFactor, GLenum> blendingFactors {
        { Destination::eZero, GL_ZERO },
        { Destination::eOne, GL_ONE },
        { Destination::eSourceColor, GL_SRC_COLOR },
        { Destination::eOneMinusSourceColor, GL_ONE_MINUS_SRC_COLOR },
        { Destination::eSourceAlpha, GL_SRC_ALPHA },
        { Destination::eOneMinusSourceAlpha, GL_ONE_MINUS_SRC_ALPHA },
        { Destination::eDestinationAlpha, GL_DST_ALPHA },
        { Destination::eOneMinusDestinationAlpha, GL_ONE_MINUS_DST_ALPHA },
        { Destination::eDestinationColor, GL_DST_COLOR },
        { Destination::eOneMinusDestinationColor, GL_ONE_MINUS_DST_COLOR },
        { Destination::eConstantColor, GL_CONSTANT_COLOR },
        { Destination::eOneMinusConstantColor, GL_ONE_MINUS_CONSTANT_COLOR },
        { Destination::eConstantAlpha, GL_CONSTANT_ALPHA },
        { Destination::eOneMinusConstantAlpha, GL_ONE_MINUS_CONSTANT_ALPHA }
    };
    return blendingFactors[eFactor];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumBlendEquation eEquation)
{
    static QMap<EnumBlendEquation, GLenum> equations {
        { eAdd, GL_FUNC_ADD },
        { eMinimum, GL_MIN },
        { eMaximum, GL_MAX },
        { eSubtract, GL_FUNC_SUBTRACT },
        { eReverseSubtract, GL_FUNC_REVERSE_SUBTRACT }
    };
    return equations[eEquation];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumCullFace eCullFace)
{
    static QMap<EnumCullFace, GLenum> cullFaceMode {
        { eFront, GL_FRONT },
        { eBack, GL_BACK },
        { eFrontAndBack, GL_FRONT_AND_BACK }
    };
    return cullFaceMode[eCullFace];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumWindingOrder eWindingOrder)
{
    static QMap<EnumWindingOrder, GLenum> windingOrders {
        { eClockwise, GL_CW },
        { eCounterclockwise, GL_CCW }
    };
    return windingOrders[eWindingOrder];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumStencilOperation eOperation)
{
    static QMap<EnumStencilOperation, GLenum> operations {
        { eZero, GL_ZERO },
        { eInvert, GL_INVERT },
        { eKeep, GL_KEEP },
        { eReplace, GL_REPLACE },
        { eIncrement, GL_INCR },
        { eDecrement, GL_DECR },
        { eIncrementWrap, GL_INCR_WRAP },
        { eDecrementWrap, GL_DECR_WRAP }
    };
    return operations[eOperation];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumStencilTestFunction eFunction)
{
    static QMap<EnumStencilTestFunction, GLenum> functions {
        { eNever, GL_NEVER },
        { eLess, GL_LESS },
        { eEqual, GL_EQUAL },
        { eLessThanOrEqual, GL_LEQUAL },
        { eGreater, GL_GREATER },
        { eNotEqual, GL_NOTEQUAL },
        { eGreaterThanOrEqual, GL_GEQUAL },
        { eAlways, GL_ALWAYS }
    };
    return functions[eFunction];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumAttachment eAttachment)
{
    static QMap<EnumAttachment, GLenum> attachments {
        { eAttachmentDepth, GL_DEPTH_ATTACHMENT },
        { eAttachmentStencil, GL_STENCIL_ATTACHMENT },
        { eAttachmentColor0, GL_COLOR_ATTACHMENT0 },
#ifdef DESKTOP_TARGET
        { eAttachmentColor1, GL_COLOR_ATTACHMENT1 },
        { eAttachmentColor2, GL_COLOR_ATTACHMENT2 },
        { eAttachmentColor3, GL_COLOR_ATTACHMENT3 },
        { eAttachmentColor4, GL_COLOR_ATTACHMENT4 },
        { eAttachmentColor5, GL_COLOR_ATTACHMENT5 },
        { eAttachmentColor6, GL_COLOR_ATTACHMENT6 },
        { eAttachmentColor7, GL_COLOR_ATTACHMENT7 },
        { eAttachmentColor8, GL_COLOR_ATTACHMENT8 },
        { eAttachmentColor9, GL_COLOR_ATTACHMENT9 },
        { eAttachmentColor10, GL_COLOR_ATTACHMENT10 },
        { eAttachmentColor11, GL_COLOR_ATTACHMENT11 },
        { eAttachmentColor12, GL_COLOR_ATTACHMENT12 },
        { eAttachmentColor13, GL_COLOR_ATTACHMENT13 },
        { eAttachmentColor14, GL_COLOR_ATTACHMENT14 },
        { eAttachmentColor15, GL_COLOR_ATTACHMENT15 }
#endif
    };
    return attachments[eAttachment];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumInternalFormat eFormat)
{
    static QMap<EnumInternalFormat, GLenum> formats {
        { eInternalFormatRGB, GL_RGBA },
        { eInternalFormatDepthComponent, GL_DEPTH_COMPONENT },
        { eInternalFormatStencilIndex, GL_STENCIL_INDEX }
    };
    return formats[eFormat];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumDrawBuffer eDrawBuffer)
{
    static QMap<EnumDrawBuffer, GLenum> drawBufferModes {
        { eDrawBufferNone, GL_NONE },
#ifdef DESKTOP_TARGET
        { eDrawBufferFrontLeft, GL_FRONT_LEFT },
        { eDrawBufferFrontRight, GL_FRONT_RIGHT },
        { eDrawBufferBackLeft, GL_BACK_LEFT },
        { eDrawBufferBackRight, GL_BACK_RIGHT },
#endif
        { eDrawBufferAttachmentColor0, GL_COLOR_ATTACHMENT0 },
#ifdef DESKTOP_TARGET
        { eDrawBufferAttachmentColor1, GL_COLOR_ATTACHMENT1 },
        { eDrawBufferAttachmentColor2, GL_COLOR_ATTACHMENT2 },
        { eDrawBufferAttachmentColor3, GL_COLOR_ATTACHMENT3 },
        { eDrawBufferAttachmentColor4, GL_COLOR_ATTACHMENT4 },
        { eDrawBufferAttachmentColor5, GL_COLOR_ATTACHMENT5 },
        { eDrawBufferAttachmentColor6, GL_COLOR_ATTACHMENT6 },
        { eDrawBufferAttachmentColor7, GL_COLOR_ATTACHMENT7 },
        { eDrawBufferAttachmentColor8, GL_COLOR_ATTACHMENT8 },
        { eDrawBufferAttachmentColor9, GL_COLOR_ATTACHMENT9 },
        { eDrawBufferAttachmentColor10, GL_COLOR_ATTACHMENT10 },
        { eDrawBufferAttachmentColor11, GL_COLOR_ATTACHMENT11 },
        { eDrawBufferAttachmentColor12, GL_COLOR_ATTACHMENT12 },
        { eDrawBufferAttachmentColor13, GL_COLOR_ATTACHMENT13 },
        { eDrawBufferAttachmentColor14, GL_COLOR_ATTACHMENT14 },
        { eDrawBufferAttachmentColor1, GL_COLOR_ATTACHMENT15 }
#endif
    };
    return drawBufferModes[eDrawBuffer];
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumFilterType eFilter)
{
    static QMap<EnumFilterType, GLenum> filterModes {
        { eNearest, GL_NEAREST },
        { eLinear, GL_LINEAR }
    };
    return filterModes[eFilter];
}

//-------------------------------------------------------------------------------------------------
QByteArray CGLHelper::fromLightParam(int iLightID, EnumLightParameter eParam)
{
    static QMap<EnumLightParameter, QString> lightParameters {
        { eIsEnabled,  "lights[%1].enabled" },
        { ePosition,  "lights[%1].position" },
        { eDirection,  "lights[%1].direction" },
        { eAmbientColor,  "lights[%1].ambient" },
        { eDiffuseColor,  "lights[%1].diffuse" },
        { eSpecularColor,  "lights[%1].specular" },
        { eConstantAttenuation,  "lights[%1].constantAttenuation" },
        { eLinearAttenuation,  "lights[%1].linearAttenuation" },
        { eQuadraticAttenuation,  "lights[%1].quadraticAttenuation" }
    };
    return lightParameters[eParam].arg(iLightID).toLatin1();
}
