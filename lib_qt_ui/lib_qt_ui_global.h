#ifndef LIB_QT_UI_H
#define LIB_QT_UI_H

#include <QtCore/qglobal.h>

#if defined(LIB_QT_UI_LIBRARY)
#  define LIB_QT_UI_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIB_QT_UI_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_QT_UI_H
