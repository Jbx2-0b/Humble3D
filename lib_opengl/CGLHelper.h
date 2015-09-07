#ifndef CGLHELPER_H
#define CGLHELPER_H

// Lib
#include "lib_opengl_global.h"

// Foundations
#include "CGLGlobal.h"
#include "CGeometryGlobal.h"
#include "CMath.h"
#include "CDepthTest.h"
#include "CStencilState.h"

// Qt
#include <QMutex>

typedef QPair<int, EnumLightParameter> TLightParam;

#define pGLHelper CGLHelper::getInstance()

class LIB_OPENGLSHARED_EXPORT CGLHelper
{
public:

    static CGLHelper* getInstance();

    QByteArray getLightParamStr(const TLightParam& param) const;

    const QString& fromType(EnumMaterialParameter eType);

    const QString& fromTypeCount(EnumMaterialParameter eType);

    static GLenum toGLType(EnumPrimitiveType eType);

    static CGLBuffer::UsagePattern toQtType(EnumUsagePattern ePattern);

    static GLenum toGLType(CDepthTest::EnumDepthTestFunction eFunction);

    static GLenum toGLType(Source::EnumBlendingFactor eFactor);

    static GLenum toGLType(Destination::EnumBlendingFactor eFactor);

    static GLenum toGLType(EnumBlendEquation eEquation);

    static GLenum toGLType(EnumCullFace eCullFace);

    static GLenum toGLType(EnumWindingOrder eWindingOrder);

    static GLenum toGLType(EnumStencilOperation eOperation);

    static GLenum toGLType(EnumStencilTestFunction eFunction);

    static GLenum toGLType(EnumAttachment eAttachment);

    static GLenum toGLType(EnumInternalFormat eFormat);

    static GLenum toGLType(EnumDrawBuffer eDrawBuffer);

    static GLenum toGLType(EnumFilterType eFilter);


protected:

    //! Constructeur
    CGLHelper();

    QMap<TLightParam, QByteArray> m_LightParamStr;

    static QByteArray fromLightParam(int iLightID, EnumLightParameter eParam);

    QStringList m_MaterialParameterName;
    QStringList m_MaterialParameterCountName;

    static CGLHelper* s_pInstance;
    static QMutex s_Mutex;
};


#endif // CGLHELPER_H
