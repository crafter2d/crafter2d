
#ifndef FUNCTION_REGISTRATION_H
#define FUNCTION_REGISTRATION_H

#include "core/string/string.h"

#include "mod_yas/script_base.h"
#include "mod_yas/vm/vminterface.h"

namespace yas
{
   class CallbackFunctor;
}

class ClassRegistration;

class FunctionRegistration
{
public:
   FunctionRegistration();
   FunctionRegistration(const String& prototype, yas::CallbackFunctor* pcallback);
   FunctionRegistration(const FunctionRegistration& that);
   FunctionRegistration(FunctionRegistration&& source);
   FunctionRegistration& operator=(FunctionRegistration&& source);
   
 // get/set
   int getIndex() const;
   void setIndex(int index);

   const ClassRegistration& getClassRegistration() const;
   void setClassRegistration(const ClassRegistration& klass);

   const String& getPrototype() const;
   yas::CallbackFunctor& getCallback() const;

   void execute(VirtualCall& call);

 // maintenance
   FunctionRegistration* clone() const;

private:
   

   String                     mPrototype;
   const ClassRegistration*   mpClass;
   yas::CallbackFunctor*      mpCallback;
   int                        mIndex;
};

#endif // FUNCTION_REGISTRATION_H
