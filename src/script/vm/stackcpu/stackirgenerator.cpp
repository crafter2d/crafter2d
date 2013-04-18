
#include "stackirgenerator.h"

#include <vector>

#include "core/defines.h"

#include "script/cil/cil.h"
#include "script/cil/class.h"
#include "script/cil/function.h"
#include "script/cil/resolver.h"

#include "script/common/variant.h"

#include "script/ast/astfunction.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/virtualinstructiontable.h"
#include "script/vm/virtualfunctiontableentry.h"
#include "script/vm/virtualstring.h"
#include "script/vm/codegen/block.h"

struct Inst
{
   SBIL::Opcode   opcode;
   SBIL::Type     type;
   int            arg;
};

typedef std::vector<Inst> Insts;

#define INSERT(opc,a,t)      \
   do {                      \
      Inst ins;              \
      ins.opcode = opc;      \
      ins.type = t;          \
      ins.arg = a;           \
      insts.push_back(ins);  \
   } while (false);

struct FunctionSymbol
{
   int args;
};

StackIRGenerator::StackIRGenerator():
   CodeGen::IRGenerator()
{
}

char* StackIRGenerator::generate(CompileContext& context, const ASTFunction& function)
{
   using namespace CIL;
   using namespace SBIL;
   using namespace CodeGen;

   Insts insts;
   std::vector<Variant> symbols;
   int ip = 0;

   VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();

   const CIL::Instructions& instructions = cilfunction.getInstructions();
   buildBlocks(context, instructions);

   for ( unsigned index = 0; index < instructions.size(); ++index )
   {
      const CIL::Instruction& inst = instructions[index];

      switch ( inst.opcode )
      {
         case CIL_nop:
         case CIL_label:
            // should not get here
            break;

         case CIL_dup:
            INSERT(SBIL_dup, 0, insts[insts.size() - 1].type);
            break;
         
         case CIL_new:
            {
               Function* pcall = resolveFunction(context, *inst.mString);
               INSERT(SBIL_new, (int)pcall, SBIL_object);
            }
            break;
         case CIL_newarray:
            {
               INSERT(SBIL_new_array, inst.mInt, SBIL_array);
            }
            break;

         // Calls

         case CIL_call:
            {
               Function* pfunction = resolveFunction(context, *inst.mString);
               FunctionSymbol* psymbol = new FunctionSymbol;
               psymbol->args = pfunction->getSignature().size();

               SBIL::Type type = typeToSBIL(pfunction->getReturnType());
               INSERT(SBIL_call, (int)pfunction, type);
            }
            break;
         case CIL_call_native:
            {
               const FunctionRegistration& funcreg = context.pclassregistry->getFunction(inst.mInt);
               SBIL::Type type = typeToSBIL(funcreg.getReturnType());
               INSERT(SBIL_call_native, inst.mInt, type);
            }
            break;
         case CIL_call_static:
            {
               Function* pfunction = resolveFunction(context, *inst.mString);
               SBIL::Type type = typeToSBIL(pfunction->getReturnType());
               INSERT(SBIL_call_static, 0, type);
            }
            break;
         case CIL_ret:
            INSERT(SBIL_ret, inst.mInt, SBIL_null);
            break;

         // Math

         case CIL_add:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_int:
                     INSERT(SBIL_iadd, 0, SBIL_int);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_radd, 0, SBIL_real);
                     break;
                  case SBIL_string:
                     INSERT(SBIL_sadd, 0, SBIL_string);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_sub:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_int:
                     INSERT(SBIL_isub, 0, SBIL_int);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rsub, 0, SBIL_real);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_mul:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_int:
                     INSERT(SBIL_imul, 0, SBIL_int);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rmul, 0, SBIL_real);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_div:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_int:
                     INSERT(SBIL_idiv, 0, SBIL_int);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rdiv, 0, SBIL_real);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_neg:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_int:
                     INSERT(SBIL_ineg, 0, SBIL_int);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rneg, 0, SBIL_real);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_rem:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               ASSERT(type == SBIL_int);
               INSERT(SBIL_irem, 0, SBIL_int);
            }
            break;
         case CIL_shl:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               ASSERT(type == SBIL_int);
               INSERT(SBIL_shl, 0, SBIL_int);
            }
            break;
         case CIL_shr:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               ASSERT(type == SBIL_int);
               INSERT(SBIL_shr, 0, SBIL_int);
            }
            break;

         case CIL_xor:
            INSERT(SBIL_xor, 0, SBIL_int);
            break;
         case CIL_and:
            INSERT(SBIL_and, 0, SBIL_int);
            break;
         case CIL_or:
            INSERT(SBIL_or, 0, SBIL_int);
            break;
         case CIL_not:
            INSERT(SBIL_not, 0, SBIL_bool);
            break;

         case CIL_cmpeq:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_bool:
                     INSERT(SBIL_bcmpeq, 0, SBIL_bool);
                     break;
                  case SBIL_int:
                     INSERT(SBIL_icmpeq, 0, SBIL_bool);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rcmpeq, 0, SBIL_bool);
                     break;
                  case SBIL_char:
                     INSERT(SBIL_ccmpeq, 0, SBIL_bool);
                     break;
                  case SBIL_string:
                     INSERT(SBIL_scmpeq, 0, SBIL_bool);
                     break;
                  case SBIL_object:
                     INSERT(SBIL_ocmpeq, 0, SBIL_bool);
                     break;
                  case SBIL_array:
                     INSERT(SBIL_acmpeq, 0, SBIL_bool);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
            }
            break;
         case CIL_cmpne:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_bool:
                     INSERT(SBIL_bcmpne, 0, SBIL_bool);
                     break;
                  case SBIL_int:
                     INSERT(SBIL_icmpne, 0, SBIL_bool);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rcmpne, 0, SBIL_bool);
                     break;
                  case SBIL_char:
                     INSERT(SBIL_ccmpne, 0, SBIL_bool);
                     break;
                  case SBIL_string:
                     INSERT(SBIL_scmpne, 0, SBIL_bool);
                     break;
                  case SBIL_object:
                     INSERT(SBIL_ocmpeq, 0, SBIL_bool);
                     INSERT(SBIL_not, 0, SBIL_bool);
                     break;
                  case SBIL_array:
                     INSERT(SBIL_acmpeq, 0, SBIL_bool);
                     INSERT(SBIL_not, 0, SBIL_bool);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
            }
            break;
         case CIL_cmpgt:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_int:
                     INSERT(SBIL_icmpgt, 0, SBIL_bool);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rcmpgt, 0, SBIL_bool);
                     break;
                  case SBIL_char:
                     INSERT(SBIL_ccmpgt, 0, SBIL_bool);
                     break;
                  case SBIL_string:
                     INSERT(SBIL_scmpgt, 0, SBIL_bool);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
            }
            break;
         case CIL_cmpge:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_int:
                     INSERT(SBIL_icmpge, 0, SBIL_bool);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rcmpge, 0, SBIL_bool);
                     break;
                  case SBIL_char:
                     INSERT(SBIL_ccmpge, 0, SBIL_bool);
                     break;
                  case SBIL_string:
                     INSERT(SBIL_scmpge, 0, SBIL_bool);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
            }
            break;
         case CIL_cmple:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_int:
                     INSERT(SBIL_icmple, 0, SBIL_bool);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rcmple, 0, SBIL_bool);
                     break;
                  case SBIL_char:
                     INSERT(SBIL_ccmple, 0, SBIL_bool);
                     break;
                  case SBIL_string:
                     INSERT(SBIL_scmple, 0, SBIL_bool);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
            }
            break;
         case CIL_cmplt:
            {
               SBIL::Type type = insts[insts.size() - 1].type;
               switch ( type )
               {
                  case SBIL_int:
                     INSERT(SBIL_icmplt, 0, SBIL_bool);
                     break;
                  case SBIL_real:
                     INSERT(SBIL_rcmplt, 0, SBIL_bool);
                     break;
                  case SBIL_char:
                     INSERT(SBIL_ccmplt, 0, SBIL_bool);
                     break;
                  case SBIL_string:
                     INSERT(SBIL_scmplt, 0, SBIL_bool);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
            }
            break;
         case CIL_isnull:
            INSERT(SBIL_isnull, 0, SBIL_bool);
            break;

         case CIL_jump:
            {
               // compute address to jump to
               int addr = 0; 
               INSERT(SBIL_jump, addr, SBIL_null);
            }
            break;
         case CIL_jump_true:
            {
               // compute addr to jump to
               int addr = 0; 
               INSERT(SBIL_jump_true, addr, SBIL_null);
            }
            break;
         case CIL_jump_false:
            {
               // compute addr to jump to
               int addr = 0; 
               INSERT(SBIL_jump_false, addr, SBIL_null);
            }
            break;

         case CIL_ldint:
            if ( inst.mInt == 0 )
            {
               INSERT(SBIL_push_i0, 0, SBIL_int);
            }
            else if ( inst.mInt == 1 )
            {
               INSERT(SBIL_push_i1, 0, SBIL_int);
            }
            else if ( inst.mInt == 2 )
            {
               INSERT(SBIL_push_i2, 0, SBIL_int);
            }
            else
            {
               INSERT(SBIL_pushi, inst.mInt, SBIL_int);
            }
            break;
         case CIL_ldreal:
            if ( inst.mReal == 0.0 )
            {
               INSERT(SBIL_push_r0, 0, SBIL_int);
            }
            else if ( inst.mReal == 1.0 )
            {
               INSERT(SBIL_push_r1, 0, SBIL_int);
            }
            else if ( inst.mReal == 2.0 )
            {
               INSERT(SBIL_push_r2, 0, SBIL_int);
            }
            else
            {
               INSERT(SBIL_push, 0, SBIL_real);
            }
            break;
         case CIL_ldchar:
            INSERT(SBIL_pushc, inst.mInt, SBIL_char);
            break;
         case CIL_ldstr:
            {
               // push in symbol table (string cache)
               int loc = 0; // replace with actual index
               INSERT(SBIL_push, loc, SBIL_string);
            }
            break;
         case CIL_ldtrue:
            INSERT(SBIL_push_true, 0, SBIL_bool);
            break;
         case CIL_ldfalse:
            INSERT(SBIL_push_false, 0, SBIL_bool);
            break;
         case CIL_ldclass:
            INSERT(SBIL_push_class, 0, SBIL_object);
            break;
         case CIL_ldnull:
            INSERT(SBIL_push_null, 0, SBIL_null);
            break;

         case CIL_ldfield:
            {
               const CIL::Type* ptype = cilclass.getFields()[inst.mInt];
               SBIL::Type type = typeToSBIL(*ptype);
               INSERT(SBIL_ldfield, inst.mInt, type);
            }
            break;
         case CIL_stfield:
            INSERT(SBIL_stfield, inst.mInt, SBIL_null);
            break;
         case CIL_ldarg:
            {
               const CIL::Type& ciltype = function.getSignature()[inst.mInt];
               SBIL::Type type = typeToSBIL(ciltype);
               INSERT(SBIL_ldlocal, inst.mInt, type);
            }
            break;
         case CIL_starg:
            INSERT(SBIL_stlocal, inst.mInt, SBIL_null);
            break;
         case CIL_ldloc:
            {
               const CIL::Type* ptype = cilfunction.getLocals()[inst.mInt];
               SBIL::Type type = typeToSBIL(*ptype);
               INSERT(SBIL_ldlocal, inst.mInt + cilfunction.getSignature().size(), type);
            }
         case CIL_stloc:
            INSERT(SBIL_stlocal, inst.mInt + cilfunction.getSignature().size(), SBIL_null);
            break;
         case CIL_ldelem_bool:
            INSERT(SBIL_ldelem, inst.mInt, SBIL_bool);
            break;
         case CIL_ldelem_int:
            INSERT(SBIL_ldelem, inst.mInt, SBIL_int);
            break;
         case CIL_ldelem_real:
            INSERT(SBIL_ldelem, inst.mInt, SBIL_real);
            break;
         case CIL_ldelem_char:
            INSERT(SBIL_ldelem, inst.mInt, SBIL_char);
            break;
         case CIL_ldelem_string:
            INSERT(SBIL_ldelem, inst.mInt, SBIL_string);
            break;
         case CIL_ldelem_object:
            INSERT(SBIL_ldelem, inst.mInt, SBIL_object);
            break;
         case CIL_ldelem_array:
            INSERT(SBIL_ldelem, inst.mInt, SBIL_array);
            break;
         case CIL_stelem:
            INSERT(SBIL_stelem, inst.mInt, SBIL_null);
            break;
         case CIL_ldstatic:
            {
               const CIL::Type* ptype = cilclass.getStaticFields()[index];
               SBIL::Type type = typeToSBIL(*ptype);
               INSERT(SBIL_ldstatic, inst.mInt, type);
            }
            break;
         case CIL_ststatic:
            INSERT(SBIL_ststatic, inst.mInt, SBIL_null);
            break;
      }
   }

   return pentry;
}

CIL::Function* StackIRGenerator::resolveFunction(VirtualContext& context, const String& name)
{
   CIL::Resolver resolver;
   CIL::Function* pfunction = resolver.resolveFunction(context.mCilClasses, name);
   return pfunction;
}

SBIL::Type StackIRGenerator::typeToSBIL(const CIL::Type& type)
{
   switch ( type.type )
   {
      case CIL::eBool:
         return SBIL::SBIL_bool;
      case CIL::eInt:
         return SBIL::SBIL_int;
      case CIL::eReal:
         return SBIL::SBIL_real;
      case CIL::eChar:
         return SBIL::SBIL_char;
      case CIL::eString:
         return SBIL::SBIL_string;
      case CIL::eObject:
         return SBIL::SBIL_object;
      case CIL::eArray:
         return SBIL::SBIL_array;
      case CIL::eVoid:
         return SBIL::SBIL_null;
   }
   return SBIL::SBIL_invalid;
}

SBIL::Type StackIRGenerator::typeToSBIL(FunctionRegistration::Type type)
{
   switch ( type )
   {
      case FunctionRegistration::eBool:
         return SBIL::SBIL_bool;
      case FunctionRegistration::eInt:
         return SBIL::SBIL_int;
      case FunctionRegistration::eReal:
         return SBIL::SBIL_real;
      case FunctionRegistration::eChar:
         return SBIL::SBIL_char;
      case FunctionRegistration::eString:
         return SBIL::SBIL_string;
   }
   return SBIL::SBIL_invalid;
}
