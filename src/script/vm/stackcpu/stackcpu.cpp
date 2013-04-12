
#include "stackcpu.h"

#include "stackirgenerator.h"

CodeGen::IRGenerator* StackCPU::createIRGenerator()
{
   return new StackIRGenerator();
}