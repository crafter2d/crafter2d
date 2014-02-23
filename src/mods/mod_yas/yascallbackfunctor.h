
#ifndef __YasCallbackFunctor_h__
#define __YasCallbackFunctor_h__

#include "core/script/scriptregistrator.h"

#include "mod_yas/common/callbackfunctor.h"

class YasScriptManager;

class YasCallbackFunctor : public yas::CallbackFunctor
{
public:
   YasCallbackFunctor(YasScriptManager& manager, c2d::ScriptRegistrator::CallbackFnc callback);

 // overrides
   virtual void exec(VirtualCall& call) override;

private:

 // data
   YasScriptManager& mManager;
   c2d::ScriptRegistrator::CallbackFnc mCallback;
};

#endif // __YasCallbackFunctor_h__
