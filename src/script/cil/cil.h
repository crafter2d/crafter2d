
#ifndef CIL_H
#define CIL_H

#include <vector>

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
      eIsArray    =  1
   };

   enum Visibility {
      ePublic,
      eProtected,
      ePrivate
   };

   enum Flags {
      eStatic     =  1,
      eAbstract   =  2,
      eVirtual    =  4,
      eNative     =  8,
      eFinal      = 16,
      eInterface  = 32
   };

   enum Opcode {
      CIL_nop,
      CIL_label,
      CIL_dup,
      CIL_new,
      CIL_newarray,
      CIL_newnative,
      CIL_conv_int,
      CIL_conv_real,
      CIL_conv_string,
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
      CIL_load,
      CIL_store,
      CIL_ldc,
      CIL_ldstr,
      CIL_ldchar,
      CIL_ldreal,
      CIL_ldbool,
      CIL_ldclass,
      CIL_ldnull,
      CIL_ldarg,
      CIL_starg,
      CIL_ldloc,
      CIL_stloc,
      CIL_ldelem,
      CIL_stelem,
      CIL_ldstatic,
      CIL_ststatic,
      CIL_lookup,
      CIL_instanceof,
      CIL_enter_guard,
      CIL_enter_guard_f,
      CIL_leave_guard,
      CIL_throw
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

   typedef std::vector<Type*> Types;
   typedef std::vector<Instruction> Instructions;
};

#endif // CIL_H
