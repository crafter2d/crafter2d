
#include "astblock.h"

#include "astvisitor.h"

ASTBlock::ASTBlock():
   ASTStatement()
{
}

// - Operations
   
void ASTBlock::addStatement(ASTStatement* pstatement)
{
   addChild(pstatement);
}

// - Visitor
   
ACCEPT_IMPL(ASTBlock)