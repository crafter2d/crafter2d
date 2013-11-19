
#ifndef AST_FUNCTION_H
#define AST_FUNCTION_H

#include "astnode.h"

#include <vector>

class ASTFunctionArgument;
class ASTType;

class ASTFunction : public ASTNode
{
public:
   typedef std::vector<ASTFunctionArgument*> Arguments;

   ASTType*    mpType;
   String      mName;
   String      mBody;
   String      mReturn;
   Arguments   mArguments;

   void addArgument(ASTFunctionArgument* parg) { mArguments.push_back(parg); }
};

#endif // AST_FUNCTION_H
