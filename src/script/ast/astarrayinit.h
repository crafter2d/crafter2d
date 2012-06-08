
#ifndef AST_ARRAY_INIT_H
#define AST_ARRAY_INIT_H

#include "astnode.h"

class ASTArrayInit : public ASTNode
{
public:
   ASTArrayInit();

 // visitor
   ACCEPT;
};

#endif // AST_ARRAY_INIT_H
