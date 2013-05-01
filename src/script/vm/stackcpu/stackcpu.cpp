
#include "stackcpu.h"

#include "core/defines.h"

#include "script/bytecode/program.h"
#include "script/bytecode/instruction.h"
#include "script/bytecode/functionsymbol.h"
#include "script/bytecode/valuesymbol.h"
#include "script/common/classregistry.h"
#include "script/vm/virtualarray.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/virtualobject.h"
#include "script/vm/virtualfunctiontableentry.h"
#include "script/vm/virtualstackaccessor.h"

#include "stackirgenerator.h"

#define OPCODE ((SBIL::Opcode)INST_OPCODE(*(int*)&pcode[ip]))
#define ARG    (INST_ARG(*(int*)&pcode[ip]));

StackCPU::StackCPU(VirtualMachine& vm):
   CPU(vm),
   mStack()
{
}

ByteCode::IRGenerator* StackCPU::createIRGenerator()
{
   return new StackIRGenerator();
}

void StackCPU::execute(VirtualContext& context, const VirtualClass& pclass, const VirtualFunctionTableEntry& entry)
{
   using namespace ByteCode;
   using namespace SBIL;

   ByteCode::Program& program = getProgram();
   const char* pcode = &program.getCode()[entry.mInstruction];

   int ip = entry.mInstruction;

   while ( ip < program.getSize() )
   {
      SBIL::Opcode opcode = OPCODE;

      switch ( INST_OPCODE(*((int*)pcode)) )
      {
         case SBIL_dup:
            break;

         case SBIL_new:
            {
               int arg = ARG;
               FunctionSymbol& symbol = (FunctionSymbol&)program.getSymbolTable()[arg];
               VirtualObject* pobject = instantiate(*symbol.klass, symbol.func);
               ASSERT_PTR(pobject);

               mStack.pushObject(*pobject);
            }
            break;
         case SBIL_new_array:
            {
            }
            break;
         case SBIL_new_native:
            {
            }
            break;

         case SBIL_call:
            {
               int arg = ARG;
               call(context, arg);
            }
            break;
         case SBIL_call_static:
            {
               int arg = ARG;
               FunctionSymbol& symbol = (FunctionSymbol&)program.getSymbolTable()[arg];
               const VirtualFunctionTableEntry& entry = symbol.klass->getVirtualFunctionTable()[symbol.func];
               call(context, *symbol.klass, entry);
            }
            break;
         case SBIL_call_interface:
            {
               int arg = ARG;
               FunctionSymbol& symbol = (FunctionSymbol&)program.getSymbolTable()[arg];

               const Variant& object = mStack[mStack.size() - symbol.args]; // find the object to call the method on
               ASSERT(object.isObject());

               const VirtualClass& theclass = object.asObject().getClass();
               const VirtualFunctionTableEntry& entry = theclass.getVirtualFunctionTable().resolveInterface(symbol.func);

               call(context, theclass, entry);
            }
            break;
         case SBIL_call_native:
            {
               int arg = ARG;
               FunctionSymbol& symbol = (FunctionSymbol&)program.getSymbolTable()[arg];

               VirtualStackAccessor accessor(context, mStack, symbol.args);
               (*context.mNativeRegistry.getCallback(symbol.func))(getVM(), accessor);

               mStack.pop(symbol.args);
               if ( accessor.hasResult() )
               {
                  mStack.push(accessor.getResult());
               }
            }
            break;
         case SBIL_ret:
            {
               mIP = mCalls.top().retaddress;
               mCalls.pop();
            }
            break;

         case SBIL_iadd:
            {
               int right = mStack.popInt();
               int left = mStack.popInt();
               mStack.pushInt(left + right);
            }
            break;
         case SBIL_isub:
            {
               int right = mStack.popInt();
               int left = mStack.popInt();
               mStack.pushInt(left - right);
            }
            break;
         case SBIL_imul:
            {
               int right = mStack.popInt();
               int left = mStack.popInt();
               mStack.pushInt(left * right);
            }
            break;
         case SBIL_idiv:
            {
               int right = mStack.popInt();
               int left = mStack.popInt();
               mStack.pushInt(left / right);
            }
            break;
         case SBIL_ineg:
            {
               int left = mStack.popInt();
               mStack.pushInt(-left);
            }
            break;
         case SBIL_irem:
            {
               int right = mStack.popInt();
               int left = mStack.popInt();
               mStack.pushInt(left % right);
            }
            break;

         case SBIL_radd:
            {
               double right = mStack.popReal();
               double left = mStack.popReal();
               mStack.pushReal(left + right);
            }
            break;
         case SBIL_rsub:
            {
               double right = mStack.popReal();
               double left = mStack.popReal();
               mStack.pushReal(left - right);
            }
            break;
         case SBIL_rmul:
            {
               double right = mStack.popReal();
               double left = mStack.popReal();
               mStack.pushReal(left * right);
            }
            break;
         case SBIL_rdiv:
            {
               double right = mStack.popReal();
               double left = mStack.popReal();
               mStack.pushReal(left / right);
            }
            break;
         case SBIL_rneg:
            {
               double left = mStack.popReal();
               mStack.pushReal(-left);
            }
            break;
         case SBIL_cadd:
            {
               char right = mStack.popChar();
               const String& left = mStack.popString();
               mStack.pushString(context.mStringCache.lookup(left + right));
            }
            break;
         case SBIL_sadd:
            {
               const String& right = mStack.popString();
               const String& left = mStack.popString();
               mStack.pushString(context.mStringCache.lookup(left + right));
            }
            break;

         case SBIL_shr:
            {
               int right = mStack.popInt();
               int left = mStack.popInt();
               mStack.pushInt(left >> right);
            }
            break;
         case SBIL_shl:
            {
               int right = mStack.popInt();
               int left = mStack.popInt();
               mStack.pushInt(left << right);
            }
            break;
         case SBIL_or:
            {
               int right = mStack.popInt();
               int left  = mStack.popInt();
               mStack.pushInt(left | right);
            }
            break;
         case SBIL_and:
            {
               int right = mStack.popInt();
               int left  = mStack.popInt();
               mStack.pushInt(left & right);
            }
            break;
         case SBIL_xor:
            {
               int right = mStack.popInt();
               int left  = mStack.popInt();
               mStack.pushInt(left ^ right);
            }
            break;
         case SBIL_not:
            {
               bool left = mStack.popBool();
               mStack.pushBool(!left);
            }
            break;

         case SBIL_bcmpeq:
            {
               bool right = mStack.popBool();
               bool left = mStack.popBool();
               mStack.pushBool(left == right);
            }
            break;
         case SBIL_bcmpne:
            {
               bool right = mStack.popBool();
               bool left = mStack.popBool();
               mStack.pushBool(left != right);
            }
            break;
         case SBIL_icmpeq:
            {
               int right = mStack.popInt();
               int left  = mStack.popInt();
               mStack.pushBool(left == right);
            }
            break;
         case SBIL_icmpne:
            {
               int right = mStack.popInt();
               int left  = mStack.popInt();
               mStack.pushBool(left != right);
            }
            break;
         case SBIL_icmpgt:
            {
               int right = mStack.popInt();
               int left  = mStack.popInt();
               mStack.pushBool(left > right);
            }
            break;
         case SBIL_icmpge:
            {
               int right = mStack.popInt();
               int left  = mStack.popInt();
               mStack.pushBool(left >= right);
            }
            break;
         case SBIL_icmple:
            {
               int right = mStack.popInt();
               int left  = mStack.popInt();
               mStack.pushBool(left <= right);
            }
            break;
         case SBIL_icmplt:
            {
               int right = mStack.popInt();
               int left  = mStack.popInt();
               mStack.pushBool(left < right);
            }
            break;
         case SBIL_rcmpeq:
            {
               double right = mStack.popReal();
               double left  = mStack.popReal();
               mStack.pushBool(left == right);
            }
            break;
         case SBIL_rcmpne:
            {
               double right = mStack.popReal();
               double left  = mStack.popReal();
               mStack.pushBool(left != right);
            }
            break;
         case SBIL_rcmpgt:
            {
               double right = mStack.popReal();
               double left  = mStack.popReal();
               mStack.pushBool(left > right);
            }
            break;
         case SBIL_rcmpge:
            {
               double right = mStack.popReal();
               double left  = mStack.popReal();
               mStack.pushBool(left >= right);
            }
            break;
         case SBIL_rcmple:
            {
               double right = mStack.popReal();
               double left  = mStack.popReal();
               mStack.pushBool(left <= right);
            }
            break;
         case SBIL_rcmplt:
            {
               double right = mStack.popReal();
               double left  = mStack.popReal();
               mStack.pushBool(left < right);
            }
            break;
         case SBIL_ccmpeq:
            {
               char right = mStack.popChar();
               char left  = mStack.popChar();
               mStack.pushBool(left == right);
            }
            break;
         case SBIL_ccmpne:
            {
               char right = mStack.popChar();
               char left  = mStack.popChar();
               mStack.pushBool(left != right);
            }
            break;
         case SBIL_ccmpgt:
            {
               char right = mStack.popChar();
               char left  = mStack.popChar();
               mStack.pushBool(left > right);
            }
            break;
         case SBIL_ccmpge:
            {
               char right = mStack.popChar();
               char left  = mStack.popChar();
               mStack.pushBool(left >= right);
            }
            break;
         case SBIL_ccmple:
            {
               char right = mStack.popChar();
               char left  = mStack.popChar();
               mStack.pushBool(left <= right);
            }
            break;
         case SBIL_ccmplt:
            {
               char right = mStack.popChar();
               char left  = mStack.popChar();
               mStack.pushBool(left < right);
            }
            break;
         case SBIL_scmpeq:
            {
               const String& right = mStack.popString();
               const String& left  = mStack.popString();
               mStack.pushBool(left == right);
            }
            break;
         case SBIL_scmpne:
            {
               const String& right = mStack.popString();
               const String& left  = mStack.popString();
               mStack.pushBool(left != right);
            }
            break;
         case SBIL_scmpgt:
            {
               const String& right = mStack.popString();
               const String& left  = mStack.popString();
               mStack.pushBool(left > right);
            }
            break;
         case SBIL_scmpge:
            {
               const String& right = mStack.popString();
               const String& left  = mStack.popString();
               mStack.pushBool(left >= right);
            }
            break;
         case SBIL_scmple:
            {
               const String& right = mStack.popString();
               const String& left  = mStack.popString();
               mStack.pushBool(left <= right);
            }
            break;
         case SBIL_scmplt:
            {
               const String& right = mStack.popString();
               const String& left  = mStack.popString();
               mStack.pushBool(left < right);
            }
            break;
         case SBIL_ocmpeq:
            {
               const VirtualObject& right = mStack.popObject();
               const VirtualObject& left  = mStack.popObject();
               mStack.pushBool(&left == &right);
            }
            break;
         case SBIL_acmpeq:
            {
               const VirtualArray& right = mStack.popArray();
               const VirtualArray& left  = mStack.popArray();
               mStack.pushBool(&left == &right);
            }
            break;

         case SBIL_jump:
            {
               int arg = ARG;
               pcode += arg;
            }
            break;
         case SBIL_jump_true:
            {
               int arg = ARG;
               bool value = mStack.popBool();
               if ( value )
               {
                  pcode += arg;
               }
            }
            break;
         case SBIL_jump_false:
            {
               int arg = ARG;
               bool value = mStack.popBool();
               if ( !value )
               {
                  pcode += arg;
               }
            }
            break;

         case SBIL_ldfield:
            {
               Variant obj = mStack.pop();

               if ( obj.isObject() )
               {
                  int arg = ARG;
                  mStack.push(obj.asObject().getMember(arg));
               }
               else if ( obj.isArray() )
               {
                  mStack.push(Variant(obj.asArray().size())); // length attribute
               }
               else if ( obj.isEmpty() )
               {
                  //throwException("system.NullPointerException", "");
               }
            }
            break;
         case SBIL_stfield:
            {
               ASSERT(mStack.back().isObject());
               VirtualObject& object = mStack.popObject();

               int arg = ARG;
               object.setMember(arg, mStack.pop());
            }
            break;
         case SBIL_ldlocal:
            {
               int arg = ARG;
               Variant& value = mCalls.top().locals[arg];
               mStack.push(value);
            }
            break;
         case SBIL_stlocal:
            {
               int arg = ARG;
               mCalls.top().locals[arg] = mStack.pop();
            }
            break;
         case SBIL_ldelem:
            {
               // stack content:
            // - n indices
            // - array object
            // the actual array object is below the indices in the stack

            int arg = ARG;
            Variant& variant = mStack[mStack.size() - arg - 1];
            ASSERT(variant.isArray());

            VirtualArray* parray = &variant.asArray();
            for ( int index = 0; index < arg - 1; index++ )
            {
               int i = mStack.popInt();
               parray = &parray->at(i).asArray();
            }

            int i = mStack.popInt();

            mStack.pop(1); // pop the array from the stack

            mStack.push(parray->at(i));
            }
            break;
         case SBIL_stelem:
            {
               // stack content:
               // - n indices
               // - array object
               // - value to store

               int arg = ARG;
               Variant& variant = mStack[mStack.size() - arg - 1];
               ASSERT(variant.isArray());

               VirtualArray* parray = &variant.asArray();
               for ( int index = 0; index < arg-1; index++ )
               {
                  int i = mStack.popInt();
                  parray = &parray->at(i).asArray();
               }

               int i = mStack.popInt();

               mStack.pop(1); // <-- pop array

               parray->at(i) = mStack.pop();
            }
            break;
         case SBIL_ldstatic:
            {
               int classlit = mStack.popInt();

               const ValueSymbol& symbol = (const ValueSymbol&)program.getSymbolTable()[classlit];
               const String& classname = symbol.value.asString().getString();
               VirtualClass& c = context.mClassTable.resolve(classname);

               int arg = ARG;
               mStack.push(c.getStatic(arg));
            }
            break;
         case SBIL_ststatic:
            {
               int classlit = mStack.popInt();

               const ValueSymbol& symbol = (const ValueSymbol&)program.getSymbolTable()[classlit];
               const String& classname = symbol.value.asString().getString();
               VirtualClass& c = context.mClassTable.resolve(classname);

               int arg = ARG;
               c.setStatic(arg, mStack.pop());
            }
            break;

         case SBIL_push:
            {
               int arg = ARG;
               ValueSymbol& symbol = (ValueSymbol&)program.getSymbolTable()[arg];
               mStack.push(symbol.value);
            }
            break;
         case SBIL_pushi:
            {
               int arg = ARG;
               mStack.pushInt(arg);
            }
            break;
         case SBIL_pushc:
            {
               int arg = ARG;
               mStack.pushChar((char)arg);
            }
            break;
         case SBIL_push_i0:
            mStack.pushInt(0);
            break;
         case SBIL_push_i1:
            mStack.pushInt(1);
            break;
         case SBIL_push_i2:
            mStack.pushInt(2);
            break;
         case SBIL_push_r0:
            mStack.pushReal(0);
            break;
         case SBIL_push_r1:
            mStack.pushReal(1);
            break;
         case SBIL_push_r2:
            mStack.pushReal(2);
            break;
         case SBIL_push_true:
            mStack.pushBool(true);
            break;
         case SBIL_push_false:
            mStack.pushBool(false);
            break;
         case SBIL_push_class:
            {
               String name;
               int arg = ARG;
               if ( arg == 1 )
               {
                  name = mStack.back().asObject().getClass().getName();
               }
               else
               {
                  const ValueSymbol& symbol = (const ValueSymbol&)program.getSymbolTable()[mStack.back().asInt()];
                  name = symbol.value.asString().getString();
               }
               
               const VirtualClass& classloader = context.mClassTable.resolve("system.ClassLoader");
               const VirtualFunctionTableEntry* pentry = classloader.getVirtualFunctionTable().findByName("findClass");
               const Variant& classloaderobject = classloader.getStatic(0);

               mStack.push(classloaderobject);
               mStack.pushString(context.mStringCache.lookup(name));

               call(context, classloader, *pentry);
            }
            break;

         case SBIL_enterguard:
            {
            }
            break;
         case SBIL_enterguard_final:
            {
            }
            break;
         case SBIL_leaveguard:
            {
            }
            break;
            
         case SBIL_nop:
         default:
            break;
      }
   }
}

void StackCPU::call(VirtualContext& context, int symbolindex)
{
   using namespace ByteCode;

   FunctionSymbol& symbol = (FunctionSymbol&)getProgram().getSymbolTable()[symbolindex];
   const Variant& object = mStack[mStack.size() - symbol.args]; // find the object to call the method on
   if ( object.isObject() )
   {
      const VirtualClass& klass = object.asObject().getClass();
      call(context, klass, klass.getVirtualFunctionTable()[symbol.func]);
   }
   else if ( object.isArray() )
   {
      const VirtualClass& klass = getArrayClass();
      call(context, klass, getArrayClass().getVirtualFunctionTable()[symbol.func]);
   }
   else if ( object.isString() )
   {
      const VirtualClass& klass = getStringClass();
      call(context, klass, getStringClass().getVirtualFunctionTable()[symbol.func]);
   }
   else
   {
      ASSERT(object.isEmpty());
      //throwException("system.NullPointerException", "");
   }
}

void StackCPU::call(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry)
{
   VM::StackFrame frame;
   frame.pclass = & klass;
   frame.pentry = & entry;
   frame.retaddress = mIP;

   frame.locals.resize(entry.mArguments);
   for ( int index = entry.mArguments - 1; index >= 0; --index )
   {
      frame.locals.push_back(mStack.pop());
   }

   mCalls.push(frame);
}

VirtualObject* StackCPU::instantiate(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry)
{
   ASSERT(klass.canInstantiate());
   
   VirtualObject* pobject = allocObject();
   klass.instantiate(*pobject);

   Variant objectvariant(*pobject);

   // run field initialization expressions
   const VirtualFunctionTableEntry& varinit_entry = klass.getVirtualFunctionTable()[1];
   mStack.push(objectvariant);
   call(context, klass, varinit_entry);

   // run the constructor (entry)
   mStack.insert(mStack.size() - (entry.mArguments - 1), objectvariant);
   call(context, klass, entry);

   // register object with GC
   getGC().collect(pobject);

   return pobject;
}
