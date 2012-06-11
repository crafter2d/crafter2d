
#ifndef AST_ARRAY_INIT_H
#define AST_ARRAY_INIT_H

#include "astnode.h"

class ASTType;

class ASTArrayInit : public ASTNode
{
public:
   ASTArrayInit();

 // get/set
   const ASTType& getType() const;

 // operations
   void determineType();

 // visitor
   ACCEPT;

private:
   ASTType* mpType;
};

#endif // AST_ARRAY_INIT_H
