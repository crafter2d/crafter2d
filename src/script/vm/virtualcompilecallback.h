
#ifndef VIRTUAL_COMPILE_CALLBACK_H_
#define VIRTUAL_COMPILE_CALLBACK_H_

#include "script/compiler/compilecallback.h"

class VirtualMachine;

class VirtualCompileCallback : public CompileCallback
{
public:
   VirtualCompileCallback(VirtualMachine& vm);

 // callback
   virtual void notify(VirtualClass* pclass);

private:
   VirtualMachine& mVM;
};

#endif // VIRTUAL_COMPILE_CALLBACK_H_