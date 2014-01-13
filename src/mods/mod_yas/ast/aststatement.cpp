
#include "aststatement.h"

ASTStatement::ASTStatement():
   ASTNode()
{
}

// - Query

bool ASTStatement::hasReturn(bool& hasunreachablecode) const
{
   return false;
}
