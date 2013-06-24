
#ifndef FUNCTION_REGISTRATION_H
#define FUNCTION_REGISTRATION_H

#include "core/string/string.h"

#include "script/script_base.h"
#include "script/vm/vminterface.h"

class ClassRegistration;

class SCRIPT_API FunctionRegistration
{
public:
   static FunctionRegistration* create(const String& prototype, VMInterface::CallbackFnc callback);
   
 // get/set
   int getIndex() const;
   void setIndex(int index);

   const ClassRegistration& getClassRegistration() const;
   void setClassRegistration(const ClassRegistration& klass);

   const String& getPrototype() const;
   VMInterface::CallbackFnc getCallback() const;

 // maintenance
   FunctionRegistration* clone() const;

private:
   FunctionRegistration(const String& prototype, VMInterface::CallbackFnc callback);
   FunctionRegistration(const FunctionRegistration& that);

   const ClassRegistration*   mpClass;
   int                        mIndex;
   String                     mPrototype;
   VMInterface::CallbackFnc   mCallback;
};

#endif // FUNCTION_REGISTRATION_H
