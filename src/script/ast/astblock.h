
#ifndef AST_BLOCK_H_
#define AST_BLOCK_H_

#include "aststatement.h"

class ASTBlock : public ASTStatement
{
public:
   ASTBlock();

 // operations
   void addStatement(ASTStatement* pstatement);

 // visitor
   ACCEPT;
};

#endif // AST_BLOCK_H_
