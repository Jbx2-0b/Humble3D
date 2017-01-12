include(../Global.pri)


QT += widgets opengl concurrent quick

TARGET = lib_qt_ui
TEMPLATE = lib

DEFINES += LIB_QT_UI_LIBRARY

SOURCES += \
    AView.cpp \
    CExclusiveButton.cpp \
    CSoftwareRenderer.cpp \
    CToolBar.cpp \
    CGraphicsView3D.cpp \
    CWidget3D.cpp \
    CAnimatedPanelDecorator.cpp \
    CSoftwareView3D.cpp \
    Humble3DApp.cpp \
    CGraphicsScene3D.cpp \
    CQmlWidget3D.cpp

HEADERS += \
    AView.h \
    CExclusiveButton.h \
    CFPSManager.h \
    CGraphicsWidgetItem.h \
    CKeyboardStates.h \
    CMouseStates.h \
    CQtHelper.h \
    CRasterizer.h \
    CSoftwareRenderer.h \
    CToolBar.h \
    CGraphicsView3D.h \
    CWidget3D.h \
    CAnimatedPanelDecorator.h \
    CSoftwareView3D.h \
    Humble3DApp.h \
    CGraphicsScene3D.h \
    lib_qt_ui_global.h \
    CQmlWidget3D.h

RESOURCES += \
    shaders.qrc

unix {
    target.path = /usr/lib
    INSTALLS += target
}

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_opengl/release/ -llib_opengl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_opengl/debug/ -llib_opengl
else:unix: LIBS += -L$$OUT_PWD/../lib_opengl/ -llib_opengl

INCLUDEPATH += $$PWD/../lib_opengl
DEPENDPATH += $$PWD/../lib_opengl

