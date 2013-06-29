
#include "d3d_base.h"

#include <Windows.h>

#include "d3ddevicefactory.h"

#pragma comment(lib, "d3d11.lib")

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

extern "C" D3D_API Graphics::DeviceFactory* cdecl getDeviceFactory()
{
   return new Graphics::D3DDeviceFactory();
}
