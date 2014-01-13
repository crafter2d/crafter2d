
#include "yasscriptobject.h"

#include "core/defines.h"

#include "script/vm/virtualobject.h"
#include "script/vm/virtualmachine.h"

#include "yasscriptmanager.h"

YasScriptObject::YasScriptObject(YasScriptManager& manager) :
   mManager(manager),
   mpObject(NULL)
{
}

// - Get/set

VirtualObject& YasScriptObject::getObject()
{
   ASSERT_PTR(mpObject);
   return *mpObject;
}

void YasScriptObject::setObject(VirtualObject& object)
{
   mpObject = &object;
}

// - Arguments

void YasScriptObject::arg(int index, bool arg)
{
   mArguments[index].setBool(arg);
}

void YasScriptObject::arg(int index, int arg)
{
   mArguments[index].setInt(arg);
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

void YasScriptObject::call(const String& function)
{
   mManager.getVM().execute(*mpObject, function, mArguments.size(), &mArguments[0]);

   mArguments.clear();
}
