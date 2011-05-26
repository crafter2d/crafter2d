
#ifndef SCRIPT_OBJECT_H_
#define SCRIPT_OBJECT_H_

#include <string>
#include <vector>

#include "script/vm/virtualmachine.h"

#include "scriptmanager.h"

class ScriptRegistrator
{
public:

   ScriptRegistrator(): mCallbacks() {}

 // operations

   void addCallback(const std::string& name, VirtualMachine::callbackfnc callback) {
      mNames.push_back(name);
      mCallbacks.push_back(callback);
   }

   void registerCallbacks(ScriptManager& manager) {
      VirtualMachine& vm = *manager.mpVirtualMachine;

      for ( int index = 0; index < mNames.size(); index++ )
      {
         vm.registerCallback(mNames[index], mCallbacks[index]);
      }
   }

private:
   std::vector<std::string> mNames;
   std::vector<VirtualMachine::callbackfnc> mCallbacks;
};

#endif // SCRIPT_OBJECT_H_
