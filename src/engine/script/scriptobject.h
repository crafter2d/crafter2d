
#ifndef SCRIPT_OBJECT_H_
#define SCRIPT_OBJECT_H_

#include <vector>

#include "core/string/string.h"
#include "core/defines.h"

#include "script/vm/virtualmachine.h"
#include "script/common/functionregistration.h"

#include "scriptmanager.h"

class ScriptRegistrator
{
public:

   ScriptRegistrator():
      mRegistry()
   {
   }

 // operations
   void addClass(const String& name)
   {
      mRegistry.addClass(name);
   }

   void addFunction(const String& prototype, VMInterface::CallbackFnc callback)
   {
      mRegistry.addFunction(prototype, callback);
   }

   void registerCallbacks(ScriptManager& manager)
   {
      manager.mVirtualMachine.mergeClassRegistry(mRegistry);
   }

private:

   ClassRegistry        mRegistry;
};

#endif // SCRIPT_OBJECT_H_
