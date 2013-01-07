
#ifndef CLASS_REGISTRATION_H
#define CLASS_REGISTRATION_H

#include <vector>

#include "core/string/string.h"

#include "script/script_base.h"
#include "script/vm/vminterface.h"

class ASTFunction;
class FunctionRegistration;

class SCRIPT_API ClassRegistration
{
public:
   ClassRegistration(const String& name);
   ClassRegistration(const ClassRegistration& that);
   const ClassRegistration& operator=(const ClassRegistration& that);

 // get/set
   const String& getName() const;

 // maintenance
   ClassRegistration* clone() const;

   void renumber(int& index);
   void merge(const ClassRegistration& that);
   void addFunction(const FunctionRegistration& function);
   void collect(std::vector<VMInterface::CallbackFnc>& callbacks) const;

 // search
   const FunctionRegistration* find(const ASTFunction& function) const;

private:
   typedef std::vector<FunctionRegistration*> Functions;

 // operations
   void renumber(int& startindex, Functions& functions);
   void merge(Functions& dest, const Functions& source);
   void copyTo(Functions& dest, const Functions& source);
   void collectFrom(std::vector<VMInterface::CallbackFnc>& callbacks, const Functions& source) const;

   String    mName;
   Functions mConstructors;
   Functions mDestructors;
   Functions mMembers;
};

#endif // CLASS_REGISTRATION_H
