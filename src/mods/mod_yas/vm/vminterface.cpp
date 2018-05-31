
#include "vminterface.h"

#include <iostream>
#include <cmath>

#include "core/string/char.h"
#include "core/math/math.h"

#include "mod_yas/common/classregistry.h"
#include "mod_yas/common/functionregistration.h"

#include "virtualmachine.h"
#include "virtualstackaccessor.h"
#include "virtualarray.h"
#include "virtualobject.h"
#include "virtualcallbackfunctor.h"

void Console_println(VirtualCall& accessor)
{
   std::cout << accessor.getString(1).toUtf8() << std::endl;
}

void Console_print(VirtualCall& accessor)
{
   std::cout << accessor.getString(1).toUtf8();
}

void ClassLoader_doLoadClass(VirtualCall& accessor)
{
   const String& classname = accessor.getString(1);
   accessor.setResult(accessor.getMachine().loadClass(classname));
}

void Class_doNewInstance(VirtualCall& accessor)
{
   VirtualObject& classobject = accessor.getObject(1);

   String name = classobject.getMember(0).asString();
   VirtualObject* pobject = accessor.getMachine().instantiate(name);

   accessor.setResult(*pobject);
}

void Function_doInvoke(VirtualCall& accessor)
{
   VirtualObject& thisobject = accessor.getThis();
   VirtualObject& instance = accessor.getObject(1);

   String fncname = thisobject.getMember(0).asString();

   accessor.getMachine().execute(instance, fncname, 0, nullptr);
}

void Throwable_fillCallStack(VirtualCall& accessor)
{
   String callstack = accessor.getMachine().buildCallStack();

   accessor.setResult(callstack);
}

void InternalArray_size(VirtualCall& accessor)
{
   VirtualArray& thisobject = accessor.getArray(0);

   accessor.setResult(thisobject.size());
}

void InternalArray_resize(VirtualCall& accessor)
{
   VirtualArray& thisobject = accessor.getArray(0);

   int newsize = accessor.getInt(1);

   thisobject.resize(newsize);
}

void InternalString_equals(VirtualCall& accessor)
{
   const String& thisstring = accessor.getString(0);
   const String& thatstring = accessor.getString(1);

   accessor.setResult(thisstring == thatstring);
}

void InternalString_subString(VirtualCall& accessor)
{
   const String& thisstring = accessor.getString(0);
   
   int pos = accessor.getInt(1);
   int len = accessor.getInt(2);

   accessor.setResult(thisstring.subStr(pos, len));
}

void InternalString_length(VirtualCall& accessor)
{
   const String& thisstring = accessor.getString(0);

   accessor.setResult((int) thisstring.length());
}

void InternalString_getChar(VirtualCall& accessor)
{
   const String& thisstring = accessor.getString(0);

   int index = accessor.getInt(1);

   accessor.setResult(thisstring[index]);
}

void InternalString_indexOf(VirtualCall& accessor)
{
   const String& thisstring = accessor.getString(0);

   UChar c = accessor.getChar(1);

   accessor.setResult(static_cast<int>(thisstring.indexOf(c)));
}

void InternalString_lastIndexOf(VirtualCall& accessor)
{
   const String& thisstring = accessor.getString(0);

   UChar c = accessor.getChar(1);

   accessor.setResult(static_cast<int>(thisstring.lastIndexOf(c)));
}

void Char_isAlphaNum(VirtualCall& accessor)
{
   UChar c = accessor.getChar(0);

   accessor.setResult(Char::isAlphaNum(c));
}

void Char_isAlpha(VirtualCall& accessor)
{
   UChar c = accessor.getChar(0);

   accessor.setResult(Char::isAlpha(c));
}

void Char_isDigit(VirtualCall& accessor)
{
   UChar c = accessor.getChar(0);

   accessor.setResult(Char::isDigit(c));
}

void Char_isWhitespace(VirtualCall& accessor)
{
   UChar c = accessor.getChar(0);

   accessor.setResult(Char::isWhitespace(c));
}

void Math_sqrt(VirtualCall& accessor)
{
   float value = accessor.getReal(0);

   accessor.setResult(sqrtf(value));
}

void Math_ceil(VirtualCall& accessor)
{
   float value = accessor.getReal(0);

   accessor.setResult(ceilf(value));
}

void VMInterface::registerCommonFunctions(ClassRegistry& registry)
{
   registry.addClass(UTEXT("system.Console"));
   registry.addFunction(UTEXT("println(string)"), new VirtualCallbackFunctor(Console_println));
   registry.addFunction(UTEXT("print(string)"), new VirtualCallbackFunctor(Console_print));

   registry.addClass(UTEXT("system.ClassLoader"));
   registry.addFunction(UTEXT("doLoadClass(string)"), new VirtualCallbackFunctor(ClassLoader_doLoadClass));
   
   registry.addClass(UTEXT("system.Class"));
   registry.addFunction(UTEXT("doNewInstance(system.Class)"), new VirtualCallbackFunctor(Class_doNewInstance));

   registry.addClass(UTEXT("system.Function"));
   registry.addFunction(UTEXT("doInvoke(system.Object)"), new VirtualCallbackFunctor(Function_doInvoke));
   
   registry.addClass(UTEXT("system.Throwable"));
   registry.addFunction(UTEXT("fillCallStack()"), new VirtualCallbackFunctor(Throwable_fillCallStack));

   registry.addClass(UTEXT("system.InternalArray"));
   registry.addFunction(UTEXT("size()"), new VirtualCallbackFunctor(InternalArray_size));
   registry.addFunction(UTEXT("resize(int)"), new VirtualCallbackFunctor(InternalArray_resize));

   registry.addClass(UTEXT("system.InternalString"));
   registry.addFunction(UTEXT("equals(string)"), new VirtualCallbackFunctor(InternalString_equals));
   registry.addFunction(UTEXT("length()"), new VirtualCallbackFunctor(InternalString_length));
   registry.addFunction(UTEXT("subString(int, int)"), new VirtualCallbackFunctor(InternalString_subString));
   registry.addFunction(UTEXT("getChar(int)"), new VirtualCallbackFunctor(InternalString_getChar));
   registry.addFunction(UTEXT("indexOf(char)"), new VirtualCallbackFunctor(InternalString_indexOf));
   registry.addFunction(UTEXT("lastIndexOf(char)"), new VirtualCallbackFunctor(InternalString_lastIndexOf));
   
   registry.addClass(UTEXT("system.Char"));
   registry.addFunction(UTEXT("isAlphaNum(char)"), new VirtualCallbackFunctor(Char_isAlphaNum));
   registry.addFunction(UTEXT("isAlpha(char)"), new VirtualCallbackFunctor(Char_isAlpha));
   registry.addFunction(UTEXT("isDigit(char)"), new VirtualCallbackFunctor(Char_isDigit));
   registry.addFunction(UTEXT("isWhitespace(char)"), new VirtualCallbackFunctor(Char_isWhitespace));
   
   registry.addClass(UTEXT("engine.core.Math"));
   registry.addFunction(UTEXT("sqrt(real)"), new VirtualCallbackFunctor(Math_sqrt));
   registry.addFunction(UTEXT("ceil(real)"), new VirtualCallbackFunctor(Math_ceil));
}