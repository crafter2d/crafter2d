

#ifndef AST_UNARY_H_
#define AST_UNARY_H_

#include "astnode.h"

class ASTAccess;
class ASTExpressionPart;

class ASTUnary : public ASTNode
{
public:
   enum Operator { ePos, eNegate, eNot, eIncrement, eDecrement, eNone };

   ASTUnary();

 // get/set
   Operator getPre() const;
   void     setPre(Operator op);

   Operator getPost() const;
   void     setPost(Operator op);

 // query
   const ASTAccess* asVariable() const;

   const ASTNodes& getParts() const;
         ASTNodes& getParts();

 // operations
   void addPart(ASTExpressionPart* ppart);

 // visitor
   ACCEPT;

private:
   Operator    mPre;
   Operator    mPost;
   ASTAccess*  mpAccess;
};

#endif // AST_UNARY_H_
