
#ifndef GLASS_EXPORT_H
#define GLASS_EXPORT_H

#ifdef GLASS_STATIC_DEFINE
#  define GLASS_EXPORT
#  define GLASS_NO_EXPORT
#else
#  ifndef GLASS_EXPORT
#    ifdef Glass_EXPORTS
        /* We are building this library */
#      define GLASS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define GLASS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef GLASS_NO_EXPORT
#    define GLASS_NO_EXPORT 
#  endif
#endif

#ifndef GLASS_DEPRECATED
#  define GLASS_DEPRECATED __declspec(deprecated)
#endif

#ifndef GLASS_DEPRECATED_EXPORT
#  define GLASS_DEPRECATED_EXPORT GLASS_EXPORT GLASS_DEPRECATED
#endif

#ifndef GLASS_DEPRECATED_NO_EXPORT
#  define GLASS_DEPRECATED_NO_EXPORT GLASS_NO_EXPORT GLASS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef GLASS_NO_DEPRECATED
#    define GLASS_NO_DEPRECATED
#  endif
#endif

#endif /* GLASS_EXPORT_H */
