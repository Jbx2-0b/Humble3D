#ifndef LIB_SCRIPTING_GLOBAL_H
#define LIB_SCRIPTING_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIB_SCRIPTING_LIBRARY)
#  define LIB_SCRIPTINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIB_SCRIPTINGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_SCRIPTING_GLOBAL_H
