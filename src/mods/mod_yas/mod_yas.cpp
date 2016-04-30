
#include "core/modules/scriptmodule.h"
#include "core/modules/modulemanager.h"
#include "core/modules/moduleinfo.h"

#include "script_base.h"

#include "yasscriptmanager.h"

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
   ScriptModule* pmodule = new ScriptModule(new YasScriptManager());
   return Modules::create(pmodule);
}

extern "C" MOD_API void DECL freeModules(Modules* pmodules)
{
   ASSERT(pmodules->count == 1);
   ASSERT(pmodules->modules[0]->getKind() == c2d::eScriptModule);
   Modules::free(pmodules);
}