
#include "d3d_base.h"

#include <Windows.h>

#include "core/modules/graphicsmodule.h"
#include "core/modules/inputmodule.h"
#include "core/modules/modulemanager.h"
#include "core/modules/windowmodule.h"

#include "input/dxinputdevice.h"
#include "window/win32gamewindow.h"
#include "d3ddevice.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dinput8.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

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
   GraphicsModule* pmodule = new GraphicsModule();
   pmodule->setDevice(new Graphics::D3DDevice());

   InputModule* pinputmodule = new InputModule;
   pinputmodule->setDevice(new Input::DXInputDevice());

   WindowModule* pwindowmod = new WindowModule(new Win32GameWindow());
   
   return Modules::create({ pmodule, pinputmodule, pwindowmod });
}

extern "C" MOD_API void DECL freeModules(Modules* pmodules)
{
   ASSERT(pmodules->count == 3);
   Modules::free(pmodules);
}
