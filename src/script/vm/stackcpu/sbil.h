
#ifndef STACK_BASED_CPU_H
#define STACK_BASED_CPU_H

namespace SBIL
{
   #define OPCODE(a,b,c) a,

   enum Opcode {
      #include "sbil.def"

      SBIL_last,
      /*
      SBIL_nop,
      SBIL_dup,

      SBIL_new,
      SBIL_new_array,
      SBIL_new_native,

      SBIL_call,
      SBIL_call_static,
      SBIL_call_native,
      SBIL_ret,

      SBIL_iconv_real,
      SBIL_iconv_str,
      SBIL_rconv_int,
      SBIL_rconv_str,
      SBIL_cconv_str,
      SBIL_bconv_str,
      SBIL_sconv_int,
      SBIL_sconv_real,
      SBIL_sconv_bool,

      SBIL_iadd,
      SBIL_isub,
      SBIL_imul,
      SBIL_idiv,
      SBIL_ineg,
      SBIL_irem,

      SBIL_shl,
      SBIL_shr,
      SBIL_xor,
      SBIL_and,
      SBIL_or,
      SBIL_not,

      SBIL_radd,
      SBIL_rsub,
      SBIL_rmul,
      SBIL_rdiv,
      SBIL_rneg,

      SBIL_cadd,
      SBIL_sadd,

      SBIL_icmpeq,
      SBIL_icmpne,
      SBIL_icmpgt,
      SBIL_icmpge,
      SBIL_icmple,
      SBIL_icmplt,

      SBIL_rcmpeq,
      SBIL_rcmpne,
      SBIL_rcmpgt,
      SBIL_rcmpge,
      SBIL_rcmple,
      SBIL_rcmplt,

      SBIL_scmpeq,
      SBIL_scmpne,
      SBIL_scmpgt,
      SBIL_scmpge,
      SBIL_scmple,
      SBIL_scmplt,

      SBIL_ccmpeq,
      SBIL_ccmpne,
      SBIL_ccmpgt,
      SBIL_ccmpge,
      SBIL_ccmple,
      SBIL_ccmplt,

      SBIL_ocmpeq,
      SBIL_acmpeq,

      SBIL_bcmpeq,
      SBIL_bcmpne,
      SBIL_isnull,

      SBIL_ldfield,
      SBIL_stfield,
      SBIL_ldlocal,
      SBIL_stlocal,
      SBIL_ldelem,
      SBIL_stelem,
      SBIL_ldstatic,
      SBIL_ststatic,

      SBIL_jump,
      SBIL_jump_true,
      SBIL_jump_false,
      
      SBIL_push,
      SBIL_pushi,       // push int
      SBIL_pushc,       // push char
      SBIL_push_i0,
      SBIL_push_i1,
      SBIL_push_i2,
      SBIL_push_r0,
      SBIL_push_r1,
      SBIL_push_r2,
      SBIL_push_true,
      SBIL_push_false,
      SBIL_push_lit,
      SBIL_push_class,
      SBIL_push_null,
      SBIL_pop,

      SBIL_switch,
      SBIL_instanceof,

      SBIL_enterguard,
      SBIL_enterguard_final,
      SBIL_leaveguard,
      SBIL_throw,*/
   };

#undef OPCODE
}

#endif // STACK_BASED_CPU_H
