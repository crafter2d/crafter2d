
#ifndef VM_INTERFACE_H
#define VM_INTERFACE_H

#include "script/script_base.h"

class ClassRegistry;
class VirtualMachine;
class VirtualStackAccessor;

namespace VMInterface
{
   typedef void (*CallbackFnc)(VirtualMachine& machine, VirtualStackAccessor& accessor);

   SCRIPT_API void registerCommonFunctions(ClassRegistry& registry);
};

#endif // VM_INTERFACE_H
