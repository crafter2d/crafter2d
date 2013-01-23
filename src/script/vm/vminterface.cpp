
#include "vminterface.h"

#include <iostream>

#include "core/string/char.h"
#include "core/math/math.h"

#include "script/common/classregistry.h"
#include "script/common/classregistration.h"
#include "script/common/functionregistration.h"

#include "virtualmachine.h"
#include "virtualstackaccessor.h"
#include "virtualarray.h"
#include "virtualobject.h"

void Console_println(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   std::cout << accessor.getString(1).toStdString() << std::endl;
}

void Console_print(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   std::cout << accessor.getString(1).toStdString();
}

void ClassLoader_doLoadClass(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& classname = accessor.getString(1);
   accessor.setResult(machine.loadClass(classname));
}

void Class_doNewInstance(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& classobject = accessor.getObject(1);

   String name = classobject.getMember(0).asString().getString();
   VirtualObject* pobject = machine.instantiate(name);

   accessor.setResult(*pobject);
}

void Function_doInvoke(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualObject& thisobject = accessor.getThis();
   VirtualObject& instance = accessor.getObject(1);

   String fncname = thisobject.getMember(0).asString().getString();

   machine.execute(instance, fncname);
}

void Throwable_fillCallStack(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   String callstack = machine.buildCallStack();

   accessor.setResult(callstack);
}

void InternalArray_resize(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualArray& thisobject = accessor.getArray(0);

   int newsize = accessor.getInt(1);

   thisobject.resize(newsize);
}

void InternalString_equals(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& thisstring = accessor.getString(0);
   const String& thatstring = accessor.getString(1);

   accessor.setResult(thisstring == thatstring);
}

void InternalString_subString(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& thisstring = accessor.getString(0);
   
   int pos = accessor.getInt(1);
   int len = accessor.getInt(2);

   accessor.setResult(thisstring.subStr(pos, len));
}

void InternalString_length(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& thisstring = accessor.getString(0);

   accessor.setResult((int) thisstring.length());
}

void InternalString_getChar(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& thisstring = accessor.getString(0);

   int index = accessor.getInt(1);

   accessor.setResult(thisstring[index]);
}

void InternalString_indexOf(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& thisstring = accessor.getString(0);

   char c = accessor.getChar(1);

   accessor.setResult(thisstring.indexOf(c));
}

void InternalString_lastIndexOf(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& thisstring = accessor.getString(0);

   char c = accessor.getChar(1);

   accessor.setResult(thisstring.lastIndexOf(c));
}

void Char_isAlphaNum(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   char c = accessor.getChar(0);

   accessor.setResult(Char::isAlphaNum(c));
}

void Char_isAlpha(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   char c = accessor.getChar(0);

   accessor.setResult(Char::isAlpha(c));
}

void Char_isDigit(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   char c = accessor.getChar(0);

   accessor.setResult(Char::isDigit(c));
}

void Char_isWhitespace(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   char c = accessor.getChar(0);

   accessor.setResult(Char::isWhitespace(c));
}

void Math_sqrt(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   double value = accessor.getReal(0);

   accessor.setResult(sqrt(value));
}

void Math_ceil(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   double value = accessor.getReal(0);

   accessor.setResult(ceil(value));
}

SCRIPT_API void VMInterface::registerCommonFunctions(ClassRegistry& registry)
{
   {
      ClassRegistration& reg = registry.addClass("Console");
      reg.addFunction(FunctionRegistration::Function(String("println"), Console_println));
      reg.addFunction(FunctionRegistration::Function(String("print"), Console_print));
   }

   {
      ClassRegistration& reg = registry.addClass("ClassLoader");
      reg.addFunction(FunctionRegistration::Function(String("doLoadClass"), ClassLoader_doLoadClass));
   }

   {
      ClassRegistration& reg = registry.addClass("Class");
      reg.addFunction(FunctionRegistration::Function(String("doNewInstance"), Class_doNewInstance));
   }

   {
      ClassRegistration& reg = registry.addClass("Function");
      reg.addFunction(FunctionRegistration::Function(String("doInvoke"), Function_doInvoke));
   }

   {
      ClassRegistration& reg = registry.addClass("Throwable");
      reg.addFunction(FunctionRegistration::Function(String("fillCallStack"), Throwable_fillCallStack));
   }

   {
      ClassRegistration& reg = registry.addClass("InternalArray");
      reg.addFunction(FunctionRegistration::Function(String("resize"), InternalArray_resize));
   }

   {
      ClassRegistration& reg = registry.addClass("InternalString");
      reg.addFunction(FunctionRegistration::Function(String("equals"), InternalString_equals));
      reg.addFunction(FunctionRegistration::Function(String("length"), InternalString_length));
      reg.addFunction(FunctionRegistration::Function(String("subString"), InternalString_subString));
      reg.addFunction(FunctionRegistration::Function(String("getChar"), InternalString_getChar));
      reg.addFunction(FunctionRegistration::Function(String("indexOf"), InternalString_indexOf));
      reg.addFunction(FunctionRegistration::Function(String("lastIndexOf"), InternalString_lastIndexOf));
   }

   {
      ClassRegistration& reg = registry.addClass("Char");
      reg.addFunction(FunctionRegistration::Function(String("isAlphaNum"), Char_isAlphaNum));
      reg.addFunction(FunctionRegistration::Function(String("isAlpha"), Char_isAlpha));
      reg.addFunction(FunctionRegistration::Function(String("isDigit"), Char_isDigit));
      reg.addFunction(FunctionRegistration::Function(String("isWhitespace"), Char_isWhitespace));
   }

   {
      ClassRegistration& reg = registry.addClass("Math");
      reg.addFunction(FunctionRegistration::Function(String("sqrt"), Math_sqrt));
      reg.addFunction(FunctionRegistration::Function(String("ceil"), Math_ceil));
   }
}