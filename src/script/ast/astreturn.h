
#ifndef AST_RETURN_H_
#define AST_RETURN_H_

#include "aststatement.h"

class ASTExpression;

class ASTReturn : public ASTStatement
{
public:
   ASTReturn();
   virtual ~ASTReturn();

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

#endif // AST_RETURN_H_
