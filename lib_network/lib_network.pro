include(../Global.pri)


QT       += network

QT       -= gui

TARGET = lib_network
TEMPLATE = lib

DEFINES += LIB_NETWORK_LIBRARY

SOURCES += \
    CTcpClient.cpp \
    CTcpServer.cpp

HEADERS +=\
        lib_network_global.h \
    CTcpClient.h \
    CTcpServer.h \
    ISender.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_tools/release/ -llib_tools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_tools/debug/ -llib_tools
else:unix: LIBS += -L$$OUT_PWD/../lib_tools/ -llib_tools

INCLUDEPATH += $$PWD/../lib_tools
DEPENDPATH += $$PWD/../lib_tools
