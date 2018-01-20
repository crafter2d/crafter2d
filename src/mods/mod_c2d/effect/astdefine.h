
#ifndef AST_DEFINE_H
#define AST_DEFINE_H

#include <vector>

#include "astnode.h"

class ASTStructEntry;

class ASTDefine : public ASTNode
{
public:
   String mName;
   int    mValue;
};

#endif // AST_STRUCT_H

