
#ifndef CIL_H
#define CIL_H

class String;

namespace CIL
{
   enum TypeName {
      eBool,
      eInt,
      eReal,
      eString,
      eChar,
      eArray,
      eObject,
      eVoid,
      eInvalidType
   };

   enum TypeFlags {
      eIsArray
   };

   enum Opcode {
      CIL_nop,
      CIL_label,
      CIL_new,
      CIL_newarray,
      CIL_newnative,
      CIL_int_to_real,
      CIL_real_to_int,
      CIL_int_to_string,
      CIL_string_to_int,
      CIL_real_to_string,
      CIL_string_to_real,
      CIL_call,
      CIL_call_static,
      CIL_call_super,
      CIL_call_intrf,
      CIL_call_native,
      CIL_ret,
      CIL_add,
      CIL_sub,
      CIL_mul,
      CIL_div,
      CIL_rem,
      CIL_shl,
      CIL_shr,
      CIL_xor,
      CIL_and,
      CIL_or,
      CIL_neg,
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
      CIL_ldcon,
      CIL_ldstr,
      CIL_ldchar,
      CIL_ldreal,
      CIL_ldbool,
      CIL_ldarg,
      CIL_starg,
      CIL_ldloc,
      CIL_stloc,
      CIL_ldelem,
      CIL_stelem,
      CIL_lookup,
      CIL_instanceof,
      CIL_enter_guard,
      CIL_enter_guard_f,
      CIL_leave_guard,
      CIL_throw,
      CIL_push,
      CIL_pop
   };

   struct Type
   {
      TypeName type;
      int      size;
      
      union
      {
         String*  name;
         Type*    elem_type;
      };
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
};

#endif // CIL_H
