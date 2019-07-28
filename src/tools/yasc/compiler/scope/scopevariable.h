
#ifndef SCOPE_VARIABLE_H_
#define SCOPE_VARIABLE_H_

#include "core/string/string.h"

class ASTFunctionArgument;
class ASTLocalVariable;
class ASTVariable;
class ASTType;

class ScopeVariable
{
public:
   static ScopeVariable* fromVariable(const ASTVariable& variable);
   
   explicit ScopeVariable(ASTVariable& variable);
   ~ScopeVariable();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

   const ASTType& getType() const;
   void           setType(const ASTType* ptype);

   int  getResourceIndex() const;
   void setResourceIndex(int index);

   const ASTVariable& getVariable() const;
         ASTVariable& getVariable();

private:
   ASTVariable& mVariable;
};

#endif // SCOPE_VARIABLE_H_
