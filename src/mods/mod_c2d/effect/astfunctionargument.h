
#ifndef AST_FUNCTION_ARGUMENT_H
#define AST_FUNCTION_ARGUMENT_H

#include "astnode.h"

class ASTType;

class ASTFunctionArgument
{
public:
   ASTFunctionArgument(): mpType(nullptr), mName(), mArraySize(-1) {}

 // data
   ASTType* mpType;
   String   mName;
   int      mArraySize;
};

#endif // AST_FUNCTION_ARGUMENT_H
