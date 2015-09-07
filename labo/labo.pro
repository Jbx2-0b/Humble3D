include(../Global.pri)

QT += core gui widgets opengl xml concurrent

TARGET = labo
TEMPLATE = app

RESOURCES += \
    ressources.qrc

FORMS += \
    CLoadDialog.ui \
    CSaveDialog.ui

HEADERS += \
    CAnimationTreeItem.h \
    CAnimationTreeWidget.h \
    CButtonTreeWidget.h \
    CCodeEditor.h \
    CComboBoxItem.h \
    CEditShaderTextItem.h \
    CEditShaderTreeItem.h \
    CEditShaderTreeWidget.h \
    CLoadDialog.h \
    CNodeTreeWidget.h \
    CParamShaderItem.h \
    CParamShaderWidget.h \
    CSaveDialog.h \
    CTreeWidgetItemBase.h \
    CVectorWidget.h \
    CVectorWidgetItem.h \
    LaboApp.h

SOURCES += \
    CAnimationTreeItem.cpp \
    CAnimationTreeWidget.cpp \
    CButtonTreeWidget.cpp \
    CCodeEditor.cpp \
    CComboBoxItem.cpp \
    CEditShaderTextItem.cpp \
    CEditShaderTreeItem.cpp \
    CEditShaderTreeWidget.cpp \
    CLoadDialog.cpp \
    CNodeTreeWidget.cpp \
    CParamShaderItem.cpp \
    CParamShaderWidget.cpp \
    CSaveDialog.cpp \
    CVectorWidget.cpp \
    CVectorWidgetItem.cpp \
    LaboApp.cpp \
    main.cpp

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_assets/release/ -llib_assets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_assets/debug/ -llib_assets
else:unix: LIBS += -L$$OUT_PWD/../lib_assets/ -llib_assets

INCLUDEPATH += $$PWD/../lib_assets
DEPENDPATH += $$PWD/../lib_assets

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_opengl/release/ -llib_opengl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_opengl/debug/ -llib_opengl
else:unix: LIBS += -L$$OUT_PWD/../lib_opengl/ -llib_opengl

INCLUDEPATH += $$PWD/../lib_opengl
DEPENDPATH += $$PWD/../lib_opengl

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_ui/release/ -llib_ui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_ui/debug/ -llib_ui
else:unix: LIBS += -L$$OUT_PWD/../lib_ui/ -llib_ui

INCLUDEPATH += $$PWD/../lib_ui
DEPENDPATH += $$PWD/../lib_ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../assimp/release/ -lassimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../assimp/debug/ -lassimp
else:unix: LIBS += -L$$OUT_PWD/../assimp/ -lassimp

INCLUDEPATH += $$PWD/../assimp
DEPENDPATH += $$PWD/../assimp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../assimp/release/libassimp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../assimp/debug/libassimp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../assimp/release/assimp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../assimp/debug/assimp.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../assimp/libassimp.a


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_assimp/release/ -llib_assimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_assimp/debug/ -llib_assimp
else:unix: LIBS += -L$$OUT_PWD/../lib_assimp/ -llib_assimp

INCLUDEPATH += $$PWD/../lib_assimp
DEPENDPATH += $$PWD/../lib_assimp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_scripting/release/ -llib_scripting
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_scripting/debug/ -llib_scripting
else:unix: LIBS += -L$$OUT_PWD/../lib_scripting/ -llib_scripting

INCLUDEPATH += $$PWD/../lib_scripting
DEPENDPATH += $$PWD/../lib_scripting

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_xml/release/ -llib_xml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_xml/debug/ -llib_xml
else:unix: LIBS += -L$$OUT_PWD/../lib_xml/ -llib_xml

INCLUDEPATH += $$PWD/../lib_xml
DEPENDPATH += $$PWD/../lib_xml
