#ifndef OCR_GLOBAL_H
#define OCR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OCR_LIBRARY)
#  define OCRSHARED_EXPORT Q_DECL_EXPORT
#else
#  define OCRSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // OCR_GLOBAL_H
