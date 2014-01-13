
#ifndef VM_INTERFACE_H
#define VM_INTERFACE_H

#include "script/script_base.h"

class ClassRegistry;
class VirtualMachine;
class VirtualCall;

namespace VMInterface
{
   typedef void (*CallbackFnc)(VirtualCall& accessor);

   SCRIPT_API void registerCommonFunctions(ClassRegistry& registry);
};

#endif // VM_INTERFACE_H
