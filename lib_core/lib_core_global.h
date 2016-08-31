#ifndef LIB_CORE_GLOBAL_H
#define LIB_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIB_CORE_LIBRARY)
#  define LIB_CORE_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIB_CORE_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_CORE_GLOBAL_H
