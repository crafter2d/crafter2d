
#include "d3d_base.h"

#include <Windows.h>

#include "core/modules/graphicsmodule.h"
#include "core/modules/inputmodule.h"
#include "core/modules/modulemanager.h"

#include "input/dxinputdevice.h"
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

extern "C" MOD_API Modules* DECL getModules()
{
   GraphicsModule* pmodule = new GraphicsModule();
   pmodule->setDevice(new Graphics::D3DDevice());

   InputModule* pinputmodule = new InputModule;
   pinputmodule->setDevice(new Input::DXInputDevice());
   
   return Modules::create({ pmodule, pinputmodule });
}

extern "C" MOD_API void DECL freeModules(Modules* pmodules)
{
   ASSERT(pmodules->count == 2);
   Modules::free(pmodules);
}
