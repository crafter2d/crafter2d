
#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include <libcil/cil/cil.h>

#include "block.h"

namespace CIL
{
   class Function;
   class Instructions;
}

class VirtualClass;
class VirtualContext;
class VirtualFunction;
class VirtualGuard;
class VirtualGuards;
class VirtualLookupTable;

namespace ByteCode
{
   class Patch;
   class Resolver;

   class IRGenerator
   {
      typedef std::vector<Patch*> Patches;

   public:
      IRGenerator();
      virtual ~IRGenerator();

    // operations
      bool compile(VirtualContext& context, VirtualFunction& function);

   protected:
    // operations
      virtual bool virGenerate(VirtualContext& context, VirtualFunction& entry) = 0;

      void cleanup();

    // block operations
      bool     hasBlock(int index) const;
      Block*   getBlock(int index);
      Blocks&  getBlocks();

    // patches
      void addPatch(Patch* ppatch);
      void applyPatches(char* pcode);

   private:

    // block operations
      void     buildBlocks(VirtualFunction& entry);
      void     buildGuards(VirtualFunction& entry);
      void     buildGuardBlocks(VirtualGuard& guard);
      void     buildTables(VirtualFunction& entry);
      void     buildTableBlocks(VirtualLookupTable& table);
      void     buildInstructions(const CIL::Instructions& instructions);

      void     allocateInstructionBlocks(int amount);
      Block&   createBlock(int target);

    // data
      Blocks         mBlocks;
      Patches        mPatches;
   };
}

#endif // IR_GENERATOR_H
