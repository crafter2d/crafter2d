
#ifndef FUNCTION_REGISTRATION_H
#define FUNCTION_REGISTRATION_H

#include "core/string/string.h"

#include "script/script_base.h"
#include "script/vm/vminterface.h"

namespace yas
{
   class CallbackFunctor;
}

class ClassRegistration;

class SCRIPT_API FunctionRegistration
{
public:
   static FunctionRegistration* create(const String& prototype, yas::CallbackFunctor* pcallback);
   
 // get/set
   int getIndex() const;
   void setIndex(int index);

   const ClassRegistration& getClassRegistration() const;
   void setClassRegistration(const ClassRegistration& klass);

   const String& getPrototype() const;
   yas::CallbackFunctor& getCallback() const;

 // maintenance
   FunctionRegistration* clone() const;

private:
   FunctionRegistration(const String& prototype, yas::CallbackFunctor* pcallback);
   FunctionRegistration(const FunctionRegistration& that);

   const ClassRegistration*   mpClass;
   int                        mIndex;
   String                     mPrototype;
   yas::CallbackFunctor*      mpCallback;
};

#endif // FUNCTION_REGISTRATION_H
