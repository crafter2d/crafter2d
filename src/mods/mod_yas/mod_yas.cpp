
#include "core/modules/scriptmodule.h"
#include "core/modules/module.h"

#include "script_base.h"

#include "yasscriptmanager.h"

using namespace c2d;

extern "C" SCRIPT_API Module* cdecl getModule()
{
   ScriptModule* pmodule = new ScriptModule(new YasScriptManager());
   return pmodule;
}
