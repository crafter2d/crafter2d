
#ifndef FUNCTION_REGISTRATION_H
#define FUNCTION_REGISTRATION_H

#include "core/string/string.h"

#include "script/script_base.h"
#include "script/vm/vminterface.h"

class SCRIPT_API FunctionRegistration
{
public:
   enum Type
   {
      eConstructor,
      eDestructor,
      eFunction
   };

   static FunctionRegistration Constructor(const String& prototype, VMInterface::CallbackFnc callback);
   static FunctionRegistration Destructor(VMInterface::CallbackFnc callback);
   static FunctionRegistration Function(const String& prototype, VMInterface::CallbackFnc callback);

   const FunctionRegistration& operator=(const FunctionRegistration& that);

 // get/set
   int getIndex() const;
   void setIndex(int index);

   Type getType() const;
   const String& getPrototype() const;
   VMInterface::CallbackFnc getCallback() const;

 // maintenance
   FunctionRegistration* clone() const;

private:
   FunctionRegistration(Type type, VMInterface::CallbackFnc callback);
   FunctionRegistration(Type type, const String& prototype, VMInterface::CallbackFnc callback);
   FunctionRegistration(const FunctionRegistration& that);

   int mIndex;
   Type mType;
   String mPrototype;
   VMInterface::CallbackFnc mCallback;
};

#endif // FUNCTION_REGISTRATION_H
