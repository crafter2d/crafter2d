
#ifndef AST_STRUCT_ENTRY_H
#define AST_STRUCT_ENTRY_H

#include "core/string/string.h"

class ASTType;

class ASTStructEntry
{
public:
   
   ASTType* ptype;
   String   name;
   String   location;
};

#endif // AST_STRUCT_ENTRY_H
