include(../Global.pri)


TARGET = lib_physics
TEMPLATE = lib

QT += concurrent

DEFINES += LIB_PHYSICS_LIBRARY

SOURCES += \
    CPhysicsManager.cpp

HEADERS +=\
        lib_physics_global.h \
    CPhysicsManager.h \
    CBulletHelper.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bullet/release/ -lbullet
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bullet/debug/ -lbullet
else:unix: LIBS += -L$$OUT_PWD/../bullet/ -lbullet

INCLUDEPATH += $$PWD/../bullet
DEPENDPATH += $$PWD/../bullet

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bullet/release/libbullet.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bullet/debug/libbullet.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bullet/release/bullet.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../bullet/debug/bullet.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../bullet/libbullet.a

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_assets/release/ -llib_assets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_assets/debug/ -llib_assets
else:unix: LIBS += -L$$OUT_PWD/../lib_assets/ -llib_assets

INCLUDEPATH += $$PWD/../lib_assets
DEPENDPATH += $$PWD/../lib_assets
