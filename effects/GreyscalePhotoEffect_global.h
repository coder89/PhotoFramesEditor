#ifndef GREYSCALEPHOTOEFFECT_GLOBAL_H
#define GREYSCALEPHOTOEFFECT_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtPlugin>

#if defined(GREYSCALEPHOTOEFFECT_LIBRARY)
#  define GREYSCALEPHOTOEFFECTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GREYSCALEPHOTOEFFECTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GREYSCALEPHOTOEFFECT_GLOBAL_H