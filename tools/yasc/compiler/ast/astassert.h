
#ifndef AST_ASSERT_H
#define AST_ASSERT_H

#include "aststatement.h"

class ASTExpression;

class ASTAssert : public ASTStatement
{
public:
   ASTAssert();
   virtual ~ASTAssert();

 // get/set
   const ASTExpression& getCondition() const;
         ASTExpression& getCondition();
   void                 setCondition(ASTExpression* pcondition);

 // query
   virtual bool hasReturn(bool& hasunreachablecode) const;

 // visitor
   ACCEPT;

private:
   ASTExpression* mpCondition;
};

#endif // AST_ASSERT_H
