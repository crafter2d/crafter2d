
#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "script/cil/cil.h"

#include "block.h"

namespace CIL
{
   class Guard;
   class Guards;
   class SwitchTable;
   class SwitchTables;
}

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

      VirtualFunctionTableEntry* generate(CompileContext& context, const ASTFunction& function);

   protected:
    // operations
      virtual bool virGenerate(CompileContext& context, VirtualFunctionTableEntry& entry, const ASTFunction& function) = 0;

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
      void     buildBlocks(CompileContext& context, VirtualFunctionTableEntry& entry, const ASTFunction& function);
      void     buildGuards(VirtualFunctionTableEntry& entry, const CIL::Guards& cilguards);
      void     buildGuardBlocks(VirtualFunctionTableEntry& entry,const CIL::Guard& cilguard);
      void     buildTables(VirtualFunctionTableEntry& entry, const CIL::SwitchTables& tables);
      void     buildTableBlocks(VirtualFunctionTableEntry& entry, const CIL::SwitchTable& table);
      void     buildInstructions(const CIL::Instructions& instructions);

      void     allocateInstructionBlocks(int amount);
      Block&   createBlock(int target);

      Blocks   mBlocks;
      Patches  mPatches;
   };
}

#endif // IR_GENERATOR_H
