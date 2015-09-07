#ifndef LIB_ASSETS_GLOBAL_H
#define LIB_ASSETS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIB_ASSETS_LIBRARY)
#  define LIB_ASSETS_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIB_ASSETS_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_ASSETS_GLOBAL_H
