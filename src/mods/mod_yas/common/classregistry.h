
#ifndef NATIVE_REGISTRY_H
#define NATIVE_REGISTRY_H

#include <vector>

#include "core/string/string.h"

#include "mod_yas/script_base.h"
#include "mod_yas/vm/vminterface.h"

namespace yas
{
   class CallbackFunctor;
}

class ClassRegistration;
class FunctionRegistration;

class ClassRegistry
{
   typedef std::vector<ClassRegistration> Classes;
   typedef std::vector<FunctionRegistration*> Functions;

public:
   ClassRegistry();
   ~ClassRegistry();

   ClassRegistry& operator=(const ClassRegistry& that);


 // maintenance
   void add(const ClassRegistry& that);
   void addClass(const String& name);
   void addFunction(const String& name, yas::CallbackFunctor* pcallback);
   void clear();

 // search
   FunctionRegistration* findCallback(const String& function);

private:
 // operations
   void renumber();

   const FunctionRegistration& getFunction(int index) const;
   yas::CallbackFunctor& getFunctionCallback(int index) const;

 // searching
   const ClassRegistration* findClass(const String& name) const;
         ClassRegistration* findClass(const String& name);

   Classes              mClasses;
   Functions            mFunctions;
   ClassRegistration*   mpCurrent;
};

#endif // NATIVE_REGISTRY_H
