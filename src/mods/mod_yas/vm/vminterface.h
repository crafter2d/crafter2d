
#ifndef VM_INTERFACE_H
#define VM_INTERFACE_H

class ClassRegistry;
class VirtualMachine;
class VirtualCall;

namespace VMInterface
{
   typedef void (*CallbackFnc)(VirtualCall& accessor);

   void registerCommonFunctions(ClassRegistry& registry);
};

#endif // VM_INTERFACE_H
