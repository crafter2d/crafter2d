
#include "cpu.h"

#include "core/defines.h"

CPU::CPU()
{
}

ByteCode::IRGenerator* CPU::createIRGenerator()
{
   PURE_VIRTUAL;
   return NULL;
}
