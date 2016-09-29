include(../Global.pri)

QT += concurrent

TARGET = lib_assets
TEMPLATE = lib

DEFINES += LIB_ASSETS_LIBRARY

SOURCES += \
    CCloudsMesh.cpp \
    CDomeMesh.cpp \
    CGrass.cpp \
    CHeightMap.cpp \
    CSkyBox.cpp \
    CWater.cpp \
    CMap.cpp \
    CArrow.cpp \
    CNoiseHeightMap.cpp \
    CSierpinskiTetrahedronFactory.cpp

HEADERS += \
    CCloudsMesh.h \
    CDomeMesh.h \
    CGrass.h \
    CHeightMap.h \
    CSkyBox.h \
    CWater.h \
    lib_assets_global.h \
    CMap.h \
    CMapDictionary.h \
    CSRTMData.h \
    CSRTMLoader.h \
    CArrow.h \
    CNoiseHeightMap.h \
    CSierpinskiTetrahedronFactory.h


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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_core/release/ -llib_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_core/debug/ -llib_core
else:unix: LIBS += -L$$OUT_PWD/../lib_core/ -llib_core

INCLUDEPATH += $$PWD/../lib_core
DEPENDPATH += $$PWD/../lib_core

RESOURCES += \
    resources.qrc

OTHER_FILES +=
