include(../Global.pri)

QT       += xml

TARGET = lib_xml
TEMPLATE = lib

DEFINES += LIB_XML

SOURCES += \
    CXmlElement.cpp \
    CXmlWriter.cpp \
    CXmlAttribute.cpp \
    CXmlReader.cpp

HEADERS += \
    CXmlElement.h \
    lib_xml_global.h \
    CXmlAttribute.h \
    CXmlWriter.h \
    CXmlReader.h
