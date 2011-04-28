
#ifndef AST_CONCATENATE_H_
#define AST_CONCATENATE_H_

#include "astnode.h"

class ASTExpression;

class ASTConcatenate : public ASTNode
{
public:
   enum Mode { eAnd, eOr, eEquals, eUnequals, eSmallerEqual, eSmaller, eGreater, eGreaterEqual,   // boolean operators
               eMul, eDiv, eRem, ePlus, eMinus,                                                   // number operators
               eBitwiseOr, eBitwiseXor, eBitwiseAnd, eShiftLeft, eShiftRight,                     // bit operators
               eNone, eInvalid };

   explicit ASTConcatenate(Mode mode);
   virtual ~ASTConcatenate();

 // get/set
   Mode getMode() const;

   const ASTNode& getLeft() const;
         ASTNode& getLeft();
         ASTNode* useLeft();
   void           setLeft(ASTNode* pleft);

   bool           hasRight() const;
   const ASTNode& getRight() const;
         ASTNode& getRight();
         ASTNode* useRight();
   void           setRight(ASTNode* pright);
   
 // visitor
   ACCEPT;

private:
   Mode mMode;
   ASTNode* mpLeft;
   ASTNode* mpRight;
};

#endif // AST_CONCATENATE_H_
