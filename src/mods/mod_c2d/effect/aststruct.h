
#ifndef AST_STRUCT_H
#define AST_STRUCT_H

#include <vector>

#include "astnode.h"

class ASTStructEntry;

class ASTStruct : public ASTNode
{
public:
   typedef std::vector<ASTStructEntry*> Entries;

   String mName;
   Entries mEntries;
};

#endif // AST_STRUCT_H
