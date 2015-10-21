
#include "stackcpu.h"

#include "core/conv/numberconverter.h"
#include "core/log/log.h"
#include "core/smartptr/autoptr.h"
#include "core/smartptr/scopedvalue.h"
#include "core/defines.h"

#include "mod_yas/bytecode/program.h"
#include "mod_yas/bytecode/instruction.h"
#include "mod_yas/bytecode/functionsymbol.h"
#include "mod_yas/bytecode/valuesymbol.h"
#include "mod_yas/common/classregistry.h"
#include "mod_yas/common/callbackfunctor.h"
#include "mod_yas/vm/virtualarray.h"
#include "mod_yas/vm/virtualclass.h"
#include "mod_yas/vm/virtualcontext.h"
#include "mod_yas/vm/virtualexception.h"
#include "mod_yas/vm/virtualfunction.h"
#include "mod_yas/vm/virtualguard.h"
#include "mod_yas/vm/virtualobject.h"
#include "mod_yas/vm/virtualfunctiontableentry.h"
#include "mod_yas/vm/virtuallookuptable.h"
#include "mod_yas/vm/virtualstackaccessor.h"

#include "stackirgenerator.h"

#define OPCODE ((SBIL::Opcode)INST_OPCODE(*(int*)&mpCode[mIP]))
#define ARG    (INST_ARG(*(int*)&mpCode[mIP]));

StackCPU::StackCPU(VirtualMachine& vm):
   CPU(vm),
   mCalls(),
   mStack(),
   mpActiveGuard(NULL),
   mpCode(NULL),
   mIP(0),
   mFP(-1),
   mSavedFP(-1),
   mState()
{
   mCalls.resize(8);
}

ByteCode::IRGenerator* StackCPU::createIRGenerator()
{
   return new StackIRGenerator();
}

VirtualValue StackCPU::execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry, int argc, VirtualValue* pargs)
{
   mStack.push(VirtualValue(object));
   mStack.push(argc, pargs);

   executeStatic(context, object.getClass(), entry);

   // for now run the garbage collector here. have to find the right spot for it.
   if ( !isGarbageCollectionBlocked() )
   {
      getGC().gc(getVM());
   }

   return entry.returns ? mStack.pop() : VirtualValue();
}

void StackCPU::execute(VirtualContext& context, VirtualObject& object, const VirtualFunctionTableEntry& entry)
{
   VirtualValue objectvariant(object);
   if ( entry.mArguments > 1 )
      mStack.insert(mStack.size() - (entry.mArguments - 1), objectvariant);
   else
      mStack.push(objectvariant);

   executeStatic(context, object.getClass(), entry);
}

void StackCPU::executeStatic(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry)
{
   // make sure that when we leave that we clean up the state to when we entered
   // SavedFP is used to identify the starting stack frame
   ScopedValue<int> value(&mSavedFP, mFP, mSavedFP);
   ScopedValue<const char*> code(&mpCode);

   call(context, klass, entry);
   execute(context, entry);
}

void StackCPU::execute(VirtualContext& context, const VirtualFunctionTableEntry& entry)
{
   using namespace ByteCode;
   using namespace SBIL;

   Program& program = context.mProgram;

   while ( mFP > mSavedFP )
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
               VirtualObject& object = instantiate(context, klass, symbol.func);
               mStack.pushObject(object);
            }
            break;
         case SBIL_new_array:
            {
               // n sizes
               // NewArray dimension
               assert(arg == 1);
               
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

               VirtualValue& value = mStack[mStack.size() - symbol.args];
               const VirtualClass* pclass = NULL;
               if ( value.isObject() )
               {
                  const VirtualObject& object = value.asObject();
                  pclass = &object.getClass();
               }
               else if ( value.isString() )
               {
                  pclass = &getStringClass();
               }
               else if ( value.isArray() )
               {
                  pclass = &getArrayClass();
               }

               if ( pclass == NULL )
                  throwException(context, UTEXT("system.NullPointerException"), String::empty());
               else
                  call(context, *pclass, pclass->getVirtualFunctionTable()[symbol.func]);
            }
            break;
         case SBIL_call_interface:
            {
               FunctionSymbol& symbol = (FunctionSymbol&)program.getSymbolTable()[arg];

               VirtualValue& object = mStack[mStack.size() - symbol.args]; // find the object to call the method on
               if ( object.isEmpty() )
               {
                  throwException(context, UTEXT("system.NullPointerException"), String::empty());
               }

               const VirtualClass& theclass = object.asObject().getClass();
               const int* ptable = theclass.getInterfaceLookupTable();
               const VirtualFunctionTableEntry& entry = theclass.getVirtualFunctionTable()[ptable[symbol.func]];

               call(context, theclass, entry);
            }
            break;
         case SBIL_call_native:
            {
               FunctionSymbol& symbol = (FunctionSymbol&)program.getSymbolTable()[arg];

               mCalls[mFP].callnative = true;

               std::vector<VirtualValue> args(symbol.args);
               for ( int index = symbol.args - 1; index >= 0; --index )
               {
                  args[index] = mStack.pop();
               }

               VirtualCall accessor(getVM(), args.data(), symbol.args);
               context.mNativeRegistry.getCallback(symbol.func).exec(accessor);

               mCalls[mFP].callnative = false;

               if ( mState == eExceptionHandling )
               {
                  VirtualObject& exception = mStack.popObject();

                  handleException(context, exception);
               }
               else if ( accessor.hasResult() )
               {
                  mStack.push(accessor.getResult());
               }
            }
            break;
         case SBIL_ret:
            {
               VM::StackFrame& frame = mCalls[mFP--];
               mIP = frame.retaddress;
               mpCode = frame.retcode;
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
               float result = NumberConverter::getInstance().toFloat(value);
               mStack.pushReal(result);  
            }
            break;
         case SBIL_sconv_bool:
            {
               const String& value = mStack.popString();
               if ( value == UTEXT("true") )
                  mStack.pushBool(true);
               else if ( value == UTEXT("false") )
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
               float right = mStack.popReal();
               float left = mStack.popReal();
               mStack.pushReal(left + right);
            }
            break;
         case SBIL_rsub:
            {
               float right = mStack.popReal();
               float left = mStack.popReal();
               mStack.pushReal(left - right);
            }
            break;
         case SBIL_rmul:
            {
               float right = mStack.popReal();
               float left = mStack.popReal();
               mStack.pushReal(left * right);
            }
            break;
         case SBIL_rdiv:
            {
               float right = mStack.popReal();
               float left = mStack.popReal();
               mStack.pushReal(left / right);
            }
            break;
         case SBIL_rneg:
            {
               float left = mStack.popReal();
               mStack.pushReal(-left);
            }
            break;
         case SBIL_cadd:
            {
               UChar right = mStack.popChar();
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
               float right = mStack.popReal();
               float left = mStack.popReal();
               mStack.pushBool(left == right);
            }
            break;
         case SBIL_rcmpne:
            {
               float right = mStack.popReal();
               float left = mStack.popReal();
               mStack.pushBool(left != right);
            }
            break;
         case SBIL_rcmpgt:
            {
               float right = mStack.popReal();
               float left = mStack.popReal();
               mStack.pushBool(left > right);
            }
            break;
         case SBIL_rcmpge:
            {
               float right = mStack.popReal();
               float left = mStack.popReal();
               mStack.pushBool(left >= right);
            }
            break;
         case SBIL_rcmple:
            {
               float right = mStack.popReal();
               float left = mStack.popReal();
               mStack.pushBool(left <= right);
            }
            break;
         case SBIL_rcmplt:
            {
               float right = mStack.popReal();
               float left  = mStack.popReal();
               mStack.pushBool(left < right);
            }
            break;
         case SBIL_ccmpeq:
            {
               UChar right = mStack.popChar();
               UChar left  = mStack.popChar();
               mStack.pushBool(left == right);
            }
            break;
         case SBIL_ccmpne:
            {
               UChar right = mStack.popChar();
               UChar left  = mStack.popChar();
               mStack.pushBool(left != right);
            }
            break;
         case SBIL_ccmpgt:
            {
               UChar right = mStack.popChar();
               UChar left  = mStack.popChar();
               mStack.pushBool(left > right);
            }
            break;
         case SBIL_ccmpge:
            {
               UChar right = mStack.popChar();
               UChar left  = mStack.popChar();
               mStack.pushBool(left >= right);
            }
            break;
         case SBIL_ccmple:
            {
               UChar right = mStack.popChar();
               UChar left  = mStack.popChar();
               mStack.pushBool(left <= right);
            }
            break;
         case SBIL_ccmplt:
            {
               UChar right = mStack.popChar();
               UChar left  = mStack.popChar();
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
               bool sign = (arg & 0x1);
               int offset = (arg >> 1) * (sign ? -1 : 1);
               mIP += offset;
            }
            break;
         case SBIL_jump_true:
            {
               bool value = mStack.popBool();
               if ( value )
               {
                  bool sign = (arg & 0x1);
                  int offset = (arg >> 1) * (sign ? -1 : 1);
                  mIP += offset;
               }
            }
            break;
         case SBIL_jump_false:
            {
               bool value = mStack.popBool();
               if ( !value )
               {
                  bool sign = (arg & 0x1);
                  int offset = (arg >> 1) * (sign ? -1 : 1);
                  mIP += offset;
               }
            }
            break;

         case SBIL_ldfield:
            {
               VirtualValue obj = mStack.pop();
               ASSERT(obj.isObject());
               if ( obj.isEmpty() )
                  throwException(context, UTEXT("system.NullPointerException"), String::empty());
               else               
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
               VirtualValue& value = mCalls[mFP].locals[arg];
               mStack.push(value);
            }
            break;
         case SBIL_stlocal:
            {
               mCalls[mFP].locals[arg] = mStack.pop();
            }
            break;
         case SBIL_ldelem:
            {
               // stack content:
               // - n indices
               // - array object
               // the actual array object is below the indices in the stack

               VirtualValue& variant = mStack[mStack.size() - arg - 1];
               ASSERT(variant.isArray());

               VirtualArray* parray = &variant.asArray();
               for ( int index = 0; index < arg - 1; index++ )
               {
                  int i = mStack.popInt();
                  parray = &parray->at(i).asArray();
               }

               int i = mStack.popInt();

               mStack.pop(1); // pop the array from the stack

               if ( i >= parray->size() )
                  throwException(context, UTEXT("ArrayIndexOutOfBoundsException"), String::empty());
               else              
                  mStack.push(parray->at(i));
            }
            break;
         case SBIL_stelem:
            {
               // stack content:
               // - n indices
               // - array object
               // - value to store

               VirtualValue& variant = mStack[mStack.size() - arg - 1];
               ASSERT(variant.isArray());

               VirtualArray* parray = &variant.asArray();
               for ( int index = 0; index < arg-1; index++ )
               {
                  int i = mStack.popInt();
                  parray = &parray->at(i).asArray();
               }

               int i = mStack.popInt();

               mStack.pop(1); // <-- pop array

               VirtualValue val = mStack.pop();

               if ( i >= parray->size() )
                  throwException(context, UTEXT("system.ArrayIndexOutOfBoundsException"), String::empty());
               else
                  parray->at(i) = val;
            }
            break;
         case SBIL_ldstatic:
            {
               const String& classname = mStack.popString();
               VirtualClass& klass = context.mClassTable.resolve(classname);

               mStack.push(klass.getStatic(arg));
            }
            break;
         case SBIL_ststatic:
            {
               String classname = mStack.popString();
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
            mStack.push(VirtualValue());
            break;
         case SBIL_push_class:
            {
               const ValueSymbol& symbol = static_cast<const ValueSymbol&>(program.getSymbolTable()[arg]);
               const String& classname = symbol.value.asString();

               if ( classname.isEmpty() )
               {
                  VirtualObject& object = mStack.popObject();
                  mStack.pushObject(object.getClass().getClassObject());
               }
               else
               {
                  VirtualClass& klass = context.resolveClass(classname);
                  mStack.pushObject(klass.getClassObject());
               }
            }
            break;

         // - Specials

         case SBIL_switch:
            {
               mIP = mCalls[mFP].pentry->lookup(arg, mStack.pop());
            }
            break;
         case SBIL_instanceof:
            {
               const ValueSymbol& symbol = static_cast<const ValueSymbol&>(program.getSymbolTable()[arg]);
               const VirtualClass& klass = context.mClassTable.resolve(symbol.value.asString());
               
               if ( mStack.back().isEmpty() )
               {
                  throwException(context, UTEXT("system.NullPointerException"), String::empty());
               }
               else
               {
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
            }
            break;

         // - Exceptions

         case SBIL_throw:
            {
               VirtualObject& exception = mStack.popObject();
               if ( !handleException(context, exception) )
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

   FunctionSymbol& symbol = (FunctionSymbol&)context.mProgram.getSymbolTable()[symbolindex];
   const VirtualClass& klass = context.mClassTable.resolve(symbol.klass);
   const VirtualFunctionTableEntry& entry = klass.getVirtualFunctionTable()[symbol.func];
   call(context, klass, entry);
}

void StackCPU::call(VirtualContext& context, const VirtualClass& klass, const VirtualFunctionTableEntry& entry)
{
   if ( entry.mpFunction->hasCode() ) {} else
   {
      compile(context, entry);
   }

   mFP++;
   if ( mFP >= mCalls.size() )
   {
      mCalls.resize(mCalls.size() * 2);
   }

   VM::StackFrame& frame = mCalls[mFP];
   frame.pclass = & klass;
   frame.pentry = & entry;

   frame.locals.resize(entry.mArguments + entry.mLocals);
   for ( int index = entry.mArguments - 1; index >= 0; --index )
   {
      frame.locals[index] = mStack.pop();
   }

   frame.retaddress = mIP;
   frame.retcode    = mpCode;
   frame.sp = mStack.size();

   mIP = 0;
   mpCode = entry.mpFunction->getCode();
}

void StackCPU::compile(VirtualContext& context, const VirtualFunctionTableEntry& entry)
{
   try
   {
      StackIRGenerator generator;
      generator.compile(context, *entry.mpFunction);
   }
   catch ( std::exception* e )
   {
      Log::getInstance().error("Exception during compilation: %s", e->what());
   }
}

// - Garbage collection
   
void StackCPU::mark()
{
   mStack.mark();
   
   for ( int index = 0; index <= mFP; ++index )
   {
      VM::StackFrame::Locals& locals = mCalls[index].locals;
      for ( auto& variant : locals )
      {
         if ( variant.isObject() )
         {
            variant.asObject().mark();
         }
         else if ( variant.isArray() )
         {
            variant.asArray().mark();
         }
      }
   }
}

// - Exception handling
   
String StackCPU::buildCallStack() const
{
   String result;

   for ( int index = mFP; index >= 0; --index )
   {
      const VM::StackFrame& frame = mCalls[index];
      ASSERT_PTR(frame.pentry);

      result += String("- ");
      result += String(frame.pclass->getName() + '.' + frame.pentry->getName() + '(');

      for ( int index = 0; index < frame.pentry->mArguments; index++ )
      {
         const VirtualValue& value = frame.locals[index];
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

bool StackCPU::handleException(VirtualContext& context, VirtualObject& exception)
{
   ASSERT(mFP >= 0);
   
   while ( mFP >= 0 )
   {
      const VirtualFunctionTableEntry& entry = *mCalls[mFP].pentry;
      const VirtualGuard* pguard = entry.findGuard(mIP);

      if ( pguard != NULL )
      {
         mStack.pushObject(exception);

         mIP = pguard->locations[VirtualGuard::sCatch];

         mState = eRunning;

         return true;
      }
      else
      {
         VM::StackFrame& frame = mCalls[mFP];
         mStack.setSize(frame.sp);
         mIP = frame.retaddress;
         mpCode = frame.retcode;

         mFP--;
         if ( mFP >= 0 && mCalls[mFP].callnative )
         {
            // need to bail out of native function calls before handling an catch/finally
            // an example that triggers this is the UnitTesting framework. it at runtime
            // calls function using the class reflection. asserts fired there must be caught
            // after the native has been abandoned.

            mState = eExceptionHandling;
            mStack.pushObject(exception);
            
            return true;
         }
      }
   }

   return false;
}
