
#include "virtualcallbackfunctor.h"

VirtualCallbackFunctor::VirtualCallbackFunctor(VMInterface::CallbackFnc callback) :
   CallbackFunctor(),
   mCallback(callback)
{

}

void VirtualCallbackFunctor::exec(VirtualCall& call)
{
   (*mCallback)(call);
}
