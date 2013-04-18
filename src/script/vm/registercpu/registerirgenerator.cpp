
#include "registerirgenerator.h"

#include "script/cil/class.h"
#include "script/cil/function.h"

bool RegisterIRGenerator::generate(const CIL::Class& cilclass)
{
   return true;
}

void RegisterIRGenerator::generate(const CIL::Function& cilfunction)
{
   using namespace CIL;

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
            {
            }
            break;
         case CIL_newarray:
            break;

         case CIL_call:
         case CIL_call_native:
         case CIL_call_static:
         case CIL_ret:
            break;

         case CIL_add:
         case CIL_sub:
         case CIL_mul:
         case CIL_div:
         case CIL_rem:
         case CIL_shl:
         case CIL_shr:
            break;

         case CIL_xor:
         case CIL_and:
         case CIL_or:
         case CIL_neg:
         case CIL_not:
            break;

         case CIL_cmpeq:
         case CIL_cmpne:
         case CIL_cmpgt:
         case CIL_cmpge:
         case CIL_cmple:
         case CIL_cmplt:
         case CIL_isnull:
            break;

         case CIL_jump:
         case CIL_jump_true:
         case CIL_jump_false:
            break;

         case CIL_ldint:
         case CIL_ldreal:
         case CIL_ldchar:
         case CIL_ldstr:
         case CIL_ldtrue:
         case CIL_ldfalse:
         case CIL_ldclass:
         case CIL_ldnull:
            break;

         case CIL_ldfield:
         case CIL_stfield:
         case CIL_ldarg:
            break;
         case CIL_starg:
            break;
         case CIL_ldloc:
         case CIL_stloc:
         case CIL_ldelem_bool:
            break;
         case CIL_ldelem_int:
            break;
         case CIL_ldelem_real:
            break;
         case CIL_ldelem_char:
            break;
         case CIL_ldelem_string:
            break;
         case CIL_ldelem_object:
            break;
         case CIL_ldelem_array:
            break;
         case CIL_stelem:
         case CIL_ldstatic:
         case CIL_ststatic:
            break;
      }
   }
}