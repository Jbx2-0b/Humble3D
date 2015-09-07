include(../Global.pri)

QT       += core gui opengl widgets concurrent

TARGET = test_transformation
TEMPLATE = app

SOURCES += main.cpp \
    TestTransformation.cpp

HEADERS  += \
    TestTransformation.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_ui/release/ -llib_ui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_ui/debug/ -llib_ui
else:unix: LIBS += -L$$OUT_PWD/../lib_ui/ -llib_ui

INCLUDEPATH += $$PWD/../lib_ui
DEPENDPATH += $$PWD/../lib_ui


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_assets/release/ -llib_assets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_assets/debug/ -llib_assets
else:unix: LIBS += -L$$OUT_PWD/../lib_assets/ -llib_assets

INCLUDEPATH += $$PWD/../lib_assets
DEPENDPATH += $$PWD/../lib_assets

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

RESOURCES += \
    Resources.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/Humble3D/Test_Transformation
INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat





