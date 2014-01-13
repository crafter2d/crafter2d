
#ifndef AST_BLOCK_H_
#define AST_BLOCK_H_

#include "aststatement.h"

class CompileContext;

class ASTBlock : public ASTStatement
{
public:
   ASTBlock();

 // query
   const ASTStatement& getStatement(int index) const;

   virtual bool hasReturn(bool& hasunreachablecode) const;

 // operations
   void addStatement(ASTStatement* pstatement);

 // visitor
   ACCEPT;
};

#endif // AST_BLOCK_H_
