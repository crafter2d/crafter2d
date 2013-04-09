
#include "virtualcompilecallback.h"

#include "virtualmachine.h"

VirtualCompileCallback::VirtualCompileCallback(VirtualMachine& vm):
   CompileCallback(),
   mVM(vm)
{
}

// - Callback

void VirtualCompileCallback::notify(CIL::Class* pclass)
{
   mVM.classLoaded(pclass);
}