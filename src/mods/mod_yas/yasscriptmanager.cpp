
#include "yasscriptmanager.h"

#include "yasscript.h"
#include "yasscriptobject.h"

YasScriptManager::YasScriptManager():
   ScriptManager(),
   mVirtualContext(),
   mVirtualMachine(mVirtualContext)
{

}

// - Get/set

VirtualMachine& YasScriptManager::getVM()
{
   return mVirtualMachine;
}

// - Overrides

c2d::Script* YasScriptManager::load(const String& classname, void* pobject, bool owned)
{
   YasScript* pscript = NULL;

   VirtualObject* pvirtualobject = mVirtualMachine.instantiateNative(classname, pobject, owned);
   if ( pvirtualobject != NULL )
   {
      pscript = new YasScript(*this);
      pscript->setThis(*pvirtualobject);
   }

   return pscript;
}

void YasScriptManager::addRootObject(c2d::ScriptObject& object)
{
   YasScriptObject& yasobject = static_cast<YasScriptObject&>(object);
   mVirtualMachine.addRootObject(yasobject.getObject());
}

// - Operations

void YasScriptManager::registerObject(YasScriptObject& object)
{
   mVirtualMachine.registerNative(object.getObject(), object.getInstance());
}
