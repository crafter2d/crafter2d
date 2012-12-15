
#include "ogl_base.h"

#include <Windows.h>

#include "ogldevicefactory.h"

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

extern "C" OGL_API Graphics::DeviceFactory* cdecl getDeviceFactory()
{
   return new Graphics::OGLDeviceFactory();
}
