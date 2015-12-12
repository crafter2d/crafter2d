
#include "core/modules/scriptmodule.h"
#include "core/modules/module.h"

#include "script_base.h"

#include "yasscriptmanager.h"

using namespace c2d;

#ifdef WIN32
#define DECL cdecl
#else
#define DECL
#endif

extern "C" SCRIPT_API Module* DECL getModule()
{
   ScriptModule* pmodule = new ScriptModule(new YasScriptManager());
   return pmodule;
}
