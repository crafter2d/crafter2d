
#ifndef STACK_CPU_H
#define STACK_CPU_H

#include "script/vm/cpu.h"

class StackCPU : public CPU
{
public:

   virtual CodeGen::IRGenerator* createIRGenerator();
};

#endif // STACK_CPU_H
