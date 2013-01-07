
#ifndef VM_INTERFACE_H
#define VM_INTERFACE_H

class VirtualMachine;
class VirtualStackAccessor;

namespace VMInterface
{
   typedef void (*CallbackFnc)(VirtualMachine& machine, VirtualStackAccessor& accessor);

   void registerCommonFunctions(VirtualMachine& machine);
};

#endif // VM_INTERFACE_H
