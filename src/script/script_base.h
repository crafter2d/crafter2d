
#ifndef SCRIPT_BASE_H_
#define SCRIPT_BASE_H_

#ifdef WIN32
#ifdef SCRIPT_EXPORTS
#  define SCRIPT_API __declspec(dllexport)
#  define SCRIPT_EXTERN
#else
#  define SCRIPT_API __declspec(dllimport)
#  define SCRIPT_EXTERN extern
#endif
#else
#  define SCRIPT_API
#endif

#endif // SCRIPT_BASE_H_
