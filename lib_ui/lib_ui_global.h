#ifndef LIB_UI_H
#define LIB_UI_H

#include <QtCore/qglobal.h>

#if defined(LIB_UI_LIBRARY)
#  define LIB_UI_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIB_UI_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_UI_H
