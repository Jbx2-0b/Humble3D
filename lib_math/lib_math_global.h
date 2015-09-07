#ifndef LIB_MATH_GLOBAL_H
#define LIB_MATH_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIB_MATH_LIBRARY)
# define LIB_MATHSHARED_EXPORT Q_DECL_EXPORT
#else
# define LIB_MATHSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_MATH_GLOBAL_H
