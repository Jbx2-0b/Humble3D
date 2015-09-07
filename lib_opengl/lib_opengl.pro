include(../Global.pri)

TARGET = lib_opengl
TEMPLATE = lib

QT       += widgets opengl concurrent

DEFINES += LIB_OPENGL_LIBRARY

HEADERS += \
    AGLTexture.h \
    CGLGlobal.h \
    CGLMeshBuffer.h \
    CGLRenderer.h \
    CGLTexture2D.h \
    CGLTextureCube.h \
    CGLTextureTarget.h \
    EnumAccess.h \
    EnumBufferType.h \
    lib_opengl_global.h \
    CGLFrameBuffer.h \
    CGLContext.h \
    CGLHelper.h

SOURCES += \
    AGLTexture.cpp \
    CGLMeshBuffer.cpp \
    CGLRenderer.cpp \
    CGLTexture2D.cpp \
    CGLTextureCube.cpp \
    CGLTextureTarget.cpp \
    CGLContext.cpp \
    CGLHelper.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_core/release/ -llib_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_core/debug/ -llib_core
else:unix: LIBS += -L$$OUT_PWD/../lib_core/ -llib_core

INCLUDEPATH += $$PWD/../lib_core
DEPENDPATH += $$PWD/../lib_core



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_math/release/ -llib_math
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_math/debug/ -llib_math
else:unix: LIBS += -L$$OUT_PWD/../lib_math/ -llib_math

INCLUDEPATH += $$PWD/../lib_math
DEPENDPATH += $$PWD/../lib_math



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_tools/release/ -llib_tools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_tools/debug/ -llib_tools
else:unix: LIBS += -L$$OUT_PWD/../lib_tools/ -llib_tools

INCLUDEPATH += $$PWD/../lib_tools
DEPENDPATH += $$PWD/../lib_tools
