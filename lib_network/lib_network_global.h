﻿#ifndef LIB_NETWORK_GLOBAL_H
#define LIB_NETWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIB_NETWORK_LIBRARY)
#  define LIB_NETWORKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIB_NETWORKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_NETWORK_GLOBAL_H
