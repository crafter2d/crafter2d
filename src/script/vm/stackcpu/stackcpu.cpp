
#include "stackcpu.h"

#include "stackirgenerator.h"

ByteCode::IRGenerator* StackCPU::createIRGenerator()
{
   return new StackIRGenerator();
}
