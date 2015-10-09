
#ifndef AST_STRUCT_ENTRY_H
#define AST_STRUCT_ENTRY_H

#include "core/string/string.h"

class ASTType;

class ASTStructEntry
{
public:
   ASTStructEntry(): ptype(NULL), name(), location(), slot(0) {}
   
   ASTType* ptype;
   String   name;
   String   location;
   UChar    slot;
};

#endif // AST_STRUCT_ENTRY_H
