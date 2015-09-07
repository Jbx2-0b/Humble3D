#include "CGLHelper.h"

CGLHelper* CGLHelper::s_pInstance = 0;
QMutex CGLHelper::s_Mutex(QMutex::Recursive);


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
CGLHelper* CGLHelper::getInstance()
{
    if (s_pInstance == 0)
    {
        QMutexLocker locker(&s_Mutex);

        if (s_pInstance == 0)
        {
            s_pInstance = new CGLHelper();
        }
    }

    return s_pInstance;
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
    switch (eType)
    {
    case ePrimitivePoint:
    default:
        return GL_POINTS;
    case ePrimitiveLine:
        return GL_LINES;
    case ePrimitiveLineLoop:
        return GL_LINE_LOOP;
    case ePrimitiveLineStrip:
        return GL_LINE_STRIP;
    case ePrimitiveTriangles:
        return GL_TRIANGLES;
    case ePrimitiveTriangleStrip:
        return GL_TRIANGLE_STRIP;
    case ePrimitiveTriangleFan:
        return GL_TRIANGLE_FAN;
#ifdef DESKTOP_TARGET
    case ePrimitiveLinesAdjacency:
        return GL_LINES_ADJACENCY;
    case ePrimitiveLineStripAdjacency:
        return GL_LINE_STRIP_ADJACENCY;
    case ePrimitiveTrianglesAdjacency:
        return GL_TRIANGLES_ADJACENCY;
    case ePrimitiveTrianglesStripAdjacency:
        return GL_TRIANGLE_STRIP_ADJACENCY;
#endif
    }

    return GL_POINTS; // avoid warning
}

//-------------------------------------------------------------------------------------------------
CGLBuffer::UsagePattern CGLHelper::toQtType(EnumUsagePattern ePattern)
{
    switch (ePattern)
    {
    case eStreamDraw:
        return CGLBuffer::StreamDraw;
    case eStreamRead:
        return CGLBuffer::StreamRead;
    case eStreamCopy:
        return CGLBuffer::StreamCopy;
    case eStaticDraw:
        return CGLBuffer::StaticDraw;
    case eStaticRead:
        return CGLBuffer::StaticRead;
    case eStaticCopy:
        return CGLBuffer::StaticCopy;
    case eDynamicDraw:
        return CGLBuffer::DynamicDraw;
    case eDynamicRead:
        return CGLBuffer::DynamicRead;
    case eDynamicCopy:
        return CGLBuffer::DynamicCopy;
    }

    return CGLBuffer::StreamDraw; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(CDepthTest::EnumDepthTestFunction eFunction)
{
    switch (eFunction)
    {
    case CDepthTest::eNever:
        return GL_NEVER;
    case CDepthTest::eLess:
        return GL_LESS;
    case CDepthTest::eEqual:
        return GL_EQUAL;
    case CDepthTest::eLessThanOrEqual:
        return GL_LEQUAL;
    case CDepthTest::eGreater:
        return GL_GREATER;
    case CDepthTest::eNotEqual:
        return GL_NOTEQUAL;
    case CDepthTest::eGreaterThanOrEqual:
        return GL_GEQUAL;
    case CDepthTest::eAlways:
        return GL_ALWAYS;
    }
    return GL_NEVER; /// avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(Source::EnumBlendingFactor eFactor)
{
    switch (eFactor)
    {
    case Source::eZero:
        return GL_ZERO;
    case Source::eOne:
        return GL_ONE;
    case Source::eSourceAlpha:
        return GL_SRC_ALPHA;
    case Source::eOneMinusSourceAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case Source::eDestinationAlpha:
        return GL_DST_ALPHA;
    case Source::eOneMinusDestinationAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case Source::eDestinationColor:
        return GL_DST_COLOR;
    case Source::eOneMinusDestinationColor:
        return GL_ONE_MINUS_DST_COLOR;
    case Source::eSourceAlphaSaturate:
        return GL_SRC_ALPHA_SATURATE;
    case Source::eConstantColor:
        return GL_CONSTANT_COLOR;
    case Source::eOneMinusConstantColor:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case Source::eConstantAlpha:
        return GL_CONSTANT_ALPHA;
    case Source::eOneMinusConstantAlpha:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    }
    return GL_ONE; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(Destination::EnumBlendingFactor eFactor)
{
    switch (eFactor)
    {
    case Destination::eZero:
        return GL_ZERO;
    case Destination::eOne:
        return GL_ONE;
    case Destination::eSourceColor:
        return GL_SRC_COLOR;
    case Destination::eOneMinusSourceColor:
        return GL_ONE_MINUS_SRC_COLOR;
    case Destination::eSourceAlpha:
        return GL_SRC_ALPHA;
    case Destination::eOneMinusSourceAlpha:
        return GL_ONE_MINUS_SRC_ALPHA;
    case Destination::eDestinationAlpha:
        return GL_DST_ALPHA;
    case Destination::eOneMinusDestinationAlpha:
        return GL_ONE_MINUS_DST_ALPHA;
    case Destination::eDestinationColor:
        return GL_DST_COLOR;
    case Destination::eOneMinusDestinationColor:
        return GL_ONE_MINUS_DST_COLOR;
    case Destination::eConstantColor:
        return GL_CONSTANT_COLOR;
    case Destination::eOneMinusConstantColor:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case Destination::eConstantAlpha:
        return GL_CONSTANT_ALPHA;
    case Destination::eOneMinusConstantAlpha:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    }
    return GL_ZERO; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumBlendEquation eEquation)
{
    switch (eEquation)
    {
    case eAdd:
        return GL_FUNC_ADD;
        break;
    case eMinimum:
        return GL_MIN;
        break;
    case eMaximum:
        return GL_MAX;
        break;
    case eSubtract:
        return GL_FUNC_SUBTRACT;
        break;
    case eReverseSubtract:
        return GL_FUNC_REVERSE_SUBTRACT;
        break;
    }

    return GL_FUNC_ADD; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumCullFace eCullFace)
{
    switch (eCullFace)
    {
    case eFront:
        return GL_FRONT;
        break;
    case eBack:
        return GL_BACK;
        break;
    case eFrontAndBack:
        return GL_FRONT_AND_BACK;
        break;
    }

    return GL_BACK; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumWindingOrder eWindingOrder)
{
    switch (eWindingOrder)
    {
    case eClockwise:
        return GL_CW;
        break;
    case eCounterclockwise:
        return GL_CCW;
        break;
    }

    return GL_CCW; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumStencilOperation eOperation)
{
    switch (eOperation)
    {
    case eZero:
        return GL_ZERO;
    case eInvert:
        return GL_INVERT;
    case eKeep:
        return GL_KEEP;
    case eReplace:
        return GL_REPLACE;
    case eIncrement:
        return GL_INCR;
    case eDecrement:
        return GL_DECR;
    case eIncrementWrap:
        return GL_INCR_WRAP;
    case eDecrementWrap:
        return GL_DECR_WRAP;
    }

    return GL_ZERO; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumStencilTestFunction eFunction)
{
    switch (eFunction)
    {
    case eNever:
        return GL_NEVER;
    case eLess:
        return GL_LESS;
    case eEqual:
        return GL_EQUAL;
    case eLessThanOrEqual:
        return GL_LEQUAL;
    case eGreater:
        return GL_GREATER;
    case eNotEqual:
        return GL_NOTEQUAL;
    case eGreaterThanOrEqual:
        return GL_GEQUAL;
    case eAlways:
        return GL_ALWAYS;
    }

    return GL_ZERO; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumAttachment eAttachment)
{
    switch (eAttachment)
    {
    case eAttachmentDepth:
        return GL_DEPTH_ATTACHMENT;
    case eAttachmentStencil:
        return GL_STENCIL_ATTACHMENT;
    case eAttachmentColor0:
        return GL_COLOR_ATTACHMENT0;

#ifndef EMBEDDED_TARGET
    case eAttachmentColor1:
        return GL_COLOR_ATTACHMENT1;
    case eAttachmentColor2:
        return GL_COLOR_ATTACHMENT2;
    case eAttachmentColor3:
        return GL_COLOR_ATTACHMENT3;
    case eAttachmentColor4:
        return GL_COLOR_ATTACHMENT4;
    case eAttachmentColor5:
        return GL_COLOR_ATTACHMENT5;
    case eAttachmentColor6:
        return GL_COLOR_ATTACHMENT6;
    case eAttachmentColor7:
        return GL_COLOR_ATTACHMENT7;
    case eAttachmentColor8:
        return GL_COLOR_ATTACHMENT8;
    case eAttachmentColor9:
        return GL_COLOR_ATTACHMENT9;
    case eAttachmentColor10:
        return GL_COLOR_ATTACHMENT10;
    case eAttachmentColor11:
        return GL_COLOR_ATTACHMENT11;
    case eAttachmentColor12:
        return GL_COLOR_ATTACHMENT12;
    case eAttachmentColor13:
        return GL_COLOR_ATTACHMENT13;
    case eAttachmentColor14:
        return GL_COLOR_ATTACHMENT14;
    case eAttachmentColor15:
        return GL_COLOR_ATTACHMENT15;
#endif
    default:
        return GL_DEPTH_ATTACHMENT;
    }

    return GL_DEPTH_ATTACHMENT; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumInternalFormat eFormat)
{
    switch (eFormat)
    {
    case eInternalFormatRGB:
        return GL_RGBA;
    case eInternalFormatDepthComponent:
        return GL_DEPTH_COMPONENT;
    case eInternalFormatStencilIndex:
        return GL_STENCIL_INDEX;
    }

    return GL_RGBA; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumDrawBuffer eDrawBuffer)
{
    switch (eDrawBuffer)
    {
    case eDrawBufferNone:
        return GL_NONE;
#ifndef EMBEDDED_TARGET
    case eDrawBufferFrontLeft:
        return GL_FRONT_LEFT;
    case eDrawBufferFrontRight:
        return GL_FRONT_RIGHT;
    case eDrawBufferBackLeft:
        return GL_BACK_LEFT;
    case eDrawBufferBackRight:
        return GL_BACK_RIGHT;
#endif
    case eDrawBufferAttachmentColor0:
        return GL_COLOR_ATTACHMENT0;
#ifndef EMBEDDED_TARGET
    case eDrawBufferAttachmentColor1:
        return GL_COLOR_ATTACHMENT1;
    case eDrawBufferAttachmentColor2:
        return GL_COLOR_ATTACHMENT2;
    case eDrawBufferAttachmentColor3:
        return GL_COLOR_ATTACHMENT3;
    case eDrawBufferAttachmentColor4:
        return GL_COLOR_ATTACHMENT4;
    case eDrawBufferAttachmentColor5:
        return GL_COLOR_ATTACHMENT5;
    case eDrawBufferAttachmentColor6:
        return GL_COLOR_ATTACHMENT6;
    case eDrawBufferAttachmentColor7:
        return GL_COLOR_ATTACHMENT7;
    case eDrawBufferAttachmentColor8:
        return GL_COLOR_ATTACHMENT8;
    case eDrawBufferAttachmentColor9:
        return GL_COLOR_ATTACHMENT9;
    case eDrawBufferAttachmentColor10:
        return GL_COLOR_ATTACHMENT10;
    case eDrawBufferAttachmentColor11:
        return GL_COLOR_ATTACHMENT11;
    case eDrawBufferAttachmentColor12:
        return GL_COLOR_ATTACHMENT12;
    case eDrawBufferAttachmentColor13:
        return GL_COLOR_ATTACHMENT13;
    case eDrawBufferAttachmentColor14:
        return GL_COLOR_ATTACHMENT14;
    case eDrawBufferAttachmentColor15:
        return GL_COLOR_ATTACHMENT15;
#endif
    default:
        return GL_NONE;
    }

    return GL_NONE; // avoid warning
}

//-------------------------------------------------------------------------------------------------
GLenum CGLHelper::toGLType(EnumFilterType eFilter)
{
    switch (eFilter)
    {
    case eNearest:
        return GL_NEAREST;
    case eLinear:
        return GL_LINEAR;
    }

    return GL_LINEAR; // avoid warning
}

//-------------------------------------------------------------------------------------------------
QByteArray CGLHelper::fromLightParam(int iLightID, EnumLightParameter eParam)
{
    switch (eParam)
    {
    case eIsEnabled				: return QString("lights[%1].enabled").arg(iLightID).toLatin1(); break;
    case ePosition				: return QString("lights[%1].position").arg(iLightID).toLatin1(); break;
    case eDirection				: return QString("lights[%1].direction").arg(iLightID).toLatin1(); break;
    case eAmbientColor			: return QString("lights[%1].ambient").arg(iLightID).toLatin1(); break;
    case eDiffuseColor			: return QString("lights[%1].diffuse").arg(iLightID).toLatin1(); break;
    case eSpecularColor			: return QString("lights[%1].specular").arg(iLightID).toLatin1(); break;
    case eConstantAttenuation	: return QString("lights[%1].constantAttenuation").arg(iLightID).toLatin1(); break;
    case eLinearAttenuation		: return QString("lights[%1].linearAttenuation").arg(iLightID).toLatin1(); break;
    case eQuadraticAttenuation	: return QString("lights[%1].quadraticAttenuation").arg(iLightID).toLatin1(); break;
    default:
        return QByteArray();
    }
}
