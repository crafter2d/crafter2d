
#include "astblock.h"

#include "aststatement.h"
#include "astvisitor.h"

ASTBlock::ASTBlock():
   ASTStatement()
{
}

// - Query

const ASTStatement& ASTBlock::getStatement(int index) const
{
   return dynamic_cast<const ASTStatement&>(getChildren()[index]);
}

// - Operations
   
void ASTBlock::addStatement(ASTStatement* pstatement)
{
   addChild(pstatement);
}

// - Visitor
   
ACCEPT_IMPL(ASTBlock)