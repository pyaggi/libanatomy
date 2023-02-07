#ifndef QUDEFS_H
#define QUDEFS_H
#ifndef QUILL_DECL_PUBLIC
#  if defined(_WIN32)
#    define QUILL_DECL_PUBLIC __declspec(dllexport)
#    pragma warning (disable:4251)
#  elif __GNUC__ >= 4
#    define QUILL_DECL_PUBLIC __attribute__((visibility("default")))
#    define Q_DECL_HIDDEN __attribute__((visibility("hidden")))
#  endif
#  ifndef QUILL_DECL_PUBLIC
#    define QUILL_DECL_PUBLIC
#  endif
#endif

#ifndef QUILL_IMPORT
#  if defined(_WIN32)
#    define QUILL_IMPORT __declspec(dllimport)
#  else
#    define QUILL_IMPORT
#  endif
#endif

#endif // QUDEFS_H
