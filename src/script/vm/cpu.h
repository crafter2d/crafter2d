
#ifndef CPU_H
#define CPU_H

#include "core/memory/memorypool.h"

#include "script/script_base.h"

namespace ByteCode
{
   class IRGenerator;
   class Program;
}

class VirtualObject;
class VirtualContext;
class VirtualClass;
class VirtualMachine;
class VirtualFunctionTableEntry;

class SCRIPT_API CPU
{
public:
   CPU(VirtualMachine& vm);

 // get/set
   ByteCode::Program& getProgram();

 // query
   virtual ByteCode::IRGenerator* createIRGenerator() = 0;

 // operations
   void initialize(VirtualContext& context);

 // execution
   virtual void execute(VirtualContext& context, const VirtualClass& pclass, const VirtualFunctionTableEntry& entry);

protected:
 // query
   VirtualMachine& getVM();
   VirtualClass& getArrayClass();
   VirtualClass& getStringClass();

 // operations
   VirtualObject* allocObject();

private:

   VirtualMachine&            mVM;
   ByteCode::Program*         mpProgram;
   VirtualClass*              mpArrayClass;
   VirtualClass*              mpStringClass;
   MemoryPool<VirtualObject>  mObjectCache;
};

#endif // CPU_H
