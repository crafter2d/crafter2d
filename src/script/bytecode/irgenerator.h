
#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "script/cil/cil.h"

#include "block.h"

class ASTFunction;
class CompileContext;
class VirtualContext;
class VirtualClass;
class VirtualFunctionTableEntry;

namespace ByteCode
{
   class Program;
   class Patch;

   class IRGenerator
   {
      typedef std::vector<Patch*> Patches;

   public:
      IRGenerator();
      virtual ~IRGenerator();

      int generate(CompileContext& context, const ASTFunction& function);

   protected:
    // operations
      virtual int virGenerate(CompileContext& context, const ASTFunction& function) = 0;

      void cleanup();

    // block operations
      void     buildBlocks(CompileContext& context, const CIL::Instructions& instructions);
      bool     hasBlock(int index) const;
      Block*   getBlock(int index);
      Blocks&  getBlocks();

    // patches
      void addPatch(Patch* ppatch);
      void applyPatches(char* pcode);

   private:

    // block operations
      void     allocateInstructionBlocks(int amount);
      Block&   createBlock(int target);

      Blocks   mBlocks;
      Patches  mPatches;
   };
}

#endif // IR_GENERATOR_H
