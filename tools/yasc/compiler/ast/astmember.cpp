
#include "astmember.h"

#include "core/defines.h"

ASTMember::ASTMember(Kind kind):
   ASTNode(),
   mpClass(nullptr),
   mKind(kind)
{
}

ASTMember::~ASTMember()
{
}

// - Get/set

const ASTClass& ASTMember::getClass() const
{
   return *mpClass;
}
   
void ASTMember::setClass(ASTClass& klass)
{
   mpClass = &klass;
}

ASTMember::Kind ASTMember::getKind() const
{
   return mKind;
}
