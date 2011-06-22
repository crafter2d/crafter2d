
#ifndef AST_STATEMENT_H_
#define AST_STATEMENT_H_

#include "astnode.h"

class ASTStatement : public ASTNode
{
public:
   ASTStatement();

 // query
   virtual bool hasReturn(bool& hasunreachablecode) const;
};

#endif // AST_STATEMENT_H_
