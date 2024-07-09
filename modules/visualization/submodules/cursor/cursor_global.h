#ifndef CURSOR_GLOBAL_H
#define CURSOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CURSOR_LIBRARY)
#  define CURSORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CURSORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CURSOR_GLOBAL_H
