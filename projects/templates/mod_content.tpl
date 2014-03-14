
// Generated module file

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "core/{{BASE_HEADER}}.h"

#ifdef WIN32
#ifdef MOD_EXPORTS
#define MOD_API __declspec(dllexport)
#else
#define MOD_API __declspec(dllimport)
#endif
#else
#define MOD_API
#endif

// {{GUID}}
static const Uuid {{NAME}}Uuid();

extern "C" MOD_API Module* cdecl getModule()
{
   {{BASE_CLASS}}* pmodule = new {{BASE_CLASS}}({{NAME}}Uuid);
   {{INIT}}
   pmodule->setReader(new {{NAME}}Reader());
   pmodule->setWriter(new {{NAME}}Writer());
   
   return pmodule;
}
