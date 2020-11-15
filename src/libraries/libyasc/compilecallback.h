
#ifndef COMPILE_CALLBACK_H_
#define COMPILE_CALLBACK_H_

class VirtualClass;

class CompileCallback
{
public:
   CompileCallback();

   virtual void notify(VirtualClass* pclass) = 0;
};

#endif // COMPILE_CALLBACK_H_
