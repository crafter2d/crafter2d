
#include "yasscriptcall.h"

#include "core/script/iscriptable.h"

#include "mod_yas/vm/virtualstackaccessor.h"
#include "mod_yas/vm/virtualobject.h"

#include "yasscriptmanager.h"

YasScriptCall::YasScriptCall(YasScriptManager& manager, VirtualCall& call) :
   mManager(manager),
   mCall(call)
{

}

c2d::ScriptObjectHandle YasScriptCall::newObject(const String& classname)
{
   YasScriptObject* presult = new YasScriptObject(mManager);
   presult->setThis(*mCall.getMachine().instantiate(classname));
   return c2d::ScriptObjectHandle(presult);
}

bool YasScriptCall::getBoolean(int arg)
{
   return mCall.getBoolean(arg);
}

int YasScriptCall::getInt(int arg)
{
   return mCall.getInt(arg);
}

float YasScriptCall::getReal(int arg)
{
   return mCall.getReal(arg);
}

UChar YasScriptCall::getChar(int arg)
{
   return mCall.getChar(arg);
}

const String& YasScriptCall::getString(int arg)
{
   return mCall.getString(arg);
}

c2d::ScriptObjectHandle YasScriptCall::getObject(int arg)
{
   YasScriptObject* pobject = new YasScriptObject(mManager);
   pobject->setThis(mCall.getObject(arg));
   return c2d::ScriptObjectHandle(pobject);
}

void* YasScriptCall::useInstance(int arg)
{
   return mCall.getObject(arg).useNativeObjectPtr();
}

void* YasScriptCall::getInstance(int arg)
{
   ASSERT(mCall.getObject(arg).hasNativeObject());
   return mCall.getObject(arg).getNativeObjectPtr();
}

void YasScriptCall::setInstance(int arg, void* pobject, bool owned)
{
   auto& virtualobject = mCall.getObject(arg);
   virtualobject.setNativeObject(pobject);
   virtualobject.setOwner(owned);

   mManager.registerObject(virtualobject);
}

void YasScriptCall::setResult(bool value)
{
   mCall.setResult(value);
}

void YasScriptCall::setResult(int value)
{
   mCall.setResult(value);
}

void YasScriptCall::setResult(float value)
{
   mCall.setResult(value);
}

void YasScriptCall::setResult(UChar value)
{
   mCall.setResult(value);
}

void YasScriptCall::setResult(const String& value)
{
   mCall.setResult(value);
}

void YasScriptCall::setResult(Variant value)
{
   mCall.setResult(mManager.toValue(value));
}

void YasScriptCall::setResult(c2d::ScriptObjectHandle& object)
{
   YasScriptObject& yasobject = static_cast<YasScriptObject&>(*object);
   mCall.setResult(yasobject.getThis());
}

void YasScriptCall::setResult(c2d::IScriptable& scriptable, bool owned)
{
   mCall.setResult(mCall.getMachine().instantiateNative(scriptable.getClassName(), &scriptable, owned));
}

void YasScriptCall::setResult(const String& classname, void* pobject, bool owned)
{
   mCall.setResult(mCall.getMachine().instantiateNative(classname, pobject, owned));
}
