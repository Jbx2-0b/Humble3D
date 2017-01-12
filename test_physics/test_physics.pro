include(../Global.pri)


QT       += core gui widgets opengl concurrent

TARGET = test_physics
TEMPLATE = app

SOURCES += main.cpp \
    TestPhysics.cpp

HEADERS  += \
    TestPhysics.h

RESOURCES += \
    resources.qrc

#models.path = $$OUT_PWD
#models.files = $$PWD/resources/*.3ds

#INSTALLS += models

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_qt_ui/release/ -llib_qt_ui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_qt_ui/debug/ -llib_qt_ui
else:unix: LIBS += -L$$OUT_PWD/../lib_qt_ui/ -llib_qt_ui

INCLUDEPATH += $$PWD/../lib_qt_ui
DEPENDPATH += $$PWD/../lib_qt_ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bullet/release/ -lbullet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bullet/debug/ -lbullet
else:unix: LIBS += -L$$OUT_PWD/../bullet/ -lbullet

INCLUDEPATH += $$PWD/../bullet
DEPENDPATH += $$PWD/../bullet

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_physics/release/ -llib_physics
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_physics/debug/ -llib_physics
else:unix: LIBS += -L$$OUT_PWD/../lib_physics/ -llib_physics

INCLUDEPATH += $$PWD/../lib_physics
DEPENDPATH += $$PWD/../lib_physics


win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bullet/release/libbullet.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bullet/debug/libbullet.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bullet/release/bullet.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bullet/debug/bullet.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../bullet/libbullet.a


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_assets/release/ -llib_assets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_assets/debug/ -llib_assets
else:unix: LIBS += -L$$OUT_PWD/../lib_assets/ -llib_assets

INCLUDEPATH += $$PWD/../lib_assets
DEPENDPATH += $$PWD/../lib_assets


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
