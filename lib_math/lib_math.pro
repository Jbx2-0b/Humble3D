include(../Global.pri)

TARGET = lib_math
TEMPLATE = lib

DEFINES += LIB_MATH_LIBRARY

SOURCES += \
    CNoiseGenerator.cpp \
    CBox3D.cpp \
    CMath.cpp \
    CImageProcessing.cpp \
    CSphere.cpp \
    CCylinder.cpp

HEADERS +=\
    lib_math_global.h \
    CEdge.h \
    CLine.h \
    CNoiseGenerator.h \
    CPlane.h \
    CRay.h \
    CSpan.h \
    CTriangle.h \
    CBox3D.h \
    CBox2D.h \
    CMath.h \
    CImageProcessing.h \
    CCylinder.h \
    CSphere.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
