
#ifndef BYTECODE_JUMP_PATCH_H
#define BYTECODE_JUMP_PATCH_H

#include "codepatch.h"

namespace ByteCode
{
   class Block;

   class JumpPatch : public Patch
   {
   public:
      int pos;
      Block* to;

      virtual void apply(char* pcode);
   };
}

#endif // BYTECODE_JUMP_PATCH_H
