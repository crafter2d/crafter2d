
#ifndef __VirtualCallbackFunctor_h__
#define __VirtualCallbackFunctor_h__

#include "mod_yas/common/callbackfunctor.h"

#include "vminterface.h"

class VirtualCallbackFunctor : public yas::CallbackFunctor
{
public:
   explicit VirtualCallbackFunctor(VMInterface::CallbackFnc callback);

 // overrides
   virtual void exec(VirtualCall& call) override;

private:

 // data
   VMInterface::CallbackFnc mCallback;
};

#endif // __VirtualCallbackFunctor_h__
