
#ifndef AST_LOCAL_VARIABLE_H_
#define AST_LOCAL_VARIABLE_H_

#include "aststatement.h"

class ASTVariable;

class ASTLocalVariable : public ASTStatement
{
public:
   explicit ASTLocalVariable(ASTVariable* pvariable);
   virtual ~ASTLocalVariable();

 // get/set
   const ASTVariable& getVariable() const;
         ASTVariable& getVariable();
   void               setVariable(ASTVariable* pvariable);

 // visitor
   ACCEPT;

private:
   ASTVariable* mpVariable;
};

#endif // AST_LOCAL_VARIABLE_H_
