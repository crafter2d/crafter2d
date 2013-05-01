
#include "cpu.h"

#include "core/defines.h"

#include "script/vm/virtualcontext.h"
#include "script/vm/virtualobject.h"
#include "script/bytecode/program.h"

CPU::CPU(VirtualMachine& vm):
   mVM(vm),
   mpProgram(NULL),
   mpArrayClass(NULL),
   mpStringClass(NULL),
   mObjectCache()
{
   mpProgram = new ByteCode::Program();
}

ByteCode::Program& CPU::getProgram()
{
   return *mpProgram;
}

// - Query

VirtualMachine& CPU::getVM()
{
   return mVM;
}

VirtualClass& CPU::getArrayClass()
{
   ASSERT_PTR(mpArrayClass);
   return *mpArrayClass;
}

VirtualClass& CPU::getStringClass()
{
   ASSERT_PTR(mpStringClass);
   return *mpStringClass;
}

ByteCode::IRGenerator* CPU::createIRGenerator()
{
   PURE_VIRTUAL;
   return NULL;
}

// - Operations

void CPU::initialize(VirtualContext& context)
{
   mpArrayClass = &context.mClassTable.resolve("system.InternalArray");
   mpStringClass = &context.mClassTable.resolve("system.InternalString");
}

VirtualObject* CPU::allocObject()
{
   return mObjectCache.alloc();
}
