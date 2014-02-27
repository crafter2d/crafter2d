
#ifndef VIRTUAL_CONTEXT_H_
#define VIRTUAL_CONTEXT_H_

#include "mod_yas/script_base.h"

#include "mod_yas/common/classregistry.h"
#include "mod_yas/common/stringcache.h"

#include "mod_yas/bytecode/program.h"

#include "virtualclasstable.h"

namespace ByteCode
{
   class IRGenerator;
}

class SCRIPT_API VirtualContext
{
public:
   VirtualContext();

   VirtualClass& resolveClass(const String& classname);

   ByteCode::IRGenerator* createIRGenerator();

   VirtualClassTable       mClassTable;
   ClassRegistry           mNativeRegistry;
   StringCache             mStringCache;
   ByteCode::Program       mProgram;

private:

   friend class VirtualMachine;

 // data
   VirtualMachine*         mpVM;
};

#endif // VIRTUAL_CONTEXT_H_
