
#ifndef AST_COMPOUND_H_
#define AST_COMPOUND_H_

#include "astexpressionpart.h"

class ASTExpression;

class ASTCompound : public ASTExpressionPart
{
public:
   ASTCompound();
   virtual ~ASTCompound();

 // get/set
   const ASTExpression& getExpression() const;
         ASTExpression& getExpression();
   void                 setExpression(ASTExpression* pexpression);
   
 // visitor
   ACCEPT;

private:
   ASTExpression* mpExpression;
};

#endif // AST_COMPOUND_H_
