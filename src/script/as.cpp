// olang.cpp : Defines the entry point for the console application.
//

#include "as.h"

#include "vm/virtualcontext.h"
#include "vm/virtualmachine.h"

void SCRIPT_API runScript()
{
   VirtualContext context;
   VirtualMachine vm(context);
   vm.execute("Army", "executeTests");
}

