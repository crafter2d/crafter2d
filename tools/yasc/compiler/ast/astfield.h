
#ifndef AST_FIELD_H_
#define AST_FIELD_H_

#include "core/string/string.h"

#include "astmember.h"

class ASTVariable;

class ASTField: public ASTMember
{
public:
   explicit ASTField(ASTVariable* pvariable);
   virtual ~ASTField();

 // get/set
   const ASTVariable& getVariable() const;
         ASTVariable& getVariable();
   void               setVariable(ASTVariable* pvariable);

 // query
   String getQualifiedName() const;

 // visitor
   ACCEPT;

private:
   ASTVariable*   mpVariable;
};

#endif // AST_FIELD_H_
