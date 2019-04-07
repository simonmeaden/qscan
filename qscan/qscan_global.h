#ifndef SCAN_GLOBAL_H
#define SCAN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SCAN_LIBRARY)
#  define SCANSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SCANSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SCAN_GLOBAL_H
