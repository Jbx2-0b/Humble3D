include(../Global.pri)


TARGET = lib_tools
TEMPLATE = lib

DEFINES += LIB_TOOLS_LIBRARY

SOURCES += \
    CConfigFileManager.cpp \
    CLogManager.cpp \
    CResourcesPathManager.cpp \
    CStringHelper.cpp \
    CFilterEvent.cpp \
    AWashable.cpp \
    ALockable.cpp

HEADERS +=\
        lib_tools_global.h \
    CConfigFileManager.h \
    CLogManager.h \
    CRegisterFactory.h \
    CResourcesPathManager.h \
    CStringHelper.h \
    CTreeExplorer.h \
    CArray.h \
    CArray2D.h \
    CArray3D.h \
    CFilterEvent.h \
    ALockable.h \
    AWashable.h \
    CScriptableFactory.h \
    IScriptable.h \
    CBinaryLoader.h
