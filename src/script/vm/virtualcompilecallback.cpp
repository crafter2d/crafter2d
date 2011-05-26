
#include "virtualcompilecallback.h"

#include "virtualmachine.h"

VirtualCompileCallback::VirtualCompileCallback(VirtualMachine& vm):
   CompileCallback(),
   mVM(vm)
{
}

// - Callback

void VirtualCompileCallback::notify(VirtualClass* pclass)
{
   mVM.classLoaded(pclass);
}