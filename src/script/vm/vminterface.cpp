
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
   std::cout << accessor.getString(1).toUtf8() << std::endl;
}

void Console_print(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   std::cout << accessor.getString(1).toUtf8();
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

void InternalArray_size(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   VirtualArray& thisobject = accessor.getArray(0);

   accessor.setResult(thisobject.size());
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

   UChar c = accessor.getChar(1);

   accessor.setResult(thisstring.indexOf(c));
}

void InternalString_lastIndexOf(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   const String& thisstring = accessor.getString(0);

   UChar c = accessor.getChar(1);

   accessor.setResult(thisstring.lastIndexOf(c));
}

void Char_isAlphaNum(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   UChar c = accessor.getChar(0);

   accessor.setResult(Char::isAlphaNum(c));
}

void Char_isAlpha(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   UChar c = accessor.getChar(0);

   accessor.setResult(Char::isAlpha(c));
}

void Char_isDigit(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   UChar c = accessor.getChar(0);

   accessor.setResult(Char::isDigit(c));
}

void Char_isWhitespace(VirtualMachine& machine, VirtualStackAccessor& accessor)
{
   UChar c = accessor.getChar(0);

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
   registry.addClass(UTEXT("system.Console"));
   registry.addFunction(UTEXT("println(string)"), Console_println);
   registry.addFunction(UTEXT("print(string)"), Console_print);

   registry.addClass(UTEXT("system.ClassLoader"));
   registry.addFunction(UTEXT("doLoadClass(string)"), ClassLoader_doLoadClass);
   
   registry.addClass(UTEXT("system.Class"));
   registry.addFunction(UTEXT("doNewInstance(system.Class)"), Class_doNewInstance);

   registry.addClass(UTEXT("system.Function"));
   registry.addFunction(UTEXT("doInvoke(system.Object)"), Function_doInvoke);
   
   registry.addClass(UTEXT("system.Throwable"));
   registry.addFunction(UTEXT("fillCallStack()"), Throwable_fillCallStack);

   registry.addClass(UTEXT("system.InternalArray"));
   registry.addFunction(UTEXT("size()"), InternalArray_size);
   registry.addFunction(UTEXT("resize(int)"), InternalArray_resize);

   registry.addClass(UTEXT("system.InternalString"));
   registry.addFunction(UTEXT("equals(string)"), InternalString_equals);
   registry.addFunction(UTEXT("length()"), InternalString_length);
   registry.addFunction(UTEXT("subString(int, int)"), InternalString_subString);
   registry.addFunction(UTEXT("getChar(int)"), InternalString_getChar);
   registry.addFunction(UTEXT("indexOf(char)"), InternalString_indexOf);
   registry.addFunction(UTEXT("lastIndexOf(char)"), InternalString_lastIndexOf);
   
   registry.addClass(UTEXT("system.Char"));
   registry.addFunction(UTEXT("isAlphaNum(char)"), Char_isAlphaNum);
   registry.addFunction(UTEXT("isAlpha(char)"), Char_isAlpha);
   registry.addFunction(UTEXT("isDigit(char)"), Char_isDigit);
   registry.addFunction(UTEXT("isWhitespace(char)"), Char_isWhitespace);
   
   registry.addClass(UTEXT("engine.core.Math"));
   registry.addFunction(UTEXT("sqrt(real)"), Math_sqrt);
   registry.addFunction(UTEXT("ceil(real)"), Math_ceil);
}