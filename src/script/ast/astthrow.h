
#ifndef AST_THROW_H_
#define AST_THROW_H_

#include "aststatement.h"

class ASTExpression;

class ASTThrow : public ASTStatement
{
public:
   ASTThrow();
   virtual ~ASTThrow();

 // get/set
   const ASTExpression& getExpression() const;
         ASTExpression& getExpression();
   void                 setExpression(ASTExpression* pexpression);

 // visitor
   ACCEPT;

private:
   ASTExpression* mpExpression;
};

#endif // AST_THROW_H_
