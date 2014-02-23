
#ifndef VIRTUAL_CONTEXT_H_
#define VIRTUAL_CONTEXT_H_

#include "mod_yas/script_base.h"

#include "mod_yas/common/classregistry.h"
#include "mod_yas/common/stringcache.h"

#include "mod_yas/bytecode/program.h"

#include "virtualclasstable.h"

class SCRIPT_API VirtualContext
{
public:
   VirtualContext();

   VirtualClassTable       mClassTable;
   ClassRegistry           mNativeRegistry;
   StringCache             mStringCache;
   ByteCode::Program       mProgram;
};

#endif // VIRTUAL_CONTEXT_H_
