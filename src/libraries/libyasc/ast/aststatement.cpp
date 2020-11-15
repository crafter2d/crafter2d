
#include "aststatement.h"

#include "core/defines.h"

ASTStatement::ASTStatement():
   ASTNode()
{
}

// - Query

bool ASTStatement::hasReturn(bool& hasunreachablecode) const
{
   C2D_UNUSED(hasunreachablecode);
   return false;
}
