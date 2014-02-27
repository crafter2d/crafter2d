
#include "virtualcontext.h"

#include "../bytecode/cpu.h"
#include "../bytecode/generator.h"

#include "virtualclass.h"
#include "virtualmachine.h"

VirtualContext::VirtualContext():
   mpVM(NULL),
   mClassTable(),
   mNativeRegistry(),
   mStringCache()
{
}

// - Operations

VirtualClass& VirtualContext::resolveClass(const String& classname)
{
   ASSERT_PTR(mpVM);

   VirtualClass* pklass = mClassTable.find(classname);
   if ( pklass != NULL )
      return *pklass;

   ByteCode::Generator generator;
   VirtualClass& klass = generator.generate(*this, classname);
   mpVM->classLoaded(klass);

   return klass;
}

ByteCode::IRGenerator* VirtualContext::createIRGenerator()
{
   return mpVM->mpCPU->createIRGenerator();
}


