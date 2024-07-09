#ifndef SLICE2D_GLOBAL_H
#define SLICE2D_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SLICE2D_LIBRARY)
#  define SLICE2DSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SLICE2DSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SLICE2D_GLOBAL_H
