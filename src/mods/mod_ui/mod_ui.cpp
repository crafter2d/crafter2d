// mod_effect.cpp : Defines the exported functions for the DLL application.
//

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "core/modules/pluginmodule.h"
#include "core/system/uuid.h"

#include "uipluginmodule.h"

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

// {72A392DA-FF9B-4E88-83B1-B9DCF0CA1042}
static const Uuid UiUuid(0x72A392DA, 0xFF9B, 0x4E88, 0x83B1, 0xB9DCF0CA1042);

extern "C" MOD_API Module* cdecl getModule()
{
   ui::UiPluginModule* pmodule = new ui::UiPluginModule(UiUuid);

   return pmodule;
}
