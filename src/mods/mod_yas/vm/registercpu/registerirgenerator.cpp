
#include "registerirgenerator.h"

#include "core/defines.h"

#include "mod_yas/bytecode/resolver.h"

#include "../virtualfunction.h"

bool RegisterIRGenerator::virGenerate(VirtualContext& context, VirtualFunction& function)
{
   using namespace CIL;
   
   ByteCode::Resolver resolver(context);

   const CIL::Instructions& instructions = function.getInstructions();
   for ( unsigned index = 0; index < instructions.size(); ++index )
   {
      const CIL::Instruction& inst = instructions[index];

      switch ( inst.opcode )
      {
         case CIL_nop:
         case CIL_last:
         case CIL_dup:
            break;
         
         case CIL_new:
            {
               VirtualFunction& constructor = resolver.resolveFunction(*inst.mString);
            }
            break;
         case CIL_newarray:
            break;

         case CIL_call:
         case CIL_call_native:
         case CIL_call_interface:
         case CIL_call_virt:
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
         case CIL_ldelem:
         case CIL_stelem:
         case CIL_ldstatic:
         case CIL_ststatic:
            break;
            
         case CIL_switch:
         case CIL_instanceof:
         case CIL_throw:
            break;
            
         case CIL_bconv_str:
         case CIL_iconv_real:
         case CIL_iconv_str:
         case CIL_rconv_int:
         case CIL_rconv_str:
         case CIL_cconv_str:
         case CIL_sconv_bool:
         case CIL_sconv_int:
         case CIL_sconv_real:
            break;
      }
   }
   return false;
}