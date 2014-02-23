
#include "scriptmodule.h"

#include "core/defines.h"

namespace c2d
{
   ScriptModule::ScriptModule(ScriptManager* pmanager) :
      Module(eScriptModule, UUID_ScriptModule),
      mpManager(pmanager)
   {
      ASSERT_PTR(mpManager);
   }

   ScriptManager& ScriptModule::getManager()
   {
      return *mpManager;
   }
}
