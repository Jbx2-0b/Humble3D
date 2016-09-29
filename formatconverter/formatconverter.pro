include(../Global.pri)

QT       += core gui opengl concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = converter
TEMPLATE = app

android {
    DEFINES += EMBEDDED_TARGET
}

SOURCES += main.cpp \
    FormatConverterApp.cpp

HEADERS  += \
    FormatConverterApp.h

FORMS += \
    FormatConverter.ui

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
