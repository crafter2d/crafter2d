#include "mod_d3d12.h"

#include "core/modules/graphicsmodule.h"
#include "core/modules/inputmodule.h"
#include "core/modules/modulecollection.h"

#include "d3d11device.h"
#include "input/d3dinput.h"

using namespace c2d;

#if defined(WIN32)
#define DECL cdecl
#else
#define DECL
#endif

extern "C" MOD_API ModuleCollection* cdecl getModuleCollection()
{
   ModuleCollection* pmodules = new ModuleCollection();

   GraphicsModule* pmodule = new GraphicsModule();
   pmodule->setDevice(new Graphics::D3D11Device());
   pmodules->add(pmodule);

   InputModule* pinputmodule = new InputModule;
   pinputmodule->setDevice(new Input::D3DInput());
   pmodules->add(pinputmodule);
   
   return pmodules;
}