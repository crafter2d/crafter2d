
#include "stackirgenerator.h"

#include <vector>
#include <stack>
#include <string.h>

#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "mod_yas/bytecode/block.h"
#include "mod_yas/bytecode/program.h"
#include "mod_yas/bytecode/symboltable.h"
#include "mod_yas/bytecode/functionsymbol.h"
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
   std::stack<AutoPtr<Type>> types;

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
               types.push(types.top());
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
                  types.pop();

               types.push(Type::fromString(constructor.getClass().getName()));

               INSERT(SBIL_new, i);
            }
            break;
         case CIL_newarray:
            {
               AutoPtr<Type> type = Type::fromString(*inst.mString);

               int depth = type->getArrayDimension();
               for ( int arg = 0; arg < depth; ++arg )
                  types.pop();

               types.push(type.release());

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
                  types.pop();

               if ( psymbol->returns )
                  types.push(func.getReturnType().clone());

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
                  types.pop();

               if ( psymbol->returns )
                  types.push(func.getReturnType().clone());

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
                  types.pop();

               if ( psymbol->returns )
                  types.push(func.getReturnType().clone());

               INSERT(SBIL_call_interface, i);
            }
            break;
         case CIL_call_native:
            {
               VirtualFunction& func = resolver.resolveFunction(*inst.mString);

               const FunctionRegistration* pfuncreg = context.mNativeRegistry.findCallback(*inst.mString);
               ASSERT_PTR(pfuncreg);

               FunctionSymbol* psymbol = new FunctionSymbol;
               psymbol->func = pfuncreg->getIndex();
               psymbol->args = func.getArgumentCount();
               psymbol->returns = !func.getReturnType().isVoid();
               int i = context.mProgram.getSymbolTable().add(psymbol);

               for ( int arg = 0; arg < psymbol->args; ++arg )
                  types.pop();

               if ( psymbol->returns )
                  types.push(func.getReturnType().clone());

               INSERT(SBIL_call_native, i);
            }
            break;
         case CIL_ret:
            INSERT(SBIL_ret, inst.mInt);
            break;

         // Conversion

         case CIL_bconv_str:
            INSERT(SBIL_bconv_str, 0);
            types.pop();
            types.push(new Type(Type::eString));
            break;
         case CIL_iconv_real:
            INSERT(SBIL_iconv_real, 0);
            types.pop();
            types.push(new Type(Type::eReal));
            break;
         case CIL_iconv_str:
            INSERT(SBIL_iconv_str, 0);
            types.pop();
            types.push(new Type(Type::eString));
            break;
         case CIL_rconv_int:
            INSERT(SBIL_rconv_int, 0);
            types.pop();
            types.push(new Type(Type::eInt));
            break;
         case CIL_rconv_str:
            INSERT(SBIL_rconv_str, 0);
            types.pop();
            types.push(new Type(Type::eString));
            break;
         case CIL_cconv_str:
            INSERT(SBIL_cconv_str, 0);
            types.pop();
            types.push(new Type(Type::eString));
            break;
         case CIL_sconv_bool:
            INSERT(SBIL_sconv_bool, 0);
            types.pop();
            types.push(new Type(Type::eBool));
            break;
         case CIL_sconv_int:
            INSERT(SBIL_sconv_int, 0);
            types.pop();
            types.push(new Type(Type::eInt));
            break;
         case CIL_sconv_real:
            INSERT(SBIL_sconv_real, 0);
            types.pop();
            types.push(new Type(Type::eReal));
            break;

         // Math

         case CIL_add:
            {
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
               {
                  case Type::eInt:
                     INSERT(SBIL_iadd, 0);
                     types.push(new Type(Type::eInt));
                     break;
                  case Type::eReal:
                     INSERT(SBIL_radd, 0);
                     types.push(new Type(Type::eReal));
                     break;
                  case Type::eString:
                     INSERT(SBIL_sadd, 0);
                     types.push(new Type(Type::eString));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_sub:
            {
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
               {
                  case Type::eInt:
                     INSERT(SBIL_isub, 0);
                     types.push(new Type(Type::eInt));
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rsub, 0);
                     types.push(new Type(Type::eReal));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_mul:
            {
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
               {
                  case Type::eInt:
                     INSERT(SBIL_imul, 0);
                     types.push(new Type(Type::eInt));
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rmul, 0);
                     types.push(new Type(Type::eReal));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_div:
            {
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
               {
                  case Type::eInt:
                     INSERT(SBIL_idiv, 0);
                     types.push(new Type(Type::eInt));
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rdiv, 0);
                     types.push(new Type(Type::eReal));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_neg:
            {
               Type type = *types.top();
               types.pop();

               switch ( type.getKind() )
               {
                  case Type::eInt:
                     INSERT(SBIL_ineg, 0);
                     types.push(new Type(Type::eInt));
                     break;
                  case Type::eReal:
                     INSERT(SBIL_rneg, 0);
                     types.push(new Type(Type::eReal));
                     break;
                  default:
                     UNREACHABLE("Invalid type");
                     break;
               } 
            }
            break;
         case CIL_rem:
            INSERT(SBIL_irem, 0);
            types.push(new Type(Type::eInt));
            break;
         case CIL_shl:
            INSERT(SBIL_shl, 0);
            types.push(new Type(Type::eInt));
            break;
         case CIL_shr:
            INSERT(SBIL_shr, 0);
            types.push(new Type(Type::eInt));
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
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
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

               types.push(new Type(Type::eBool));
            }
            break;
         case CIL_cmpne:
            {
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
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
               types.push(new Type(Type::eBool));
            }
            break;
         case CIL_cmpgt:
            {
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
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
               types.push(new Type(Type::eBool));
            }
            break;
         case CIL_cmpge:
            {
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
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
               types.push(new Type(Type::eBool));
            }
            break;
         case CIL_cmple:
            {
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
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
               types.push(new Type(Type::eBool));
            }
            break;
         case CIL_cmplt:
            {
               Type type = *types.top();
               types.pop();
               types.pop();

               switch ( type.getKind() )
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
               types.push(new Type(Type::eBool));
            }
            break;
         case CIL_isnull:
            INSERT(SBIL_isnull, 0);
            types.push(new Type(Type::eBool));
            break;

         case CIL_jump:
            INSERT(SBIL_jump, 0);
            break;
         case CIL_jump_true:
            ASSERT(types.top()->isBool());
            INSERT(SBIL_jump_true, 0);
            types.pop();
            break;
         case CIL_jump_false:
            ASSERT(types.top()->isBool());
            INSERT(SBIL_jump_false, 0);
            types.pop();
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
            types.push(new Type(Type::eInt));
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
            types.push(new Type(Type::eReal));
            break;
         case CIL_ldchar:
            INSERT(SBIL_pushc, inst.mInt);
            types.push(new Type(Type::eChar));
            break;
         case CIL_ldstr:
            {
               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setString(context.mProgram.getStringCache().lookup(*inst.mString));
               int i = context.mProgram.getSymbolTable().add(psymbol);

               INSERT(SBIL_push, i);
               types.push(new Type(Type::eString));
            }
            break;
         case CIL_ldtrue:
            INSERT(SBIL_push_true, 0);
            types.push(new Type(Type::eBool));
            break;
         case CIL_ldfalse:
            INSERT(SBIL_push_false, 0);
            types.push(new Type(Type::eBool));
            break;
         case CIL_ldclass:
            {
               ValueSymbol* psymbol = new ValueSymbol();
               psymbol->value.setString(context.mProgram.getStringCache().lookup(*inst.mString));
               int i = context.mProgram.getSymbolTable().add(psymbol);

               INSERT(SBIL_push_class, i);
                              
               types.push(Type::fromString(UTEXT("system.Class")));
            }
            break;
         case CIL_ldnull:
            INSERT(SBIL_push_null, 0);
            types.push(new Type());
            break;

         case CIL_ldarg:
            {
               INSERT(SBIL_ldlocal, inst.mInt);

               if ( inst.mInt == 0 && !function.getModifiers().isStatic() )
               {
                  types.push(Type::fromString(function.getClass().getName()));
               }
               else
               {
                  const Type& argtype = function.getArguments()[inst.mInt - (function.getModifiers().isStatic() ? 0 : 1)];
                  types.push(argtype.clone());
               }
            }
            break;
         case CIL_starg:
            INSERT(SBIL_stlocal, inst.mInt);
            types.pop();
            break;
         case CIL_ldloc:
            {
               const Type& type = function.getLocals()[inst.mInt];
               INSERT(SBIL_ldlocal, (inst.mInt + function.getArgumentCount()));
               types.push(type.clone());
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
               types.pop();
            }
            break;
         case CIL_ldelem:
            {
               // pop indices
               for ( int ei = 0; ei < inst.mInt; ++ ei )
                  types.pop();
            
               INSERT(SBIL_ldelem, inst.mInt);
               
               Type type = *types.top();
               types.pop();                     // pop array
               types.push(type.getArrayType().clone()); // push element
            }
            break;
         case CIL_stelem:
            {
               // pop indices
               for ( int ei = 0; ei < inst.mInt; ++ei )
                  types.pop();

               INSERT(SBIL_stelem, inst.mInt);
               types.pop();   // pop element
               types.pop();   // pop array
            }
            break;
         case CIL_ldfield:
            {
               Type type = *types.top();
               ASSERT(type.getKind() == Type::eObject);
               types.pop();
               
               String path = *inst.mString;
               const VirtualField& field = resolver.resolveField(path);
               INSERT(SBIL_ldfield, field.getIndex());

               types.push(field.getType().clone());
            }
            break;
         case CIL_stfield:
            {
               String path = *inst.mString;
               const VirtualField& field = resolver.resolveField(path);

               INSERT(SBIL_stfield, field.getIndex());

               types.pop(); // object
               types.pop(); // value
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

               types.push(field.getType().clone());
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

               types.pop();
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

               types.pop();
               types.push(new Type(Type::eBool));
            }
            break;

         // exceptions
            
         case CIL_throw:
            types.pop();
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
            case SBIL_call_native:
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
