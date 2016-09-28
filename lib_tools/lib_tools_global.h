#ifndef LIB_TOOLS_GLOBAL_H
#define LIB_TOOLS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIB_TOOLS_LIBRARY)
# define LIB_TOOLSSHARED_EXPORT Q_DECL_EXPORT
#else
# define LIB_TOOLSSHARED_EXPORT Q_DECL_IMPORT
#endif


#endif // LIB_TOOLS_GLOBAL_H
