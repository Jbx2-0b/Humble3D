#ifndef LIB_XML_GLOBAL_H
#define LIB_XML_GLOBAL_H

#include <qglobal.h>

#if defined(LIB_XML)
#  define LIB_XML_EXPORT Q_DECL_EXPORT
#else
#  define LIB_XML_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_XML_GLOBAL_H
