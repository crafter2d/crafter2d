
#include "core/modules/scriptmodule.h"
#include "core/modules/modulemanager.h"

#include "script_base.h"

#include "yasscriptmanager.h"

using namespace c2d;

#if defined(WIN32)
  #define DECL cdecl
#else
  #define DECL
#endif

extern "C" SCRIPT_API Modules* DECL getModules()
{  
   ScriptModule* pmodule = new ScriptModule(new YasScriptManager());
   return Modules::create(pmodule);
}

extern "C" SCRIPT_API void DECL freeModules(Modules* pmodules)
{
   ASSERT(pmodules->count == 1);
   ASSERT(pmodules->modules[0]->getKind() == c2d::eScriptModule);
   Modules::free(pmodules);
}