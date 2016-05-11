#include "mod_d3d12.h"

#include "core/defines.h"
#include "core/modules/graphicsmodule.h"
#include "core/modules/inputmodule.h"
#include "core/modules/modulemanager.h"
#include "core/modules/moduleinfo.h"

#include "d3d11device.h"
#include "input/d3dinput.h"

using namespace c2d;

extern "C" MOD_API ModuleInfo* MOD_DECL getModuleInfo()
{
   return ModuleInfo::create(1, { });
}

extern "C" MOD_API void MOD_DECL freeModuleInfo(ModuleInfo* pinfo)
{
   ModuleInfo::free(pinfo);
}

extern "C" MOD_API Modules* MOD_DECL getModules()
{
   GraphicsModule* pgraphicsmodule = new GraphicsModule();
   pgraphicsmodule->setDevice(new Graphics::D3D11Device());

   InputModule* pinputmodule = new InputModule;
   pinputmodule->setDevice(new Input::D3DInput());
   
   return Modules::create({ pgraphicsmodule, pinputmodule });
}

extern "C" MOD_API void MOD_DECL freeModules(Modules* pmodules)
{
   ASSERT(pmodules->count == 1);
   ASSERT(pmodules->modules[0]->getKind() == c2d::eUiModule);
   Modules::free(pmodules);
}
