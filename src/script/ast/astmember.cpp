
#include "astmember.h"

ASTMember::ASTMember(Kind kind):
   ASTNode(),
   mKind(kind)
{
}

ASTMember::~ASTMember()
{
}

// - Get/set

ASTMember::Kind ASTMember::getKind() const
{
   return mKind;
}
