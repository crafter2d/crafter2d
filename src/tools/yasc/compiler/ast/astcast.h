
#ifndef AST_CAST_H_
#define AST_CAST_H_

#include "astexpressionpart.h"

class ASTType;

class ASTCast : public ASTExpressionPart
{
public:
   ASTCast();
   virtual ~ASTCast();

 // get/set
   const ASTType& getType() const;
         ASTType& getType();
   void           setType(ASTType* ptype);

   const ASTNode& getNode() const;
         ASTNode& getNode();
   void           setNode(ASTNode* pnode);

 // visitor
   ACCEPT;

private:
   ASTType* mpType;
   ASTNode* mpNode;
};

#endif // AST_CAST_H_
