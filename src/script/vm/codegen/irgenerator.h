
#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "script/cil/cil.h"

#include "block.h"

namespace CIL
{
   class Class;
   class Function;
}

namespace CodeGen
{
   class IRContext;

   class IRGenerator
   {
   public:
      IRGenerator();

      bool generate(IRContext& context, const CIL::Class& cilclass);

   protected:
      virtual bool generate(IRContext& context, const CIL::Class& cilclass, const CIL::Function& cilfunction) = 0;

      void buildBlocks(IRContext& context, const CIL::Instructions& instructions);
   };
}

#endif // IR_GENERATOR_H
