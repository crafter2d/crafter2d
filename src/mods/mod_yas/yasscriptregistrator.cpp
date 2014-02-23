
#include "yasscriptregistrator.h"

#include "yascallbackfunctor.h"
#include "yasscriptmanager.h"

YasScriptRegistrator::YasScriptRegistrator(YasScriptManager& manager):
   mScriptManager(manager)
{
}

void YasScriptRegistrator::addClass(const String& name)
{
   mRegistry.addClass(name);
}

void YasScriptRegistrator::addFunction(const String& prototype, CallbackFnc callback)
{
   mRegistry.addFunction(prototype, new YasCallbackFunctor(mScriptManager, callback));
}

void YasScriptRegistrator::registerCallbacks()
{
   mScriptManager.getVM().mergeClassRegistry(mRegistry);
}
