#ifndef MOD_UI_BASE_H
#define MOD_UI_BASE_H

#ifdef WIN32
#  ifdef MOD_EXPORTS
#    define MOD_API __declspec(dllexport)
#  else
#    define MOD_API __declspec(dllimport)
#  endif
#  define MOD_DECL cdecl
#else
#  define MOD_API
#  define MOD_DECL
#endif

#endif // MOD_UI_BASE_H
