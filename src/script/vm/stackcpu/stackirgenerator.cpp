
#include "stackirgenerator.h"

#include "script/cil/cil.h"
#include "script/cil/class.h"
#include "script/cil/function.h"

#include "script/common/variant.h"

#include "script/vm/virtualinstruction.h"
#include "script/vm/virtualinstructiontable.h"
#include "script/vm/virtualfunctiontable.h"
#include "script/vm/virtualstring.h"

struct StackClass
{
   // virtual table
   // functions
   VirtualFunctionTable functions;
   // statics
};

struct StackFunction
{
   // instructions
   // arguments
   // locals
};

struct StackObject
{
   // virtual table
   // variables
};

bool StackIRGenerator::generate(const CIL::Class& cilclass)
{
   StackClass sclass;

   const CIL::Class::Functions& functions = cilclass.getFunctions();
   for ( unsigned index = 0; index < functions.size(); ++index )
   {
      const CIL::Function& function = *functions[index];
      generate(function);
   }

   return true;
}

bool StackIRGenerator::generate(const CIL::Function& cilfunction)
{
   using namespace CIL;

   VirtualInstructionTable table;
   std::vector<Variant> symbols;

   const CIL::Instructions& instructions = cilfunction.getInstructions();
   for ( unsigned index = 0; index < instructions.size(); ++index )
   {
      const CIL::Instruction& inst = instructions[index];

      switch ( inst.opcode )
      {
         case CIL_nop:
         case CIL_label:
            break;
         
         case CIL_new:
            const String& function = *inst.mString;
            VirtualFunctionTableEntry* pentry = resolveFunction(function);
            
            table.add(VirtualInstruction(VirtualInstruction::eNew, (int)pentry));
            break;
      }
   }
   return true;
}

VirtualFunctionTableEntry* StackIRGenerator::resolveFunction(const String& name)
{
   int index = name.lastIndexOf('.');
   String classname = name.subStr(0, index - 1);
   String func = name.subStr(index + 1, name.length() - index);

   return NULL;
}
