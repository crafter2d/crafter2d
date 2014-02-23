
#ifndef AST_EXPRESSION_H_
#define AST_EXPRESSION_H_

#include "astnode.h"

class ASTExpression : public ASTNode
{
public:
   enum Kind { eUnary, eAssign, ePlusAssign, eMinAssign, eInvalid };

   ASTExpression();
   virtual ~ASTExpression();

 // get/set
   Kind getKind() const;
   void setKind(Kind kind);

   const ASTNode& getLeft() const;
         ASTNode& getLeft();
         ASTNode* useLeft();
   void           setLeft(ASTNode* pleft);

   bool                 hasRight() const;
   const ASTExpression& getRight() const;
         ASTExpression& getRight();
         ASTExpression* useRight();
   void                 setRight(ASTExpression* pright);

 // visitor
   ACCEPT;

private:
   ASTNode*       mpLeft;
   ASTExpression* mpRight;
   Kind mKind;
};

#endif // AST_EXPRESSION_H_
