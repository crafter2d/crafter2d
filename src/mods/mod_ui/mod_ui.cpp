// mod_effect.cpp : Defines the exported functions for the DLL application.
//

#include "ui_base.h"

#include "core/defines.h"
#include "core/modules/moduleinfo.h"
#include "core/modules/uimodule.h"
#include "core/modules/modulemanager.h"

#include "yuisystem.h"

using namespace c2d;

extern "C" MOD_API ModuleInfo* MOD_DECL getModuleInfo()
{
   return ModuleInfo::create(1, { UUID_ScriptModule });
}

extern "C" MOD_API void MOD_DECL freeModuleInfo(ModuleInfo* pinfo)
{
   ModuleInfo::free(pinfo);
}

extern "C" MOD_API Modules* MOD_DECL getModules()
{
   auto pmodule = new UiModule(new YuiSystem());
   return Modules::create(pmodule);
}

extern "C" MOD_API void MOD_DECL freeModules(Modules* pmodules)
{
   ASSERT(pmodules->count == 1);
   ASSERT(pmodules->modules[0]->getKind() == c2d::eUiModule);
   Modules::free(pmodules);
}
