// mod_effect.cpp : Defines the exported functions for the DLL application.
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "core/defines.h"
#include "core/modules/modulemanager.h"
#include "core/modules/soundmodule.h"

#include "fmodsoundmanager.h"

#ifdef WIN32
#define DECL cdecl
#ifdef MOD_EXPORTS
#define MOD_API __declspec(dllexport)
#else
#define MOD_API __declspec(dllimport)
#define DECL cdecl
#endif
#else
#define MOD_API
#define DECL
#endif

using namespace c2d;

extern "C" MOD_API ModuleInfo* DECL getModuleInfo()
{
   return ModuleInfo::create(1, {});
}

extern "C" MOD_API void DECL freeModuleInfo(ModuleInfo* pinfo)
{
   ModuleInfo::free(pinfo);
}

extern "C" MOD_API Modules* DECL getModules()
{
   SoundModule* pmod = new SoundModule(new FModSoundManager());
   return Modules::create(pmod);
}

extern "C" MOD_API void DECL freeModules(Modules* pmodules)
{
   ASSERT(pmodules->count == 1);
   ASSERT(pmodules->modules[0]->getKind() == c2d::eSoundModule);
   Modules::free(pmodules);
}
