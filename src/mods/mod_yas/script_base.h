
#ifndef SCRIPT_BASE_H_
#define SCRIPT_BASE_H_

#if defined(WIN32)
  #define DECL cdecl
  #ifdef SCRIPT_EXPORTS
    #define MOD_API __declspec(dllexport)
  #else
    #define MOD_API __declspec(dllimport)
  #endif
#else
  #define DECL
  #define MOD_API
#endif

#endif // SCRIPT_BASE_H_
