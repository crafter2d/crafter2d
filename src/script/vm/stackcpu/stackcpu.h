
#ifndef STACK_CPU_H
#define STACK_CPU_H

#include "script/vm/cpu.h"

class SCRIPT_API StackCPU : public CPU
{
public:

   virtual ByteCode::IRGenerator* createIRGenerator();
};

#endif // STACK_CPU_H
