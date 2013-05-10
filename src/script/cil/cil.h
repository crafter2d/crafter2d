
#ifndef CIL_H
#define CIL_H

#include <vector>

class String;

namespace CIL
{
   enum Opcode {
      CIL_nop,
      CIL_dup,
      CIL_new,
      CIL_newarray,
      CIL_bconv_str,
      CIL_iconv_real,
      CIL_iconv_str,
      CIL_rconv_int,
      CIL_rconv_str,
      CIL_cconv_str,
      CIL_sconv_bool,
      CIL_sconv_int,
      CIL_sconv_real,
      CIL_call,
      CIL_call_virt,
      CIL_call_interface,
      CIL_call_static,
      CIL_call_native,
      CIL_ret,
      CIL_add,
      CIL_sub,
      CIL_mul,
      CIL_div,
      CIL_neg,
      CIL_rem,
      CIL_shl,
      CIL_shr,
      CIL_xor,
      CIL_or,
      CIL_and,
      CIL_not,
      CIL_cmpeq,
      CIL_cmpne,
      CIL_cmpgt,
      CIL_cmpge,
      CIL_cmple,
      CIL_cmplt,
      CIL_isnull,
      CIL_jump,
      CIL_jump_true,
      CIL_jump_false,
      CIL_ldint,
      CIL_ldstr,
      CIL_ldchar,
      CIL_ldreal,
      CIL_ldtrue,
      CIL_ldfalse,
      CIL_ldclass,
      CIL_ldnull,
      CIL_ldfield,
      CIL_stfield,
      CIL_ldarg,
      CIL_starg,
      CIL_ldloc,
      CIL_stloc,
      CIL_ldelem,
      CIL_stelem,
      CIL_ldstatic,
      CIL_ststatic,
      CIL_switch,
      CIL_instanceof,
      CIL_throw
   };

   struct Instruction
   {
      CIL::Opcode opcode;

      union
      {
         int mInt;
         double mReal;
         String* mString;
         void* mPtr;
      };
   };

   typedef std::vector<Instruction> Instructions;
};

#endif // CIL_H
