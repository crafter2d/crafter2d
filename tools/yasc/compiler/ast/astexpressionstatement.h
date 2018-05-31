
#ifndef AST_EXPRESSION_STATEMENT_H_
#define AST_EXPRESSION_STATEMENT_H_

#include "aststatement.h"

class ASTExpression;

class ASTExpressionStatement : public ASTStatement
{
public:
   explicit ASTExpressionStatement(ASTExpression* pexpression = nullptr);
   virtual ~ASTExpressionStatement();

 // get/set
   bool                 hasExpression() const;
   const ASTExpression& getExpression() const;
         ASTExpression& getExpression();
   void                 setExpression(ASTExpression* pexpression);
   
 // visitor
   ACCEPT;

private:
   ASTExpression* mpExpression;
};

#endif // AST_EXPRESSION_STATEMENT_H_
