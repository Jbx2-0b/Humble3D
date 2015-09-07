include(../Global.pri)


QT       += core network widgets

TARGET = test_tcp_client
TEMPLATE = app


SOURCES += main.cpp \
    CTcpClientTest.cpp

HEADERS += \
    CTcpClientTest.h

CONFIG += mobility
MOBILITY =




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib_network/release/ -llib_network
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib_network/debug/ -llib_network
else:unix: LIBS += -L$$OUT_PWD/../lib_network/ -llib_network

INCLUDEPATH += $$PWD/../lib_network
DEPENDPATH += $$PWD/../lib_network
