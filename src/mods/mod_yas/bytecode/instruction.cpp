
#include "instruction.h"

#include "core/defines.h"

namespace ByteCode
{
   Instruction::Instruction():
      next(nullptr),
      inst(0)
   {
   }
}
