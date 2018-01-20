
#include "stackirgenerator.h"

#include <vector>
#include <stack>
#include <memory>
#include <string.h>

#include "core/defines.h"

#include "mod_yas/bytecode/block.h"
#include "mod_yas/bytecode/program.h"
#include "mod_yas/bytecode/symboltable.h"
#include "mod_yas/bytecode/functionsymbol.h"
#include "mod_yas/bytecode/nativefunctionsymbol.h"
#include "mod_yas/bytecode/resolver.h"
#include "mod_yas/bytecode/valuesymbol.h"
#include "mod_yas/bytecode/jumppatch.h"
#include "mod_yas/bytecode/block.h"
#include "mod_yas/bytecode/instruction.h"

#include "mod_yas/cil/cil.h"
#include "mod_yas/cil/instructions.h"

#include "mod_yas/common/type.h"
#include "mod_yas/common/classregistry.h"

#include "mod_yas/vm/virtualclass.h"
#include "mod_yas/vm/virtualcontext.h"
#include "mod_yas/vm/virtualfield.h"
#include "mod_yas/vm/virtualguard.h"
#include "mod_yas/vm/virtualstring.h"
#include "mod_yas/vm/virtualfunction.h"
#include "mod_yas/vm/virtuallookuptable.h"
#include "mod_yas/vm/virtualvalue.h"

struct OpcodeInfo
{
   int push;
   int pop;
};

#define OPCODE(a,b,c) { b, c },
const OpcodeInfo stackinfo[SBIL::SBIL_last + 1] = {
#include "sbil.def"
};

StackIRGenerator::StackIRGenerator():
   ByteCode::IRGenerator()
{
}

bool StackIRGenerator::virGenerate(VirtualContext& context, VirtualFunction& entry)
{
   generateInstructions(context, entry);
   checkAndFixStack(context, entry);

   return buildCode(context, entry);
}

void StackIRGenerator::generateInstructions(VirtualContext& context, const VirtualFunction& function)
{
   using namespace CIL;
   using namespace SBIL;
   using namespace ByteCode;
   using namespace yasc;

   Resolver resolver(context);
   std::deque<Type> types;

   const CIL::Instructions& instructions = function.getInstructions();

   Block* pblock = nullptr;

   for ( unsigned index = 0; index < instructions.size(); ++index )
   {
      const CIL::Instruction& inst = instructions[index];

      if ( hasBlock(index) )
         pblock = getBlock(index);
      ASSERT_PTR(pblock);

      switch ( inst.opcode )
      {
         case CIL_nop:
            // should not get here
            break;

         case CIL_dup:
            {
               types.emplace_back(types.back());
               INSERT(SBIL_dup, 0);
            }
            break;
         
         case CIL_new:
            {
               VirtualFunction& constructor = resolver.resolveFunction(*inst.mString);
               ASSERT(constructor.getIndex() >= 0);

               FunctionSymbol* psymbol = new FunctionSymbol();
               psymbol->klass = constructor.getClass().getName();
               psymbol->func = constructor.getIndex();
               psymbol->args = constructor.getArguments().size();   // object is created in this instruction
               psymbol->returns = !constructor.getReturnType().isVoid();
               int i = context.mProgram.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < psymbol->args; ++arg )
                  types.pop_back();

               types.push_back(Type::fromString(constructor.getClass().getName()));

               INSERT(SBIL_new, i);
            }
            break;
         case CIL_newarray:
            {
               Type type = Type::fromString(*inst.mString);
               ASSERT(type.isArray());

               int depth = type.getArrayDimension();
               for ( int arg = 0; arg < depth; ++arg )
                  types.pop_back();

               types.push_back(type);

               INSERT(SBIL_new_array, depth);
            }
            break;

         // Calls

         case CIL_call:
            {
               VirtualFunction& func = resolver.resolveFunction(*inst.mString);
               ASSERT(func.getIndex() >= 0);

               FunctionSymbol* psymbol = new FunctionSymbol();
               psymbol->klass = func.getClass().getName();
               psymbol->func = func.getIndex();
               psymbol->args = func.getArgumentCount();
               psymbol->returns = !func.getReturnType().isVoid();
               int i = context.mProgram.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < psymbol->args; ++arg )
                  types.pop_back();

               if ( psymbol->returns )
                  types.push_back(func.getReturnType());

               INSERT(SBIL_call, i);
            }
            break;
         case CIL_call_virt:
            {
               VirtualFunction& func = resolver.resolveFunction(*inst.mString);
               ASSERT(func.getIndex() >= 0);

               FunctionSymbol* psymbol = new FunctionSymbol();
               psymbol->klass = func.getClass().getName();
               psymbol->func = func.getIndex();
               psymbol->args = func.getArgumentCount();
               psymbol->returns = !func.getReturnType().isVoid();
               int i = context.mProgram.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < psymbol->args; ++arg )
                  types.pop_back();

               if ( psymbol->returns )
                  types.push_back(func.getReturnType());

               INSERT(SBIL_call_virt, i);
            }
            break;
         case CIL_call_interface:
            {
               VirtualFunction& func = resolver.resolveFunction(*inst.mString);

               FunctionSymbol* psymbol = new FunctionSymbol();
               psymbol->func = func.getIndex();
               psymbol->args = func.getArgumentCount();
               psymbol->returns = !func.getReturnType().isVoid();
               int i = context.mProgram.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < psymbol->args; ++arg )
                  types.pop_back();

               if ( psymbol->returns )
                  types.push_back(func.getReturnType());

               INSERT(SBIL_call_interface, i);
            }
            break;
         case CIL_call_native:
            {
               VirtualFunction& func = resolver.resolveFunction(*inst.mString);

               auto pfuncreg = context.mNativeRegistry.findCallback(*inst.mString);
               ASSERT_PTR(pfuncreg);

               NativeFunctionSymbol* psymbol = new NativeFunctionSymbol;
               psymbol->pnativefunction = pfuncreg;
               psymbol->args = func.getArgumentCount();
               psymbol->returns = !func.getReturnType().isVoid();
               int i = context.mProgram.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < psymbol->args; ++arg )
                  types.pop_back();

               if ( psymbol->returns )
                  types.push_back(func.getReturnType());

               INSERT(SBIL_call_native, i);
            }
            break;
         case CIL_ret:
            INSERT(SBIL_ret, inst.mInt);
            break;

         // Conversion

         case CIL_bconv_str:
            INSERT(SBIL_bconv_str, 0);
            types.pop_back();
            types.emplace_back(Type::eString);
            break;
         case CIL_iconv_real:
            INSERT(SBIL_iconv_real, 0);
            types.pop_back();
            types.emplace_back(Type::eReal);
            break;
         case CIL_iconv_str:
            INSERT(SBIL_iconv_str, 0);
            types.pop_back();
            types.emplace_back(Type::eString);
            break;
         case CIL_rconv_int:
            INSERT(SBIL_rconv_int, 0);
            types.pop_back();
            types.emplace_back(Type::eInt);
            break;
         case CIL_rconv_str:
            INSERT(SBIL_rconv_str, 0);
            types.pop_back();
            types.emplace_back(Type::eString);
            break;
         case CIL_cconv_str:
            INSERT(SBIL_cconv_str, 0);
            types.pop_back();
            types.emplace_back(Type::eString);
            break;
         case CIL_sconv_bool:
            INSERT(SBIL_sconv_bool, 0);
            types.pop_back();
            types.emplace_back(Type::eBool);
            break;
         case CIL_sconv_int:
            INSERT(SBIL_sconv_int, 0);
            types.pop_back();
            types.emplace_back(Type::eInt);
            break;
         case CIL_sconv_real:
            INSERT(SBIL_sconv_real, 0);
            types.pop_back();
            types.emplace_back(Type::eReal);
            break;

         // Math

         case CIL_add:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eInt:
                     INSERT(SBIL_iadd, 0);
                     types.emplace_back(Type::eInt);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_radd, 0);
                     types.emplace_back(Type::eReal);
                     break;
                  case Type::eString:
                     INSERT(SBIL_sadd, 0);
                     types.emplace_back(Type::eString);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_sub:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eInt:
                     INSERT(SBIL_isub, 0);
                     types.emplace_back(Type::eInt);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rsub, 0);
                     types.emplace_back(Type::eReal);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_mul:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eInt:
                     INSERT(SBIL_imul, 0);
                     types.emplace_back(Type::eInt);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rmul, 0);
                     types.emplace_back(Type::eReal);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_div:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eInt:
                     INSERT(SBIL_idiv, 0);
                     types.emplace_back(Type::eInt);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rdiv, 0);
                     types.emplace_back(Type::eReal);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_neg:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eInt:
                     INSERT(SBIL_ineg, 0);
                     types.emplace_back(Type::eInt);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rneg, 0);
                     types.emplace_back(Type::eReal);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_rem:
            INSERT(SBIL_irem, 0);
            types.emplace_back(Type::eInt);
            break;
         case CIL_shl:
            INSERT(SBIL_shl, 0);
            types.emplace_back(Type::eInt);
            break;
         case CIL_shr:
            INSERT(SBIL_shr, 0);
            types.emplace_back(Type::eInt);
            break;

         case CIL_xor:
            INSERT(SBIL_xor, 0);
            break;
         case CIL_and:
            INSERT(SBIL_and, 0);
            break;
         case CIL_or:
            INSERT(SBIL_or, 0);
            break;
         case CIL_not:
            INSERT(SBIL_not, 0); // push & pop a boolean
            break;

         case CIL_cmpeq:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eBool:
                     INSERT(SBIL_bcmpeq, 0);
                     break;
                  case Type::eInt:
                     INSERT(SBIL_icmpeq, 0);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rcmpeq, 0);
                     break;
                  case Type::eChar:
                     INSERT(SBIL_ccmpeq, 0);
                     break;
                  case Type::eString:
                     INSERT(SBIL_scmpeq, 0);
                     break;
                  case Type::eGeneric:
                  case Type::eObject:
                     INSERT(SBIL_ocmpeq, 0);
                     break;
                  case Type::eArray:
                     INSERT(SBIL_acmpeq, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }

               types.emplace_back(Type::eBool);
            }
            break;
         case CIL_cmpne:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eBool:
                     INSERT(SBIL_bcmpne, 0);
                     break;
                  case Type::eInt:
                     INSERT(SBIL_icmpne, 0);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rcmpne, 0);
                     break;
                  case Type::eChar:
                     INSERT(SBIL_ccmpne, 0);
                     break;
                  case Type::eString:
                     INSERT(SBIL_scmpne, 0);
                     break;
                  case Type::eGeneric:
                  case Type::eObject:
                     INSERT(SBIL_ocmpeq, 0);
                     INSERT(SBIL_not, 0);
                     break;
                  case Type::eArray:
                     INSERT(SBIL_acmpeq, 0);
                     INSERT(SBIL_not, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.emplace_back(Type::eBool);
            }
            break;
         case CIL_cmpgt:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eInt:
                     INSERT(SBIL_icmpgt, 0);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rcmpgt, 0);
                     break;
                  case Type::eChar:
                     INSERT(SBIL_ccmpgt, 0);
                     break;
                  case Type::eString:
                     INSERT(SBIL_scmpgt, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.emplace_back(Type::eBool);
            }
            break;
         case CIL_cmpge:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eInt:
                     INSERT(SBIL_icmpge, 0);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rcmpge, 0);
                     break;
                  case Type::eChar:
                     INSERT(SBIL_ccmpge, 0);
                     break;
                  case Type::eString:
                     INSERT(SBIL_scmpge, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.emplace_back(Type::eBool);
            }
            break;
         case CIL_cmple:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eInt:
                     INSERT(SBIL_icmple, 0);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rcmple, 0);
                     break;
                  case Type::eChar:
                     INSERT(SBIL_ccmple, 0);
                     break;
                  case Type::eString:
                     INSERT(SBIL_scmple, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.emplace_back(Type::eBool);
            }
            break;
         case CIL_cmplt:
            {
               Type::Kind kind = types.back().getKind();
               types.pop_back();
               types.pop_back();

               switch ( kind )
               {
                  case Type::eInt:
                     INSERT(SBIL_icmplt, 0);
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rcmplt, 0);
                     break;
                  case Type::eChar:
                     INSERT(SBIL_ccmplt, 0);
                     break;
                  case Type::eString:
                     INSERT(SBIL_scmplt, 0);
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               }
               types.emplace_back(Type::eBool);
            }
            break;
         case CIL_isnull:
            INSERT(SBIL_isnull, 0);
            types.emplace_back(Type::eBool);
            break;

         case CIL_jump:
            INSERT(SBIL_jump, 0);
            break;
         case CIL_jump_true:
            ASSERT(types.back().isBool());
            INSERT(SBIL_jump_true, 0);
            types.pop_back();
            break;
         case CIL_jump_false:
            ASSERT(types.back().isBool());
            INSERT(SBIL_jump_false, 0);
            types.pop_back();
            break;

         case CIL_ldint:
            if ( inst.mInt == 0 )
            {
               INSERT(SBIL_push_i0, 0);
            }
            else if ( inst.mInt == 1 )
            {
               INSERT(SBIL_push_i1, 0);
            }
            else if ( inst.mInt == 2 )
            {
               INSERT(SBIL_push_i2, 0);
            }
            else
            {
               INSERT(SBIL_pushi, inst.mInt);
            }
            types.emplace_back(Type::eInt);
            break;
         case CIL_ldreal:
            if ( inst.mReal == 0.0 )
            {
               INSERT(SBIL_push_r0, 0);
            }
            else if ( inst.mReal == 1.0 )
            {
               INSERT(SBIL_push_r1, 0);
            }
            else if ( inst.mReal == 2.0 )
            {
               INSERT(SBIL_push_r2, 0);
            }
            else
            {
               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setReal(inst.mReal);
               int i = context.mProgram.getSymbolTable().add(psymbol);

               INSERT(SBIL_push, i);
            }
            types.emplace_back(Type::eReal);
            break;
         case CIL_ldchar:
            INSERT(SBIL_pushc, inst.mInt);
            types.emplace_back(Type::eChar);
            break;
         case CIL_ldstr:
            {
               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setString(context.mProgram.getStringCache().lookup(*inst.mString));
               int i = context.mProgram.getSymbolTable().add(psymbol);

               INSERT(SBIL_push, i);
               types.emplace_back(Type::eString);
            }
            break;
         case CIL_ldtrue:
            INSERT(SBIL_push_true, 0);
            types.emplace_back(Type::eBool);
            break;
         case CIL_ldfalse:
            INSERT(SBIL_push_false, 0);
            types.emplace_back(Type::eBool);
            break;
         case CIL_ldclass:
            {
               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setString(context.mProgram.getStringCache().lookup(*inst.mString));
               int i = context.mProgram.getSymbolTable().add(psymbol);

               INSERT(SBIL_push_class, i);
                              
               types.push_back(Type::fromString(UTEXT("system.Class")));
            }
            break;
         case CIL_ldnull:
            INSERT(SBIL_push_null, 0);
            types.emplace_back();
            break;

         case CIL_ldarg:
            {
               INSERT(SBIL_ldlocal, inst.mInt);

               if ( inst.mInt == 0 && !function.getModifiers().isStatic() )
               {
                  types.push_back(Type::fromString(function.getClass().getName()));
               }
               else
               {
                  const Type& argtype = function.getArguments()[inst.mInt - (function.getModifiers().isStatic() ? 0 : 1)];
                  types.push_back(argtype);
               }
            }
            break;
         case CIL_starg:
            INSERT(SBIL_stlocal, inst.mInt);
            types.pop_back();
            break;
         case CIL_ldloc:
            {
               const Type& type = function.getLocals()[inst.mInt];
               INSERT(SBIL_ldlocal, (inst.mInt + function.getArgumentCount()));
               types.push_back(type);
            }
            break;
         case CIL_stloc:
            INSERT(SBIL_stlocal, (inst.mInt + function.getArgumentCount()));
            if ( pblock->pguard != NULL && pblock->start == index && pblock->guard_type == VirtualGuard::sCatch )
            {
               // no popping here, as this is the store exception instruction in case an exception was thrown.
               // so currently there is nothing on the types stack.
            }
            else
            {
               types.pop_back();
            }
            break;
         case CIL_ldelem:
            {
               // pop indices
               for ( int ei = 0; ei < inst.mInt; ++ ei )
                  types.pop_back();
            
               INSERT(SBIL_ldelem, inst.mInt);
               
               Type arraytype = types.back().getArrayType();
               types.pop_back();                      // pop array
               types.push_back(std::move(arraytype)); // push element
            }
            break;
         case CIL_stelem:
            {
               // pop indices
               for ( int ei = 0; ei < inst.mInt; ++ei )
                  types.pop_back();

               INSERT(SBIL_stelem, inst.mInt);
               types.pop_back();   // pop element
               types.pop_back();   // pop array
            }
            break;
         case CIL_ldfield:
            {
               ASSERT(types.back().isObject());
               
               String path = *inst.mString;
               const VirtualField& field = resolver.resolveField(path);
               INSERT(SBIL_ldfield, field.getIndex());

               types.pop_back(); // object
               types.push_back(field.getType());
            }
            break;
         case CIL_stfield:
            {
               String path = *inst.mString;
               const VirtualField& field = resolver.resolveField(path);

               INSERT(SBIL_stfield, field.getIndex());

               types.pop_back(); // object
               types.pop_back(); // value
            }
            break;
         case CIL_ldstatic:
            {
               const String& path = *inst.mString;
               const VirtualField& field = resolver.resolveStaticField(path);

               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setString(context.mProgram.getStringCache().lookup(field.getClass().getName()));
               int i = context.mProgram.getSymbolTable().add(psymbol);

               INSERT(SBIL_push, i);
               INSERT(SBIL_ldstatic, field.getIndex());

               types.push_back(field.getType());
            }
            break;
         case CIL_ststatic:
            {
               const String& path = *inst.mString;
               const VirtualField& field = resolver.resolveStaticField(path);

               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setString(context.mProgram.getStringCache().lookup(field.getClass().getName()));
               int i = context.mProgram.getSymbolTable().add(psymbol);

               INSERT(SBIL_push, i);
               INSERT(SBIL_ststatic, field.getIndex());

               types.pop_back(); // value
            }
            break;

         // specials

         case CIL_switch:
            {
               INSERT(SBIL_switch, inst.mInt);
            }
            break;

         case CIL_instanceof:
            {
               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setString(context.mProgram.getStringCache().lookup(*inst.mString));
               int index = context.mProgram.getSymbolTable().add(psymbol);
               INSERT(SBIL_instanceof, index);

               types.pop_back(); // value
               types.emplace_back(Type::eBool);
            }
            break;

         // exceptions
            
         case CIL_throw:
            types.pop_back();
            INSERT(SBIL_throw, 0);
            break;
      }
   }
}

void StackIRGenerator::checkAndFixStack(VirtualContext& context, const VirtualFunction& function)
{
   using namespace SBIL;
   using namespace ByteCode;

   typedef std::vector<Instruction*> Calls;
   Calls calls;

   Blocks& blocks = getBlocks();
   
   // fill the stack
   for ( std::size_t index = 0; index < blocks.size(); ++index )
   {
      if ( !hasBlock(index) )
         continue;
         
      Block* pblock = blocks[index];

      Instruction* pinst = pblock->pstart;
      while ( pinst != NULL )
      {
         SBIL::Opcode opcode = (SBIL::Opcode) INST_OPCODE(pinst->inst);

         switch ( opcode ) 
         {
            case SBIL_call:
            case SBIL_call_interface:
            case SBIL_call_virt:
               {
                  int instarg = INST_ARG(pinst->inst);
                  const FunctionSymbol& symbol = static_cast<const FunctionSymbol&>(context.mProgram.getSymbolTable()[instarg]);
                  for ( int arg = 0; arg < symbol.args; ++arg )
                     calls.pop_back();

                  // push return value
                  if ( symbol.returns )
                     calls.push_back(pinst);
               }
               break;
            case SBIL_call_native:
               {
                  int instarg = INST_ARG(pinst->inst);
                  const NativeFunctionSymbol& symbol = static_cast<const NativeFunctionSymbol&>(context.mProgram.getSymbolTable()[instarg]);
                  for (int arg = 0; arg < symbol.args; ++arg)
                     calls.pop_back();

                  // push return value
                  if (symbol.returns)
                     calls.push_back(pinst);
               }
               break;
            case SBIL_ldelem:
               {
                  int size = INST_ARG(pinst->inst);
                  for ( int elem = 0; elem < size; ++elem )
                     calls.pop_back();

                  // pop (object) & push (value)
               }
               break;
            case SBIL_stelem:
               {
                  int size = INST_ARG(pinst->inst) + 2;
                  for ( int elem = 0; elem < size; ++elem )
                     calls.pop_back();
               }
               break;
            case SBIL_stlocal:
               {
                  if ( pinst == pblock->pstart && pblock->pguard != NULL && pblock->guard_type == VirtualGuard::sCatch )
                  {
                     // special case, as it could be that this is the storage of an exception which
                     // is push on the stack whenever the it is thrown.
                     break;
                  }

                  // Fall through to execute the normal code for this instruction
               }
            default:
               {
                  int pop = stackinfo[opcode].pop;
                  for ( int i = 0; i < pop; ++i )
                     calls.pop_back();

                  if ( stackinfo[opcode].push == 1 )
                     calls.push_back(pinst);
               }
         }

         pinst = pinst->next;
      }
   }

   // now see what indices are still there that need an additional pop
   // skip the return value at the top of the stack

   std::size_t size = calls.size() - (function.getReturnType().isVoid() ? 0 : 1);
   for ( std::size_t index = 0; index < size; ++index )
   {
      Instruction* inst = new Instruction;
      inst->inst = MAKE_INST(SBIL_pop,0);
      inst->next = calls[index]->next;

      calls[index]->next = inst;
   }
}

bool StackIRGenerator::buildCode(VirtualContext& context, VirtualFunction& function)
{
   using namespace SBIL;
   using namespace ByteCode;

   unsigned int pos = 0;
   unsigned int size = 32;
   char* pcode = (char*)malloc(size);
   memset(pcode, 0, size);

   Blocks& blocks = getBlocks();

   // fill the stack
   for ( std::size_t index = 0; index < blocks.size(); ++index )
   {
      if ( !hasBlock(index) )
         continue;

      Block* pblock = blocks[index];
      pblock->codepos = pos;

      if ( pblock->pguard != NULL )
      {
         pblock->pguard->locations[pblock->guard_type] = pos;
      }
      else if ( pblock->plookup != NULL )
      {
         switch ( pblock->lookup_type )
         {
         case Block::eDefault:
            pblock->plookup->setDefault(pos);
            break;
         case Block::eEnd:
            pblock->plookup->setEnd(pos);
            break;
         case Block::eValue:
            pblock->plookup->add(pblock->lookup_value, pos);
            break;
         }
      }

      Instruction* pinst = pblock->pstart;
      while ( pinst != NULL )
      {
         SBIL::Opcode opcode = (SBIL::Opcode) INST_OPCODE(pinst->inst);
         if ( opcode == SBIL_jump || opcode == SBIL_jump_true || opcode == SBIL_jump_false )
         {
            JumpPatch* ppatch = new JumpPatch();
            ppatch->pos = pos;
            ppatch->to = pblock->to[0];

            addPatch(ppatch);
         }

         if ( pos >= size )
         {
            size *= 2;
            char* pnew = (char*)realloc(pcode, size);
            if ( pnew == NULL )
            {
               // out of memory?
               free(pcode);
               return false;
            }

            pcode = pnew;
         }

         memmove(&pcode[pos], &pinst->inst, sizeof(int));
         pos += sizeof(int);

         pinst = pinst->next;
      }
   }

   applyPatches(pcode);
   
   function.setCode(pcode, pos);

   return true;
}
