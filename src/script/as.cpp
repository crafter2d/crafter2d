// olang.cpp : Defines the entry point for the console application.
//

#include "as.h"

#include "vm/virtualmachine.h"

void SCRIPT_API runScript()
{
   VirtualMachine vm;
   vm.execute("Army", "executeTests");
}

