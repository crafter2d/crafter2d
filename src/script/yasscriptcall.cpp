
#include "yasscriptcall.h"

#include "script/vm/virtualstackaccessor.h"
#include "script/vm/virtualobject.h"

YasScriptCall::YasScriptCall(YasScriptManager& manager, VirtualCall& call) :
   mCall(call),
   mObject(manager)
{

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

c2d::ScriptObject& YasScriptCall::getObject(int arg)
{
   mObject.setObject(mCall.getObject(arg));
   return mObject;
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

void YasScriptCall::setResult(const String& classname, void* pobject, bool owned)
{
   mCall.setResult(mCall.getMachine().instantiateNative(classname, pobject, owned));
}
