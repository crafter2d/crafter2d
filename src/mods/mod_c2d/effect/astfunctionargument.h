
#ifndef AST_FUNCTION_ARGUMENT_H
#define AST_FUNCTION_ARGUMENT_H

#include "astnode.h"

class ASTType;

class ASTFunctionArgument
{
public:

 // data
   ASTType* mpType;
   String   mName;
};

#endif // AST_FUNCTION_ARGUMENT_H
