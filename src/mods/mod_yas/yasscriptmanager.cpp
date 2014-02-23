
#include "yasscriptmanager.h"

#include "core/vfs/filesystem.h"

#include "yasscriptobject.h"
#include "yasscriptregistrator.h"

YasScriptManager::YasScriptManager():
   ScriptManager(),
   mVirtualContext(),
   mVirtualMachine(mVirtualContext)
{

   FileSystem::getInstance().addPath(UTEXT("data.zip\\scripts"));

   mVirtualMachine.initialize();
}

// - Get/set

VirtualMachine& YasScriptManager::getVM()
{
   return mVirtualMachine;
}

// - Overrides

c2d::ScriptRegistrator* YasScriptManager::getRegistrator()
{
   return new YasScriptRegistrator(*this);
}

c2d::ScriptObject* YasScriptManager::load(const String& classname, void* pobject, bool owned)
{
   YasScriptObject* pscript = NULL;

   VirtualObject* pvirtualobject = mVirtualMachine.instantiateNative(classname, pobject, owned);
   if ( pvirtualobject != NULL )
   {
      pscript = new YasScriptObject(*this);
      pscript->setThis(*pvirtualobject);
   }

   return pscript;
}

void YasScriptManager::addRootObject(c2d::ScriptObject& object)
{
   YasScriptObject& yasobject = static_cast<YasScriptObject&>(object);
   mVirtualMachine.addRootObject(yasobject.getThis());
}

// - Operations

void YasScriptManager::registerObject(YasScriptObject& object)
{
   mVirtualMachine.registerNative(object.getThis(), object.getInstance());
}

// - Execution

Variant YasScriptManager::execute(VirtualObject& object, const String& function, int argc, VirtualValue* pargv)
{
   VirtualValue result = mVirtualMachine.execute(object, function, argc, pargv);
   return toVariant(result);
}

// - Conversion

Variant YasScriptManager::toVariant(const VirtualValue& value)
{
   if ( value.isBoolean() )
   {
      return Variant(value.asBoolean());
   }
   else if ( value.isNumber() )
   {
      return Variant(value.asNumber());
   }
   else if ( value.isReal() )
   {
      return Variant(value.asReal());
   }
   else if ( value.isChar() )
   {
      return Variant(value.asChar());
   }
   else if ( value.isString() )
   {
      return Variant(value.asString());
   }
   return Variant();
}

VirtualValue YasScriptManager::toValue(const Variant& value)
{
   if ( value.isInt() )
   {
      return VirtualValue(value.asInt());
   }
   else if ( value.isReal() )
   {
      return VirtualValue(value.asReal());
   }
   else if ( value.isBool() )
   {
      return VirtualValue(value.asBool());
   }
   else if ( value.isChar() )
   {
      return VirtualValue(value.asChar());
   }
   else if ( value.isString() )
   {
      return VirtualValue(mVirtualContext.mStringCache.lookup(value.asString()));
   }

   return VirtualValue();
}

