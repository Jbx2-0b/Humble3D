#ifndef LIB_QML_UI_H
#define LIB_QML_UI_H

#include <QtCore/qglobal.h>

#if defined(LIB_QML_UI_LIBRARY)
#  define LIB_QML_UI_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIB_QML_UI_SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_QML_UI_H
