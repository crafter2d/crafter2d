
#ifndef AST_BUFFER_H
#define AST_BUFFER_H

#include "astnode.h"

class ASTBuffer : public ASTNode
{
public:
   ASTBuffer(): mName(), mBody(), mRegister(-1) {}

   String mName;
   String mBody;
   int    mRegister;
};

#endif // AST_BUFFER_H
