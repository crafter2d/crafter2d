
#include "vminterface.h"

#include <iostream>

#include "core/string/char.h"
#include "core/math/math.h"

#include "script/common/classregistry.h"
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
   registry.addClass("Console");
   registry.addFunction("println", Console_println);
   registry.addFunction("print", Console_print);

   registry.addClass("ClassLoader");
   registry.addFunction("doLoadClass", ClassLoader_doLoadClass);
   
   registry.addClass("Class");
   registry.addFunction("doNewInstance", Class_doNewInstance);

   registry.addClass("Function");
   registry.addFunction("doInvoke", Function_doInvoke);
   
   registry.addClass("Throwable");
   registry.addFunction("fillCallStack", Throwable_fillCallStack);

   registry.addClass("InternalArray");
   registry.addFunction("resize", InternalArray_resize);

   registry.addClass("InternalString");
   registry.addFunction("equals", InternalString_equals);
   registry.addFunction("length", InternalString_length);
   registry.addFunction("subString", InternalString_subString);
   registry.addFunction("getChar", InternalString_getChar);
   registry.addFunction("indexOf", InternalString_indexOf);
   registry.addFunction("lastIndexOf", InternalString_lastIndexOf);
   
   registry.addClass("Char");
   registry.addFunction("isAlphaNum", Char_isAlphaNum);
   registry.addFunction("isAlpha", Char_isAlpha);
   registry.addFunction("isDigit", Char_isDigit);
   registry.addFunction("isWhitespace", Char_isWhitespace);
   
   registry.addClass("Math");
   registry.addFunction("sqrt", Math_sqrt);
   registry.addFunction("ceil", Math_ceil);
}