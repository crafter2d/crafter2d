
#ifndef NATIVE_REGISTRY_H
#define NATIVE_REGISTRY_H

#include <vector>

#include "core/string/string.h"

#include "script/script_base.h"
#include "script/vm/vminterface.h"

namespace yas
{
   class CallbackFunctor;
}

class ASTClass;
class ASTFunction;
class ClassRegistration;
class FunctionRegistration;

class SCRIPT_API ClassRegistry
{
   typedef std::vector<ClassRegistration*> Classes;
   typedef std::vector<FunctionRegistration*> Functions;

public:
   ClassRegistry();

   yas::CallbackFunctor& getCallback(int index) const;
   const FunctionRegistration& getFunction(int index) const;

 // maintenance
   void add(const ClassRegistry& that);
   void addClass(const String& name);
   void addFunction(const String& name, yas::CallbackFunctor* pcallback);

 // search
   const FunctionRegistration* findCallback(const ASTClass& astclass, const ASTFunction& function) const;
   const FunctionRegistration* findCallback(const ASTClass& astclass, const String& fncname) const;

private:
 // operations
   void renumber();

 // searching
   const ClassRegistration* findClass(const String& name) const;
         ClassRegistration* findClass(const String& name);

   Classes              mClasses;
   Functions            mFunctions;
   ClassRegistration*   mpCurrent;
};

#endif // NATIVE_REGISTRY_H
