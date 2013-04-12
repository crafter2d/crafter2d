
#include "cpu.h"

#include "core/defines.h"

CPU::CPU()
{
}

CodeGen::IRGenerator* CPU::createIRGenerator()
{
   PURE_VIRTUAL;
   return NULL;
}
