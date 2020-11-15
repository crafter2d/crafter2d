
#include "astblock.h"

#include "libyasc/compilecontext.h"

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

bool ASTBlock::hasReturn(bool& hasunreachablecode) const
{
   const ASTNodes& statements = getChildren();

   for ( int index = 0; index < statements.size(); index++ )
   {
      const ASTStatement& statement = getStatement(index);
      if ( statement.hasReturn(hasunreachablecode) )
      {
         if ( index < statements.size() - 1 )
         {
            hasunreachablecode = true;
         }

         return true;
      }
   }
   return false;
}

// - Operations
   
void ASTBlock::addStatement(ASTStatement* pstatement)
{
   addChild(pstatement);
}

// - Visitor
   
ACCEPT_IMPL(ASTBlock)