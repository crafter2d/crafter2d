
#include "stackcpu.h"

#include "core/conv/numberconverter.h"
#include "core/smartptr/scopedvalue.h"
#include "core/defines.h"

#include "script/bytecode/program.h"
#include "script/bytecode/instruction.h"
#include "script/bytecode/functionsymbol.h"
#include "script/bytecode/valuesymbol.h"
#include "script/common/classregistry.h"
#include "script/vm/virtualarray.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualcontext.h"
#include "script/vm/virtualexception.h"
#include "script/vm/virtualguard.h"
#include "script/vm/virtualobject.h"
#include "script/vm/virtualfunctiontableentry.h"
#include "script/vm/virtualstackaccessor.h"

#include "stackirgenerator.h"

#define OPCODE ((SBIL::Opcode)INST_OPCODE(*(int*)&pcode[mIP]))
#define ARG    (INST_ARG(*(int*)&pcode[mIP]));

StackCPU::StackCPU(VirtualMachine& vm):
   CPU(vm),
   mStack(),
   mIP(0),
   mFP(-1)
{
}

ByteCode::IRGenerator* StackCPU::createIRGenerator()
{
   return new StackIRGenerator();
}

Variant StackCPU::execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry, int argc, Variant* pargs)
{
   mStack.push(argc, pargs);

   return execute(context, object, entry);
}

Variant StackCPU::execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry)
{
   Variant objectvariant(object);
   if ( entry.mArguments > 1 )
      mStack.insert(mStack.size() - (entry.mArguments - 1), objectvariant);
   else
      mStack.push(objectvariant);

   execute(context, object.getClass(), entry);

   return entry.returns ? mStack.pop() : Variant();
}

void StackCPU::execute(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry)
{
   ScopedValue<int> value(&mSavedFP, mFP, mSavedFP);

   call(context, klass, entry);
   execute(context);

   // for now run the garbage collector here. have to find the right spot for it.
   getGC().gc(getVM());
}

void StackCPU::execute(VirtualContext& context)
{
   using namespace ByteCode;
   using namespace SBIL;

   ByteCode::Program& program = getProgram();
   const char* pcode = program.getCode();

   while ( mIP < program.getSize() && mFP > mSavedFP )
   {
      SBIL::Opcode opcode = OPCODE;
      int arg = ARG;
      mIP += sizeof(int);

      switch ( opcode )
      {
         case SBIL_dup:
            mStack.push(mStack.back());
            break;

         case SBIL_new:
            {
               FunctionSymbol& symbol = (FunctionSymbol&)program.getSymbolTable()[arg];
               VirtualClass& klass = context.mClassTable.resolve(symbol.klass);
               VirtualObject* pobject = instantiate(context, klass, symbol.func);
               ASSERT_PTR(pobject);

               mStack.pushObject(*pobject);
            }
            break;
         case SBIL_new_array:
            {
               // n sizes
               // NewArray dimension
               
               // no full support yet for multi-dimensional arrays
               VirtualArray* parray = instantiateArray();
               VirtualArray* pinit = parray;
               for ( int index = 0; index < arg; index++ )
               {
                  int size = mStack.popInt();

                  pinit->addLevel(size);
                  if ( index < arg - 1 )
                  { // need to implement this for multi dimensional arrays
                     // VirtualArray* ptemp = pinit;
                  }
               }

               mStack.pushArray(*parray);
            }
            break;

         case SBIL_call:
            {
               call(context, arg);
            }
            break;
         case SBIL_call_virt:
            {
               const FunctionSymbol& symbol = (FunctionSymbol&)program.getSymbolTable()[arg];
               const VirtualObject& object = mStack[mStack.size() - symbol.args].asObject();
               const VirtualClass& klass = object.getClass();
               call(context, klass, klass.getVirtualFunctionTable()[symbol.func]);
            }
            break;
         case SBIL_call_interface:
            {
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
               mIP = mCalls[mFP--].retaddress;
               mCalls.pop_back();
            }
            break;

         case SBIL_bconv_str:
            {
               bool value = mStack.popBool();
               String string(value ? String("true") : String("false"));
               mStack.pushString(context.mStringCache.lookup(string));
            }
            break;
         case SBIL_iconv_real:
            mStack.back().int2real();
            break;
         case SBIL_iconv_str:
            {
               String result;
               NumberConverter::getInstance().format(result, mStack.popInt());
               mStack.pushString(context.mStringCache.lookup(result));
            }
            break;
         case SBIL_rconv_int:
            mStack.back().real2int();
            break;
         case SBIL_rconv_str:
            {
               String result;
               NumberConverter::getInstance().format(result, mStack.popReal());
               mStack.pushString(context.mStringCache.lookup(result));
            }
            break;
         case SBIL_cconv_str:
            {
               String result;
               result = mStack.popChar();
               mStack.pushString(context.mStringCache.lookup(result));
            }
            break;
         case SBIL_sconv_int:
            {
               const String& value = mStack.popString();
               int result = NumberConverter::getInstance().toInt(value);
               mStack.pushInt(result);  
            }
            break;
         case SBIL_sconv_real:
            {
               const String& value = mStack.popString();
               double result = NumberConverter::getInstance().toDouble(value);
               mStack.pushReal(result);  
            }
            break;
         case SBIL_sconv_bool:
            {
               const String& value = mStack.popString();
               if ( value == "true" )
                  mStack.pushBool(true);
               else if ( value == "false" )
                  mStack.pushBool(false);
               else
               {
                  // throw an exception
               }
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
         case SBIL_isnull:
            mStack.pushBool(mStack.pop().isEmpty());
            break;

         case SBIL_jump:
            {
               mIP += arg;
            }
            break;
         case SBIL_jump_true:
            {
               bool value = mStack.popBool();
               if ( value )
               {
                  mIP += arg;
               }
            }
            break;
         case SBIL_jump_false:
            {
               bool value = mStack.popBool();
               if ( !value )
               {
                  mIP += arg;
               }
            }
            break;

         case SBIL_ldfield:
            {
               Variant obj = mStack.pop();
               if ( obj.isEmpty() )
               {
                  throwException(context, "system.NullPointerException", "");
               }

               mStack.push(obj.asObject().getMember(arg));
            }
            break;
         case SBIL_stfield:
            {
               ASSERT(mStack.back().isObject());
               VirtualObject& object = mStack.popObject();
               object.setMember(arg, mStack.pop());
            }
            break;
         case SBIL_ldlocal:
            {
               Variant& value = mCalls.back().locals[arg];
               mStack.push(value);
            }
            break;
         case SBIL_stlocal:
            {
               mCalls.back().locals[arg] = mStack.pop();
            }
            break;
         case SBIL_ldelem:
            {
               // stack content:
               // - n indices
               // - array object
               // the actual array object is below the indices in the stack

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

               mStack.push(c.getStatic(arg));
            }
            break;
         case SBIL_ststatic:
            {
               //int classlit = mStack.popInt();
               String classname = mStack.popString();

               // const ValueSymbol& symbol = (const ValueSymbol&)program.getSymbolTable()[classlit];
               // const String& classname = symbol.value.asString().getString();
               VirtualClass& c = context.mClassTable.resolve(classname);

               c.setStatic(arg, mStack.pop());
            }
            break;

         case SBIL_push:
            {
               ValueSymbol& symbol = (ValueSymbol&)program.getSymbolTable()[arg];
               mStack.push(symbol.value);
            }
            break;
         case SBIL_pushi:
            {
               mStack.pushInt(arg);
            }
            break;
         case SBIL_pushc:
            {
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
         case SBIL_push_null:
            mStack.push(Variant());
            break;
         case SBIL_push_class:
            {
               String name;
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

         // - Specials

         case SBIL_switch:
            {
               /*
               String classname = context.mLiteralTable[mStack.popInt()].getValue().asString().getString();
               const VirtualLookupTable& table = context.mClassTable.resolve(classname).getLookupTable(instruction.getArgument());

               int codeindex = table.lookup(mStack.back());
               mStack.pop();

               mCall.jump(codeindex);
               */
            }
            break;
         case SBIL_instanceof:
            {
               const ValueSymbol& symbol = (const ValueSymbol&)program.getSymbolTable()[arg];
               const VirtualClass& klass = context.mClassTable.resolve(symbol.value.asString().getString());
               
               ASSERT(mStack.back().isObject());
               VirtualObject& object = mStack.popObject();

               if ( object.getClass().isBaseClass(klass)
                 || object.getClass().implements(klass) )
               {
                  mStack.pushBool(true);
               }
               else
               {
                  mStack.pushBool(false);
               }
            }
            break;

         // - Exceptions

         case SBIL_throw:
            {
               VirtualObject& exception = mStack.popObject();
               if ( !handleException(exception) )
               {
                  // ow boy!! need to figure out what to do here.
                  return;
               }
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
      const VirtualClass& klass = context.mClassTable.resolve(symbol.klass);// object.asObject().getClass();
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
   frame.sp     = mStack.size();
   frame.retaddress = mIP;

   frame.locals.resize(entry.mArguments);
   for ( int index = entry.mArguments - 1; index >= 0; --index )
   {
      frame.locals[index] = mStack.pop();
   }

   mCalls.push_back(frame);
   mFP++;

   mIP = entry.mInstruction;
}

// - Garbage collection
   
void StackCPU::mark()
{
   mStack.mark();
}

// - Exception handling
   
String StackCPU::buildCallStack() const
{
   String result;

   for ( std::size_t index = mFP; index >= 0; ++index )
   {
      const VM::StackFrame& frame = mCalls[index];
      ASSERT_PTR(frame.pentry);

      result += String("- ");
      result += String(frame.pclass->getName() + '.' + frame.pentry->mName + '(');

      for ( int index = 0; index < frame.pentry->mArguments; index++ )
      {
         const Variant& value = frame.locals[index];
         result += value.typeAsString() + String(" = ") + value.toString();

         if ( index < frame.pentry->mArguments - 1 )
         {
            result += String(", ");
         }
      }

      result += String(")\n");
   }

   return result;
}

bool StackCPU::handleException(VirtualObject& exception)
{
   if ( mFP <= 0 )
   {
      return false;
   }
   
   const VirtualFunctionTableEntry& entry = *mCalls[mFP].pentry;

   /*
   if ( entry.guards.size() > 0 )
   {
      
      const VM::Guard& guard = entry.guards.back();
      if ( mIP <= guard.mJumpTo )
      {
         // exception in code, jump to first catch statement
         mStack.pushObject(exception);

         mIP = guard.mJumpTo;

         return true;
      }
      else
      {
         // exception within a catch handler, so see if there is another try/catch around this one
         frame.guards.pop_back();

         return handleException(exception);
      }
   }
   else
   {
      // no try/catch in this method, jump out and see if the previous function has one
      mStack.pop(mStack.size() - mCalls[mFP].sp);
      mIP = frame.retaddress;
      mCalls.pop_back();

      return handleException(exception);
   }
   
      */
   return true;
}
