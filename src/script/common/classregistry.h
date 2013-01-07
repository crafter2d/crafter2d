
#ifndef NATIVE_REGISTRY_H
#define NATIVE_REGISTRY_H

#include <vector>

#include "script/script_base.h"
#include "script/vm/vminterface.h"

class ASTClass;
class ASTFunction;
class ClassRegistration;
class String;

class SCRIPT_API ClassRegistry
{
   typedef std::vector<ClassRegistration*> Classes;

public:
 // maintenance
   ClassRegistration&  addClass(const String& name);
   
   void merge(const ClassRegistry& that);
   void assign(const ClassRegistry& that);
   void collect(std::vector<VMInterface::CallbackFnc>& callbacks) const;

 // search
   const ClassRegistration* findClass(const String& name) const;
         ClassRegistration* findClass(const String& name);

   int findCallback(const ASTClass& astclass, const ASTFunction& function) const;

private:
   void renumber();

   Classes mClasses;
};

#endif // NATIVE_REGISTRY_H
