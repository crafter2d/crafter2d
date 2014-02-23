
#ifndef AST_VARIABLE_INIT_H
#define AST_VARIABLE_INIT_H

#include "astnode.h"

class ASTArrayInit;
class ASTExpression;

class ASTVariableInit : public ASTNode
{
public:
   ASTVariableInit();
   ~ASTVariableInit();

 // get/set
   bool                 hasExpression() const;
   const ASTExpression& getExpression() const;
         ASTExpression& getExpression();
   void                 setExpression(ASTExpression* pexpression);

   bool                 hasArrayInit() const;
   const ASTArrayInit&  getArrayInit() const;
         ASTArrayInit&  getArrayInit();
   void                 setArrayInit(ASTArrayInit* parrayinit);

 // visitor
   ACCEPT;

private:
   ASTExpression* mpExpression;
   ASTArrayInit*  mpArrayInit;
};

#endif // AST_VARIABLE_INIT_H
