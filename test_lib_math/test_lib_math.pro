include(../Global.pri)


QT       += core xml concurrent

TARGET = test_lib_math
TEMPLATE = app

SOURCES += main.cpp

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_xml/release/ -llib_xml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_xml/debug/ -llib_xml
else:unix: LIBS += -L$$OUT_PWD/../lib_xml/ -llib_xml

INCLUDEPATH += $$PWD/../lib_xml
DEPENDPATH += $$PWD/../lib_xml
