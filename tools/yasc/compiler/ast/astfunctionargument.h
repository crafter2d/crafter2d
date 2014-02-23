
#ifndef AST_FUNCTION_ARGUMENT_H_
#define AST_FUNCTION_ARGUMENT_H_

#include "astnode.h"

class ASTVariable;

class ASTFunctionArgument : public ASTNode
{
public:
   explicit ASTFunctionArgument(ASTVariable* pvariable);
   virtual ~ASTFunctionArgument();

 // get/set
   const ASTVariable& getVariable() const;
         ASTVariable& getVariable();
   void               setVariable(ASTVariable* pvariable);

 // visitor
   ACCEPT;

private:
   ASTVariable*   mpVariable;
};

#endif // AST_FUNCTION_ARGUMENT_H_
