
#include "d3d_base.h"

#include <Windows.h>

#include "core/graphics/graphicssystem.h"
#include "core/input/inputsystem.h"
#include "core/modules/modulecollection.h"
#include "core/system/systemmodule.h"

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

extern "C" D3D_API ModuleCollection* cdecl getModuleCollection()
{
   ModuleCollection* pmodules = new ModuleCollection();

   c2d::SystemModule* pmodule = new c2d::SystemModule();
   pmodule->setSystem(new c2d::GraphicsSystem(new Graphics::D3DDevice()));
   pmodules->add(pmodule);

   pmodule = new c2d::SystemModule();
   pmodule->setSystem(new c2d::InputSystem(new Input::DXInputDevice()));
   pmodules->add(pmodule);

   return pmodules;
}
