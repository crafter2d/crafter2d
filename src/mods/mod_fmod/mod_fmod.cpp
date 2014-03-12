// mod_effect.cpp : Defines the exported functions for the DLL application.
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "core/modules/modulecollection.h"
#include "core/modules/soundmodule.h"
#include "fmodsoundmanager.h"

#ifdef WIN32
#ifdef MOD_EXPORTS
#define MOD_API __declspec(dllexport)
#else
#define MOD_API __declspec(dllimport)
#endif
#else
#define MOD_API
#endif

using namespace c2d;

extern "C" MOD_API Module* cdecl getModule()
{
   ModuleCollection* pmodules = new ModuleCollection();

   SoundModule* pmod = new SoundModule(new FModSoundManager());
   
   return pmod;
}
