
#include "yasscriptobject.h"

#include "core/defines.h"

#include "mod_yas/vm/virtualobject.h"
#include "mod_yas/vm/virtualmachine.h"

#include "yasscriptmanager.h"

YasScriptObject::YasScriptObject(YasScriptManager& manager) :
   mManager(manager),
   mpObject(NULL)
{
}

// - Get/set

VirtualObject& YasScriptObject::getThis()
{
   ASSERT_PTR(mpObject);
   return *mpObject;
}

void YasScriptObject::setThis(VirtualObject& object)
{
   mpObject = &object;
}

// - Arguments

void YasScriptObject::arg(int index, bool arg)
{
   mArguments[index].setBoolean(arg);
}

void YasScriptObject::arg(int index, int arg)
{
   mArguments[index].setNumber(arg);
}

void YasScriptObject::arg(int index, float arg)
{
   mArguments[index].setReal(arg);
}

void YasScriptObject::arg(int index, UChar arg)
{
   mArguments[index].setChar(arg);
}

void YasScriptObject::arg(int index, const String& arg)
{
   mArguments[index].setString(mManager.getVM().getContext().mStringCache.lookup(arg));
}

void YasScriptObject::arg(int index, void* pinstance)
{
   VirtualObject* pobject = mManager.getVM().lookupNative(pinstance);
   if ( pobject == NULL )
   {
      throw std::exception("Could not find object. Make sure it has been registered.");
   }

   mArguments[index].setObject(*pobject);
}

void YasScriptObject::arg(int index, const String& classname, void* pinstance)
{
   mArguments[index].setObject(mManager.getVM().instantiateNative(classname, pinstance, false));
}

// - Member

void YasScriptObject::setMember(int index, const Variant& value)
{
   VirtualValue yasvalue = mManager.toValue(value);
   mpObject->setMember(index, yasvalue);
}

// - Instance

void* YasScriptObject::getInstance()
{
   return mpObject->getNativeObjectPtr();
}

void* YasScriptObject::useInstance()
{
   return mpObject->useNativeObjectPtr();
}

void YasScriptObject::setInstance(void* pinstance, bool owned)
{
   mpObject->setNativeObject(pinstance);
   mpObject->setOwner(owned);

   mManager.registerObject(*this);
}

// - Operations

void YasScriptObject::prepareCall(int args)
{
   mArguments.resize(args);
}

Variant YasScriptObject::call(const String& function)
{
   return mManager.execute(*mpObject, function, mArguments.size(), mArguments.data());
}

Variant YasScriptObject::call(const String& function, int argc, Variant* args)
{
   mArguments.resize(argc);
   for ( int index = 0; index < argc; ++index )
   {
      mArguments[index] = mManager.toValue(args[index]);
   }

   return mManager.execute(*mpObject, function, argc, &mArguments[0]);
}
