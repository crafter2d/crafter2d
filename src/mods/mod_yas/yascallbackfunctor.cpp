
#include "yascallbackfunctor.h"

#include "yasscriptcall.h"

YasCallbackFunctor::YasCallbackFunctor(YasScriptManager& manager, c2d::ScriptRegistrator::CallbackFnc callback):
   mManager(manager),
   mCallback(callback)
{

}

void YasCallbackFunctor::exec(VirtualCall& call)
{
   YasScriptCall yascall(mManager, call);
   (*mCallback)(yascall);
}
