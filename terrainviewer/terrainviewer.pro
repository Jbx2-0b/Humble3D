#-------------------------------------------------
#
# Project created by QtCreator 2013-11-21T15:47:35
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = labo
TEMPLATE = app

win32-g++
{
    QMAKE_CXXFLAGS += -Wno-unused-function -Wno-literal-suffix
}

SOURCES += main.cpp \
    TerrainViewerApp.cpp

HEADERS  += \
    TerrainViewerApp.h

RESOURCES += \
    ressources.qrc


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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_geometry/release/ -llib_geometry
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_geometry/debug/ -llib_geometry
else:unix: LIBS += -L$$OUT_PWD/../lib_geometry/ -llib_geometry

INCLUDEPATH += $$PWD/../lib_geometry
DEPENDPATH += $$PWD/../lib_geometry


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_qt_opengl/release/ -llib_qt_opengl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_qt_opengl/debug/ -llib_qt_opengl
else:unix: LIBS += -L$$OUT_PWD/../lib_qt_opengl/ -llib_qt_opengl

INCLUDEPATH += $$PWD/../lib_qt_opengl
DEPENDPATH += $$PWD/../lib_qt_opengl

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_loaderjbx/release/ -llib_loaderjbx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_loaderjbx/debug/ -llib_loaderjbx
else:unix: LIBS += -L$$OUT_PWD/../lib_loaderjbx/ -llib_loaderjbx

INCLUDEPATH += $$PWD/../lib_loaderjbx
DEPENDPATH += $$PWD/../lib_loaderjbx

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_carto/release/ -llib_carto
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_carto/debug/ -llib_carto
else:unix: LIBS += -L$$OUT_PWD/../lib_carto/ -llib_carto

INCLUDEPATH += $$PWD/../lib_carto
DEPENDPATH += $$PWD/../lib_carto
