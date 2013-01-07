
#ifndef SCRIPT_OBJECT_H_
#define SCRIPT_OBJECT_H_

#include <vector>

#include "core/string/string.h"
#include "core/defines.h"

#include "script/vm/virtualmachine.h"
#include "script/common/classregistration.h"
#include "script/common/functionregistration.h"

#include "scriptmanager.h"

class ScriptRegistrator
{
public:

   ScriptRegistrator():
      mRegistry(),
      mpActive(NULL)
   {
   }

 // operations
   void addClass(const String& name)
   {
      mpActive = &mRegistry.addClass(name);
   }

   void addConstructor(const String& prototype, VMInterface::CallbackFnc callback)
   {
      ASSERT_PTR(mpActive);
      mpActive->addFunction(FunctionRegistration::Constructor(prototype, callback));
   }

   void addDestructor(VMInterface::CallbackFnc callback)
   {
      ASSERT_PTR(mpActive);
      mpActive->addFunction(FunctionRegistration::Destructor(callback));
   }

   void addFunction(const String& name, VMInterface::CallbackFnc callback)
   {
      ASSERT_PTR(mpActive);
      mpActive->addFunction(FunctionRegistration::Function(name, callback));
   }

   void registerCallbacks(ScriptManager& manager)
   {
      manager.mVirtualMachine.mergeClassRegistry(mRegistry);
   }

private:
   typedef std::vector<ClassRegistration> Classes;

   ClassRegistry        mRegistry;
   ClassRegistration*   mpActive;
};

#endif // SCRIPT_OBJECT_H_
